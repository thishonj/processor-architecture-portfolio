# 02 — RISC-V Assembly (RV32I)

**Goal:** read, write and trace RV32I assembly, and see how C maps to instructions.
This is the instruction set my SystemVerilog CPU (project 05) implements a subset of.

**Status:** 3 programs written, assembled with the GNU toolchain, and checked with a small
Python simulator. **Not yet done:** Ripes pipeline screenshots (do these yourself, see below).

## Contents

| Path | What |
|------|------|
| `asm/sum_array.s` | loop + `lw` + pointer increment → `a0 = 35` |
| `asm/gcd.s` | `beq`, `blt`, `sub` loop → `a0 = 6` |
| `asm/mem_copy.s` | `lw` / `sw` copy loop → `a0 = 40` |
| `c/sum_array.c` | C function to compile and compare at `-O0` vs `-O2` |
| `tools/rv32sim.py` | ~60-line RV32I interpreter (subset) for command-line checks |

## Reproduce

```bash
sudo apt install gcc-riscv64-unknown-elf python3   # Ubuntu / WSL2
cd 02-riscv-assembly
make check        # assemble, link, run in the mini simulator
make disasm       # see the real machine code
make c-asm        # generate C → RISC-V assembly at -O0 and -O2
```

Expected output of `make check`:
```
sum_array  a0 = 35  a1 = 0
gcd        a0 = 6  a1 = 6
mem_copy   a0 = 40  a1 = 0
```

> `rv32sim.py` supports only the instructions these programs use. A passing result here
> means "my program and my mini model agree", not "verified on real hardware".
> Always cross-check in Ripes too.

## Linker note (a real gotcha I hit)

Without `--no-relax`, the linker rewrote `la t1, dst` into a `gp`-relative `addi`. Bare-metal
code never initialises `gp`, so the store went to a garbage address. The Makefile passes `--no-relax`.

## Part A — Ripes walkthrough (free, runs on Windows/Linux/macOS)

1. Download Ripes from https://github.com/mortbopet/Ripes/releases.
2. *Select processor* → **5-stage processor** (RV32I). Choose "Assembly" as input.
3. Paste `asm/sum_array.s`. Replace `_start:` with `main:` if Ripes complains (label names are free).
4. Step with the **Single instruction** button. Watch the five stages (IF, ID, EX, MEM, WB).
5. Record: cycles, instructions retired, CPI. Save a screenshot to `docs/` (see TODO list).
6. Repeat with the **single-cycle** processor and compare cycles and CPI.
7. Find the stall/flush caused by `beq` or `jal` and by `lw` followed by a dependent `add`.
   (In `sum_array.s`, `lw t2` → `add a0,a0,t2` is a **load-use hazard**.)

## Part B — Compiler Explorer (https://godbolt.org)

1. Language **C**, compiler **RISC-V rv32gc gcc** (or "RISC-V (32-bits) gcc").
2. Add flags `-march=rv32i -mabi=ilp32 -O0`, paste `c/sum_array.c`. Then switch to `-O2`.
3. Compare. My local result with GCC 13:

`-O2` (core loop, 12 instructions; the loop body is 5):
```
sum:    ble   a1,zero,.L4        # n <= 0 → return 0
        slli  a1,a1,2            # n*4
        mv    a5,a0              # p = a
        add   a3,a0,a1           # end = a + n
        li    a0,0
.L3:    lw    a4,0(a5)
        addi  a5,a5,4
        add   a0,a0,a4
        bne   a5,a3,.L3
        ret
.L4:    li    a0,0
        ret
```
`-O0` is 41 lines: every variable lives in the stack frame, with loads/stores around each use. `-O2` keeps everything in registers and replaces the index `i` with a moving pointer.

**Questions to answer in your own words** (put the answers in `analysis.md`):
- Which instructions access memory at `-O0` vs `-O2`? How many per iteration?
- What does `ret` expand to? (`jalr x0, 0(ra)`)
- Why is the loop counter replaced by an end pointer?
- How many cycles would a 5-stage pipeline spend per iteration if `lw`→`add` needs a stall?

## Part C — Instruction-format table (fill in by hand)

Decode these by hand from `make disasm` and check against the RISC-V spec (Volume I, "RV32I Base Instruction Set"):
`00750533` (`add a0,a0,t2`), `0002a383` (`lw t2,0(t0)`), `00030c63` (`beq`), `fedff06f` (`jal`).
Write opcode, rd, funct3, rs1, rs2, funct7, immediate for each. This exercise is the most useful preparation for writing a decoder.

## TODO (honest checklist)

- [ ] Ripes screenshots: 5-stage and single-cycle, with cycle/CPI numbers
- [ ] `analysis.md` answering the questions above
- [ ] Hand-decoded instruction table
- [ ] One more program of your own (e.g. bubble sort, strlen)
