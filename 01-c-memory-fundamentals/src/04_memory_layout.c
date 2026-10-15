/* 04_memory_layout.c - Where do variables live? Stack, heap, .data, .bss, .rodata, .text
 * Also: endianness (byte order), which RISC-V and ARM Cortex-M both use as little-endian. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int  g_initialized = 7;            /* .data   : global with a non-zero initial value */
int  g_zero;                       /* .bss    : global, zero-initialised at startup   */
const int g_const = 123;           /* .rodata : read-only data                        */

static void show_stack_frame(int depth)
{
    int local = depth;             /* stack: each call gets a new frame */
    printf("  depth %d: &local = %p\n", depth, (void *)&local);
    if (depth < 2) {
        show_stack_frame(depth + 1);
    }
}

int main(void)
{
    int local = 1;                 /* stack */
    int *heap = malloc(sizeof *heap);   /* heap */
    if (heap == NULL) {
        return 1;
    }
    *heap = 5;

    printf("Addresses (exact values change each run; compare the REGIONS):\n");
    printf("  .text   (function main)  %p\n", (void *)(uintptr_t)main);
    printf("  .rodata (g_const)        %p\n", (void *)&g_const);
    printf("  .data   (g_initialized)  %p\n", (void *)&g_initialized);
    printf("  .bss    (g_zero)         %p\n", (void *)&g_zero);
    printf("  heap    (malloc)         %p\n", (void *)heap);
    printf("  stack   (local in main)  %p\n", (void *)&local);

    printf("Nested calls get different stack frames:\n");
    show_stack_frame(0);

    free(heap);
    heap = NULL;                   /* avoid a dangling pointer */

    /* Endianness: store 0x11223344 and look at the bytes in memory. */
    uint32_t word = 0x11223344u;
    const uint8_t *bytes = (const uint8_t *)&word;
    printf("0x11223344 in memory, lowest address first: %02x %02x %02x %02x\n",
           bytes[0], bytes[1], bytes[2], bytes[3]);
    printf("This machine is %s-endian\n", (bytes[0] == 0x44) ? "little" : "big");
    return 0;
}
