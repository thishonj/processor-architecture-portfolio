# 03 — Digital Logic

**Status: planned.** Building blocks I need before the CPU makes sense.

## Plan
1. **HDLBits** (https://hdlbits.01xz.net): finish *Getting Started*, *Verilog Language → Basics/Vectors/Modules*, *Circuits → Combinational*, *Sequential*. Keep a table of exercises done.
2. **Nand2Tetris** (https://www.nand2tetris.org), projects 1–3 (logic gates, ALU, registers/RAM). Use the free software suite; keep only *your own* HDL solutions and respect the course's request not to publish solutions publicly (check the current course policy; if in doubt, publish notes and diagrams, not solutions).
3. Re-implement in SystemVerilog and simulate locally: full adder → 32-bit ripple adder → ALU (add, sub, and, or, slt) → register file (2 read, 1 write) → 8-bit counter.
4. For each block: one module, one self-checking testbench, one sentence on what the testbench does *not* cover.

## Deliverables
- [ ] `hdlbits_progress.md` (list of exercises, honest completion status)
- [ ] `rtl/` + `tb/` for adder, ALU, register file
- [ ] Short note: ripple-carry vs carry-lookahead (delay, not just theory)
