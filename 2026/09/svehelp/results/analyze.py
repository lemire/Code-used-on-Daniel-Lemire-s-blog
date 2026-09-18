import math, collections, os, sys, glob
RES=sys.argv[1]
MACH=[("g3","Graviton 3","c7g.2xlarge","Neoverse-V1","SVE (256-bit), no SVE2"),
      ("g4","Graviton 4","c8g.2xlarge","Neoverse-V2","SVE2 (128-bit)"),
      ("g5","Graviton 5","c9g.2xlarge","Neoverse-V3","SVE2 (128-bit)")]
CONF=[("gcc15-default","gcc-15, default flags"),("clang21-default","clang-21, default flags"),
      ("gcc15-native","gcc-15, -mcpu=native"),("clang21-native","clang-21, -mcpu=native")]
def load(path):
    d={}
    for line in open(path):
        p=line.rstrip('\n').split('\t')
        if len(p)<8: continue
        d[p[0].strip('"')]=(float(p[5]),float(p[6]),float(p[7]))  # all, s1, s2
    return d
def best(m,c,t):
    acc=collections.defaultdict(lambda:[0,0,0]); reps=[]
    for rep in (1,2,3):
        f=f"{RES}/{m}/{c}.{t}.{rep}.tsv"
        if not os.path.exists(f): return None,None
        r=load(f); reps.append(r)
        for k,v in r.items():
            for i in range(3): acc[k][i]=max(acc[k][i],v[i])
    spread=[max(r[k][1] for r in reps)/min(r[k][1] for r in reps) for k in acc]
    return acc, math.exp(sum(map(math.log,spread))/len(spread))
def geo(a,b,i): return math.exp(sum(math.log(b[k][i]/a[k][i]) for k in a)/len(a))
def minmax(a,b,i):
    r={k:b[k][i]/a[k][i] for k in a}; lo=min(r,key=r.get); hi=max(r,key=r.get)
    return r[lo],lo,r[hi],hi
out=[]
P=out.append
P("# simdjson PR #2863 on AWS Graviton 3 / 4 / 5\n")
P("**PR:** [simdjson/simdjson#2863](https://github.com/simdjson/simdjson/pull/2863) — *perf(arm64): optimize Stage 1 matching and index writing* (head `265ea481`), against its merge-base `9572dd09` (master).\n")
P("**Date:** 2026-09-17\n")
P("## What the PR changes\n")
P("1. **Mixed4 structural-index writer** (`bit_indexer::write` in `src/arm64.cpp`): writes the first 4 indexes, then groups of 2 up to 24, then scalar. Always compiled in on arm64; plain NEON/scalar code, no SVE needed.")
P("2. **SVE2 `svmatch` operator classifier** (`json_character_block::classify`): replaces the NEON `tbl` lookup for the operator class (`,:[]{}`) with `svmatch_u8` on 16-byte NEON-SVE-bridge vectors, and extracts the predicate with `str p`. Whitespace still uses NEON `tbl`. **Compile-time gated** on `__ARM_FEATURE_SVE2 && __ARM_NEON_SVE_BRIDGE`, i.e. only active with `-mcpu=native` / `-march=armv9-a+sve2`; there is no runtime dispatch. `MATCH` is an SVE2 instruction, so this path cannot be used on Graviton 3 (SVE only).\n")
P("## Method\n")
P("- One instance per processor type, us-east-1, Ubuntu 26.04 arm64, gcc 15.2 and clang 21.1 from apt.")
P("- `benchmark/dom/parse -t -n 300 <22 files>` (the simdjson-data corpus, ≈24 MB total), `taskset -c 3`, Release build, `SIMDJSON_DEVELOPER_MODE=ON`, static lib.")
P("- `parse -t` reports the *best* of 300 iterations per file; each binary was run 3 times interleaved with its counterpart (base, PR, base, PR, ...) and the best of the 3 is used. Run-to-run spread of stage 1 (geomean over files of max/min across the 3 runs) is reported as *noise* below.")
P("- Four builds per tree: gcc-15 / clang-21 × default flags / `-mcpu=native`. Under `-mcpu=native` two extra PR variants isolate the two changes: **Mixed4 only** (SVE2 macro suppressed) and **SVE2 match only** (Mixed4 macro suppressed).")
P("- I checked in every binary that the SVE `match` instruction appears only where expected (16 copies in PR `-mcpu=native` builds on G4/G5; 0 everywhere else).")
P("- Ratios are **PR / base** throughput (GB/s); >1 means the PR is faster. Geomean over the 22 files.\n")
P("## Headline: geomean PR / base\n")
P("| Machine | Build | Stage 1 | Stage 2 | Whole parse | Noise | S1 worst file | S1 best file |")
P("|---|---|---|---|---|---|---|---|")
for m,name,it,cpu,sve in MACH:
    for c,cl in CONF:
        B,nb=best(m,c,"base"); Pp,_=best(m,c,"pr")
        if B is None or Pp is None: P(f"| {name} | {cl} | (missing) | | | | | |"); continue
        lo,lof,hi,hif=minmax(B,Pp,1)
        P(f"| {name} | {cl} | **{geo(B,Pp,1):.3f}** | {geo(B,Pp,2):.3f} | {geo(B,Pp,0):.3f} | {nb-1:.1%} | {lo:.3f} ({lof}) | {hi:.3f} ({hif}) |")
P("")
P("## Isolating the two changes (`-mcpu=native`, geomean PR-variant / base)\n")
P("| Machine | Compiler | Variant | Stage 1 | Stage 2 | Whole parse |")
P("|---|---|---|---|---|---|")
for m,name,it,cpu,sve in MACH:
    for c,cl in CONF[2:]:
        B,_=best(m,c,"base")
        for t,lab in [("pr","full PR"),("pr-nosve","Mixed4 writer only"),("pr-nomixed4","SVE2 match only")]:
            Pp,_=best(m,c,t)
            if Pp is None: P(f"| {name} | {cl.split(',')[0]} | {lab} | n/a | | |"); continue
            P(f"| {name} | {cl.split(',')[0]} | {lab} | {geo(B,Pp,1):.3f} | {geo(B,Pp,2):.3f} | {geo(B,Pp,0):.3f} |")
P("")
P("On Graviton 3 the two `-mcpu=native` PR variants are not listed because the SVE2 path does not compile there (SVE only); the full-PR row *is* the Mixed4-only measurement.\n")
P("## Absolute stage-1 throughput, base vs PR (GB/s, best)\n")
P("| Machine | Build | base S1 | PR S1 | base total | PR total |")
P("|---|---|---|---|---|---|")
for m,name,it,cpu,sve in MACH:
    for c,cl in CONF:
        B,_=best(m,c,"base"); Pp,_=best(m,c,"pr")
        if B is None or Pp is None: continue
        gm=lambda D,i: math.exp(sum(math.log(D[k][i]) for k in D)/len(D))
        P(f"| {name} | {cl} | {gm(B,1):.2f} | {gm(Pp,1):.2f} | {gm(B,0):.2f} | {gm(Pp,0):.2f} |")
P("\n(geomean over files of the per-file GB/s)\n")
P("## Per-file stage-1 ratios (PR / base)\n")
files=None
for m,name,it,cpu,sve in MACH:
    P(f"### {name} ({it}, {cpu}, {sve})\n")
    cols=[(c,cl) for c,cl in CONF]
    P("| file | "+" | ".join(cl for c,cl in cols)+" |")
    P("|---|"+"---|"*len(cols))
    data={c:best(m,c,"base")[0] for c,_ in cols}; pdata={c:best(m,c,"pr")[0] for c,_ in cols}
    files=sorted(next(iter(data.values())).keys())
    for k in files:
        P(f"| {k} | "+" | ".join(f"{pdata[c][k][1]/data[c][k][1]:.3f}" for c,_ in cols)+" |")
    P("| **geomean** | "+" | ".join(f"**{geo(data[c],pdata[c],1):.3f}**" for c,_ in cols)+" |")
    P("")
open(sys.argv[2],"w").write("\n".join(out)+"\n")
print("wrote",sys.argv[2])
