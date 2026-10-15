# sum_array.s - sum five words from memory. Expected: a0 = 35
# RV32I. Works in Ripes (paste into the editor) and with the GNU assembler.
    .data
arr:    .word 3, 5, 7, 9, 11

    .text
    .globl _start
_start:
    la   t0, arr          # t0 = pointer to arr[0]
    li   t1, 5            # t1 = elements left
    li   a0, 0            # a0 = running sum
loop:
    beq  t1, zero, done   # if no elements left, finish
    lw   t2, 0(t0)        # t2 = *t0
    add  a0, a0, t2       # sum += t2
    addi t0, t0, 4        # advance pointer by one word (4 bytes)
    addi t1, t1, -1       # count--
    j    loop
done:
    li   a7, 10           # Ripes/RARS environment call: exit
    ecall
