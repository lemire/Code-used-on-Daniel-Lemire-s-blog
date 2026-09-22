#!/usr/bin/env python3
"""Commit-by-commit benchmark sweep for simdutf, CRoaring and roaring (Go).

  sweep.py <project> build [--jobs N]   export + build every first-parent commit
  sweep.py <project> bench [--core C]   run the fixed harness on each built commit
  sweep.py <project> collect            write results/<project>.tsv

Layout (under ~/histbench): <project>/ is the clone, work/<project>/<sha>/ is a
throwaway export, bin/<project>/<sha> the harness binary, out/<project>/<sha>.txt
the benchmark output, logs/<project>/<sha>.log the build log.
"""
import os, sys, subprocess, shutil, re, argparse
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.expanduser("~/histbench")
SINCE = "2024-08-01"
HARNESS = os.path.join(ROOT, "harness")
DATA_SIMDUTF = os.path.expanduser("~/subench/data")
DATA_ROARING = os.path.join(ROOT, "CRoaring", "benchmarks", "realdata")

def sh(cmd, cwd=None, log=None, env=None):
    with open(log, "ab") if log else open(os.devnull, "wb") as f:
        f.write(("\n$ " + cmd + "\n").encode())
        f.flush()
        r = subprocess.run(cmd, shell=True, cwd=cwd, stdout=f, stderr=subprocess.STDOUT, env=env)
    return r.returncode == 0

def commits(project):
    repo = os.path.join(ROOT, project)
    out = subprocess.check_output(
        ["git", "log", "--first-parent", "--reverse", f"--since={SINCE}",
         "--format=%H\t%cI\t%s", "origin/HEAD"], cwd=repo, text=True)
    rows = []
    for line in out.splitlines():
        sha, date, subj = line.split("\t", 2)
        rows.append((sha, date, subj))
    return rows

def tags(project):
    repo = os.path.join(ROOT, project)
    out = subprocess.check_output(["git", "tag", "--format=%(objectname)\t%(refname:short)"],
                                  cwd=repo, text=True)
    t = {}
    for line in out.splitlines():
        sha, name = line.split("\t")
        # tags may be annotated: resolve to commit
        sha = subprocess.check_output(["git", "rev-parse", name + "^{commit}"], cwd=repo, text=True).strip()
        t.setdefault(sha, name)
    return t

def export(project, sha):
    work = os.path.join(ROOT, "work", project, sha)
    if os.path.isdir(work):
        shutil.rmtree(work)
    os.makedirs(work)
    repo = os.path.join(ROOT, project)
    subprocess.check_call(f"git archive {sha} | tar -x -C {work}", shell=True, cwd=repo)
    return work

def build_one(project, sha):
    binp = os.path.join(ROOT, "bin", project, sha)
    fail = binp + ".FAIL"
    if os.path.exists(binp) or os.path.exists(fail):
        return
    log = os.path.join(ROOT, "logs", project, sha + ".log")
    os.makedirs(os.path.dirname(log), exist_ok=True)
    os.makedirs(os.path.dirname(binp), exist_ok=True)
    if os.path.exists(log):
        os.remove(log)
    work = export(project, sha)
    ok = False
    try:
        if project == "simdutf":
            ok = (sh("python3 singleheader/amalgamate.py", cwd=work, log=log) and
                  sh(f"g++ -O3 -std=c++17 -Isingleheader {HARNESS}/simdutf_bench.cpp "
                     f"singleheader/simdutf.cpp -o {binp}", cwd=work, log=log))
        elif project == "CRoaring":
            ok = (sh("bash amalgamation.sh", cwd=work, log=log) and
                  sh(f"gcc -O3 -std=gnu11 -I. {HARNESS}/croaring_bench.c roaring.c -o {binp} -lm",
                     cwd=work, log=log))
        elif project == "roaring":
            gb = os.path.join(work, "_gobench")
            shutil.copytree(os.path.join(HARNESS, "gobench"), gb)
            with open(os.path.join(HARNESS, "gobench", "go.mod.template")) as f:
                mod = f.read().replace("__ROARING_DIR__", work)
            with open(os.path.join(gb, "go.mod"), "w") as f:
                f.write(mod)
            env = dict(os.environ, GOFLAGS="-mod=mod", CGO_ENABLED="0")
            ok = (sh("go mod tidy", cwd=gb, log=log, env=env) and
                  sh(f"go build -o {binp} .", cwd=gb, log=log, env=env))
    finally:
        shutil.rmtree(work, ignore_errors=True)
    if not ok:
        open(fail, "w").close()
    print(sha[:12], "OK" if ok else "FAIL", flush=True)

def bench_one(project, sha, core):
    binp = os.path.join(ROOT, "bin", project, sha)
    outp = os.path.join(ROOT, "out", project, sha + ".txt")
    if not os.path.exists(binp) or os.path.exists(outp):
        return
    os.makedirs(os.path.dirname(outp), exist_ok=True)
    if project == "simdutf":
        cmd = f"taskset -c {core} {binp} {DATA_SIMDUTF}"
    else:
        cmd = f"taskset -c {core} {binp} {DATA_ROARING}"
    with open(outp + ".tmp", "wb") as f:
        r = subprocess.run(cmd, shell=True, stdout=f, stderr=subprocess.STDOUT)
    if r.returncode == 0:
        os.rename(outp + ".tmp", outp)
        print(sha[:12], "benched", flush=True)
    else:
        os.rename(outp + ".tmp", outp + ".FAIL")
        print(sha[:12], "BENCH FAIL", flush=True)

def collect(project):
    rows = commits(project)
    t = tags(project)
    resd = os.path.join(ROOT, "results")
    os.makedirs(resd, exist_ok=True)
    outf = os.path.join(resd, project + ".tsv")
    n = 0
    with open(outf, "w") as w:
        w.write("date\tsha\ttag\tmetric\tvalue\tsubject\n")
        for sha, date, subj in rows:
            outp = os.path.join(ROOT, "out", project, sha + ".txt")
            if not os.path.exists(outp):
                continue
            n += 1
            for line in open(outp):
                if project == "simdutf":
                    m = re.match(r"(\S+)\s+\S+ bytes\s+\S+ s\s+(\S+) GB/s", line)
                    if m:
                        w.write(f"{date[:10]}\t{sha[:12]}\t{t.get(sha,'')}\t{m.group(1)}\t{m.group(2)}\t{subj}\n")
                else:
                    m = re.match(r"(\S+)\s+(\S+)\s+(\S+) us", line)
                    if m:
                        w.write(f"{date[:10]}\t{sha[:12]}\t{t.get(sha,'')}\t{m.group(1)}/{m.group(2)}\t{m.group(3)}\t{subj}\n")
    print(f"{project}: {n} commits -> {outf}")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("project", choices=["simdutf", "CRoaring", "roaring"])
    ap.add_argument("phase", choices=["build", "bench", "collect", "list"])
    ap.add_argument("--jobs", type=int, default=16)
    ap.add_argument("--core", type=int, default=8)
    a = ap.parse_args()
    rows = commits(a.project)
    if a.phase == "list":
        for sha, date, subj in rows:
            print(date[:10], sha[:12], subj)
        print(len(rows), "commits")
    elif a.phase == "build":
        with ThreadPoolExecutor(a.jobs) as ex:
            list(ex.map(lambda r: build_one(a.project, r[0]), rows))
        print("BUILD DONE", flush=True)
    elif a.phase == "bench":
        for sha, _, _ in rows:
            bench_one(a.project, sha, a.core)
        print("BENCH DONE", flush=True)
    else:
        collect(a.project)

if __name__ == "__main__":
    main()
