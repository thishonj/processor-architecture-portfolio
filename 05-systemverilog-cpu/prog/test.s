# test.s - uses only ADD, SUB, ADDI, LW, SW, BEQ (what the CPU supports)
    .text
    .globl _start
_start:
    addi x1, x0, 5
    addi x2, x0, 7
    add  x3, x1, x2        # 12
    sub  x4, x2, x1        # 2
    sw   x3, 0(x0)         # dmem[0] = 12
    sw   x4, 4(x0)         # dmem[1] = 2
    lw   x5, 0(x0)         # x5 = 12
    lw   x6, 4(x0)         # x6 = 2
    beq  x5, x3, taken     # taken (12 == 12)
    addi x7, x0, 99        # must be skipped
taken:
    addi x8, x0, 1
    beq  x1, x2, skip2     # not taken (5 != 7)
    addi x9, x0, 42        # must execute
skip2:
    addi x10, x0, -1       # negative immediate: 0xFFFFFFFF
    addi x0, x0, 123       # write to x0 must be ignored
halt:
    beq  x0, x0, halt      # spin forever
