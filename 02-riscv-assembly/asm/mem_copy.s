# mem_copy.s - copy 4 words from src to dst, then load dst[3]. Expected: a0 = 40
    .data
src:    .word 10, 20, 30, 40
dst:    .word 0, 0, 0, 0

    .text
    .globl _start
_start:
    la   t0, src
    la   t1, dst
    li   t2, 4
copy:
    beq  t2, zero, done
    lw   t3, 0(t0)
    sw   t3, 0(t1)
    addi t0, t0, 4
    addi t1, t1, 4
    addi t2, t2, -1
    j    copy
done:
    la   t1, dst
    lw   a0, 12(t1)       # dst[3]
    li   a7, 10
    ecall
