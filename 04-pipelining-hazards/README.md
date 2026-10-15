# 04 — Pipelining and Hazards

**Status: planned.** Concepts and the evidence I will produce.

## The 5-stage pipeline

```
cycle:      1    2    3    4    5    6    7
add x3,..   IF   ID   EX   MEM  WB
sub x4,..        IF   ID   EX   MEM  WB
lw  x5,..             IF   ID   EX   MEM  WB
```

## Hazard types

| Type | Example | Fix |
|------|---------|-----|
| Structural | one memory port for instruction and data | separate I/D memory |
| Data (RAW) | `add x3,x1,x2` then `sub x4,x3,x5` | forwarding from EX/MEM or MEM/WB |
| Load-use | `lw x5,0(x1)` then `add x6,x5,x2` | forwarding **plus** one-cycle stall |
| Control | `beq` resolved in EX | predict not-taken, flush 1–2 instructions on taken |

## Plan
1. In **Ripes** run `02-riscv-assembly/asm/sum_array.s` on the 5-stage core with forwarding on/off. Record cycles and CPI in a table; screenshot the stall.
2. Hand-draw pipeline diagrams (cycle-by-cycle) for three short programs: no hazard, RAW, load-use.
3. Write a small Python script that counts stall cycles for a given instruction sequence (with/without forwarding) and compare with Ripes.
4. Extend project 05 into a pipelined version with forwarding unit + hazard detection; reuse the same testbench and reference model.

## Deliverables
- [ ] `ripes_results.md` (table + screenshots)
- [ ] `hazard_diagrams.md`
- [ ] `stall_counter.py` + tests
- [ ] pipelined RTL in project 05 (v0.2)
