# gcd.s - greatest common divisor by repeated subtraction. Expected: a0 = 6
    .text
    .globl _start
_start:
    li   a0, 48
    li   a1, 18
gcd_loop:
    beq  a0, a1, done     # equal -> finished
    blt  a0, a1, b_larger
    sub  a0, a0, a1       # a > b : a -= b
    j    gcd_loop
b_larger:
    sub  a1, a1, a0       # b > a : b -= a
    j    gcd_loop
done:
    li   a7, 10
    ecall
