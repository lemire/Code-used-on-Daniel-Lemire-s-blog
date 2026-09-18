# Alternatives to the `str p` inline assembly in simdjson PR #2863

- `pred.cpp`: four ways to turn four SVE predicates into a 64-bit mask (inline asm, `svsel`+`addp`,
  `bext`+`tbl`, `memcpy` from the predicate's address) with a scalar reference and a randomized test.
  Build with `-march=armv9-a+sve2+sve2-bitperm`; run under QEMU at VL 128/256/512 (all agree).
- `asm_g++.s`, `asm_clang++.s`: the assembly for the above (GCC 16, clang 21).
- `strp1..4.cpp`: what the compilers say about storing a predicate without asm.
- `bridge.patch`: the PR with the `svsel`/`addp` extraction and a shared final `addp` (Mixed4 writer disabled).
- `memcpy.patch`: the PR with the `memcpy` extraction (Mixed4 writer disabled).
- `results/{g4,g5}`: base vs `prasm` (PR, Mixed4 disabled) vs `bridge`, `-mcpu=native`, 3 runs each.
- `results_memcpy/{g4,g5}`: base vs `prasm` vs `memcpy`, same protocol.

Stage-1 geomean vs NEON base (G4 gcc / G4 clang / G5 gcc / G5 clang):
asm 1.08 / 1.14 / 1.06 / 1.08; sel+addp 1.06 / 1.10 / 1.03 / 1.04; memcpy 1.02 / 1.13 / 1.00 / 1.07.

## pack4 (predicate stores at 2-byte stride, Daniel's idea)

- `pack4.cpp`: standalone test of `pack4_masks` (+ padded variant + overflow probe). At VL > 128 the
  unpadded version overflows its 8-byte buffer (GCC's stack protector aborts at VL 256/512).
- `pack4.patch`: the PR with `operator_predicates_to_bitmask` = byte buffer `[8 + 32]`, four predicate
  stores at 2-byte stride, one 8-byte `memcpy` reload (Mixed4 writer disabled).
- `results_pack4/{g4,g5}`: base / prasm / pack4 (3 runs each, `-mcpu=native`). On g4 also:
  `pack4b` = four 2-byte reloads (GCC folds them back into the sizeless-memcpy codegen, `addpl`),
  `pack4c` = compiler barrier + four 2-byte reloads.
- Stage 1 vs NEON, G4 gcc / G4 clang / G5 gcc / G5 clang: asm 1.08 / 1.14 / 1.06 / 1.08;
  pack4 0.98 / 1.14 / 0.98 / 1.08; (G4 only) pack4b 1.02 / 1.14; pack4c 1.04 / 1.15.
