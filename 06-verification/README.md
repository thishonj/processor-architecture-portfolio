# 06 — Verification

**Status: planned.** The first self-checking testbench already exists in
[05-systemverilog-cpu/tb](../05-systemverilog-cpu/tb/cpu_tb.sv); this project makes verification a first-class topic.

## Plan
1. Learn the vocabulary: DUT, testbench, stimulus, checker/scoreboard, reference model, coverage, assertion, regression, mutation.
2. Add **SystemVerilog assertions** to the CPU (x0 always 0, PC word aligned, no write when `reg_write=0`).
3. **Constrained-random** instruction generator (Python is fine) → hex → run on RTL and `rv32sim.py` → compare register state.
4. Add functional coverage ideas: every opcode, taken/not-taken branch, negative immediates, back-to-back dependent instructions. Report coverage honestly (hand-counted or via Verilator `--coverage`).
5. Optionally try **cocotb** (free, Python) and a first look at **UVM** concepts using EDA Playground's UVM-capable simulators. Be careful in the README to say "UVM-style" unless you actually use UVM.
6. Keep a `bugs_found.md`: every real bug, how it was found, the fix.

## Deliverables
- [ ] assertions file + results
- [ ] random test generator and comparison script
- [ ] coverage table
- [ ] `bugs_found.md`
