#!/usr/bin/env python3
"""Tiny RV32I instruction-set simulator used to sanity-check the .s programs.

NOT a full simulator: supports only the instructions the examples use.
Not a replacement for Ripes; its job is to give a reproducible command-line
check (and later, a golden reference model for the SystemVerilog CPU).

Usage: rv32sim.py program.bin [max_steps]   (binary linked at address 0)
"""
import struct, sys

def sext(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v

def run(binary, max_steps=100000):
    mem = bytearray(binary) + bytearray(65536 - len(binary))
    x = [0] * 32
    pc = 0
    for _ in range(max_steps):
        ins = struct.unpack_from("<I", mem, pc)[0]
        op, rd = ins & 0x7F, (ins >> 7) & 31
        f3, rs1, rs2, f7 = (ins >> 12) & 7, (ins >> 15) & 31, (ins >> 20) & 31, ins >> 25
        imm_i = sext(ins >> 20, 12)
        imm_s = sext(((ins >> 25) << 5) | ((ins >> 7) & 31), 12)
        imm_b = sext(((ins >> 31) << 12) | (((ins >> 7) & 1) << 11) |
                     (((ins >> 25) & 63) << 5) | (((ins >> 8) & 15) << 1), 13)
        imm_u = ins & 0xFFFFF000
        imm_j = sext(((ins >> 31) << 20) | (((ins >> 12) & 255) << 12) |
                     (((ins >> 20) & 1) << 11) | (((ins >> 21) & 1023) << 1), 21)
        npc = pc + 4
        s = lambda r: sext(x[r], 32)
        if   op == 0x33 and f3 == 0:                       # add / sub
            x[rd] = (x[rs1] - x[rs2] if f7 == 0x20 else x[rs1] + x[rs2]) & 0xFFFFFFFF
        elif op == 0x13 and f3 == 0:                       # addi
            x[rd] = (x[rs1] + imm_i) & 0xFFFFFFFF
        elif op == 0x03 and f3 == 2:                       # lw
            x[rd] = struct.unpack_from("<I", mem, (x[rs1] + imm_i) & 0xFFFFFFFF)[0]
        elif op == 0x23 and f3 == 2:                       # sw
            struct.pack_into("<I", mem, (x[rs1] + imm_s) & 0xFFFFFFFF, x[rs2])
        elif op == 0x63:                                   # branches
            taken = {0: x[rs1] == x[rs2], 1: x[rs1] != x[rs2], 4: s(rs1) < s(rs2),
                     5: s(rs1) >= s(rs2)}[f3]
            if taken: npc = pc + imm_b
        elif op == 0x6F:                                   # jal
            x[rd] = pc + 4; npc = pc + imm_j
        elif op == 0x37:                                   # lui
            x[rd] = imm_u
        elif op == 0x17:                                   # auipc
            x[rd] = (pc + imm_u) & 0xFFFFFFFF
        elif op == 0x73:                                   # ecall: a7 == 10 -> exit
            if x[17] == 10:
                return x
            raise SystemExit(f"unsupported ecall a7={x[17]}")
        else:
            raise SystemExit(f"unsupported instruction {ins:#010x} at pc={pc:#x}")
        x[0] = 0
        pc = npc
    raise SystemExit("max steps exceeded")

if __name__ == "__main__":
    regs = run(open(sys.argv[1], "rb").read(), int(sys.argv[2]) if len(sys.argv) > 2 else 100000)
    print(f"a0 = {regs[10]}  a1 = {regs[11]}")
