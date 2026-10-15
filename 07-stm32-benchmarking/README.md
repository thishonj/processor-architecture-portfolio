# 07 — STM32 Performance Benchmarking

**Status: planned.** Connects the architecture theory to real silicon. Fill in your exact board (e.g. NUCLEO-F4xx / Cortex-M?) before starting.

## Plan
1. Enable the **DWT cycle counter** (`DWT->CYCCNT`) on Cortex-M3/M4/M7 and measure empty-loop overhead first.
2. Benchmarks: array sum, memcpy, bubble sort, small FIR filter. Run each with `-O0`, `-O2`, `-Os`.
3. Experiments: stride access vs sequential (cache/prefetch effects if the part has them), flash wait states vs clock frequency, 32-bit vs 8-bit loads, `__attribute__((aligned))`.
4. Compare the generated ARM Thumb-2 assembly with the RV32 output from project 02.
5. Report: table of cycles, compiler flags, clock, core, toolchain version, and the raw code. Averages over ≥ 100 runs; state the measurement overhead.

## Deliverables
- [ ] `src/` (STM32CubeIDE project, generated files excluded via `.gitignore` where possible)
- [ ] `results.csv` + plots
- [ ] `analysis.md` that states what the numbers do **not** prove
