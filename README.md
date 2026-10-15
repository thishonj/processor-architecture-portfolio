# Processor Architecture Portfolio

![CI](https://github.com/<your-username>/processor-architecture-portfolio/actions/workflows/ci.yml/badge.svg)

Learning projects toward **CPU design, RTL design and design verification**.
I am an M.Sc. ICT student (Embedded Systems) at FAU Erlangen-Nürnberg. I know embedded C
and have an STM32 board; I am building the rest of the stack from the ground up and
publishing the work as I go.

> **Honesty note:** this is a learning portfolio. Each project lists exactly what is done,
> what was tested, and what is *not* done. Nothing here has been taped out, run on an FPGA,
> or verified to industry standards unless a project says so.

## Status

| # | Project | Topics | Status |
|---|---------|--------|--------|
| 01 | [C memory fundamentals](01-c-memory-fundamentals/) | pointers, arrays, structs, memory layout, ASan/GDB, ring buffer | 🟢 Code complete — exercises in progress |
| 02 | [RISC-V assembly](02-riscv-assembly/) | RV32I, Ripes, Compiler Explorer | 🟡 Programs verified; Ripes evidence pending |
| 03 | [Digital logic](03-digital-logic/) | gates, adders, ALU, registers (HDLBits / Nand2Tetris) | ⚪ Planned |
| 04 | [Pipelining & hazards](04-pipelining-hazards/) | 5-stage pipeline, data/control hazards, forwarding | ⚪ Planned |
| 05 | [SystemVerilog CPU](05-systemverilog-cpu/) | single-cycle CPU: ADD, SUB, ADDI, LW, SW, BEQ | 🟡 v0.1 simulated (Icarus), testbench mutation-checked |
| 06 | [Verification](06-verification/) | self-checking testbenches, assertions, coverage ideas | ⚪ Planned |
| 07 | [STM32 benchmarking](07-stm32-benchmarking/) | cycle counting (DWT), compiler flags, memory effects | ⚪ Planned |

Legend: 🟢 done and reproducible · 🟡 in progress · ⚪ planned

## Skills (stated conservatively)

| Area | Level | Evidence |
|------|-------|----------|
| Embedded C | Working knowledge | STM32 coursework/projects (link when added) |
| C pointers & memory | Revising | [Project 01](01-c-memory-fundamentals/) |
| RISC-V assembly | Learning | Project 02 (planned) |
| SystemVerilog / RTL | Beginner | Projects 03–05 (planned) |
| Verification | Beginner | Project 06 (planned) |

## Tools (all free)

GCC/Clang, GDB, Valgrind, Make · [Ripes](https://github.com/mortbopet/Ripes) ·
[Compiler Explorer](https://godbolt.org) · [Nand2Tetris](https://www.nand2tetris.org) ·
[HDLBits](https://hdlbits.01xz.net) · [EDA Playground](https://www.edaplayground.com) ·
Icarus Verilog / Verilator / GTKWave (later) · STM32CubeIDE.

## Repository layout

```
processor-architecture-portfolio/
├── README.md
├── LICENSE
├── .gitignore
├── .github/workflows/ci.yml
├── docs/                      roadmap, skills matrix, keywords, review checklist
├── 01-c-memory-fundamentals/  src/ exercises/ Makefile README.md
├── 02-riscv-assembly/
├── 03-digital-logic/
├── 04-pipelining-hazards/
├── 05-systemverilog-cpu/
├── 06-verification/
└── 07-stm32-benchmarking/
```

## Quick start

```bash
git clone https://github.com/<your-username>/processor-architecture-portfolio.git
cd processor-architecture-portfolio/01-c-memory-fundamentals
make run
```

## Contact

<your-name> · <LinkedIn URL> · <email>

Licensed under MIT. See [LICENSE](LICENSE).
