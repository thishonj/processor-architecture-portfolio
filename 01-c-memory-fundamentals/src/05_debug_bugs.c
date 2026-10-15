/* 05_debug_bugs.c - Deliberate memory bugs for practising debugging tools.
 *
 * Usage: ./05_debug_bugs <case>
 *   overflow  heap buffer overflow
 *   uaf       use after free
 *   leak      memory leak
 *   uninit    use of uninitialised value
 *   null      NULL pointer dereference
 *   stack     stack buffer overflow
 *
 * Run it with AddressSanitizer / Valgrind / GDB (see README). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void heap_overflow(void)
{
    int *a = malloc(4 * sizeof(int));
    a[4] = 1;                      /* BUG: valid indices are 0..3 */
    free(a);
}

static void use_after_free(void)
{
    int *a = malloc(sizeof(int));
    *a = 1;
    free(a);
    printf("value = %d\n", *a);    /* BUG: memory already freed */
}

static void leak(void)
{
    char *s = malloc(100);
    strcpy(s, "never freed");      /* BUG: no free(s) */
}

static void uninit(void)
{
    int x;
    if (x > 0) {                   /* BUG: x was never set */
        printf("positive\n");
    }
}

static void null_deref(void)
{
    int *p = NULL;
    *p = 5;                        /* BUG: writing to address 0 */
}

static void stack_overflow(void)
{
    char buf[8];
    int i;
    volatile char *vb = buf;       /* volatile: stop compiler hiding the bug */
    for (i = 0; i <= 8; i++) {     /* BUG: <= writes buf[8] */
        vb[i] = 'A';
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s overflow|uaf|leak|uninit|null|stack\n", argv[0]);
        return 2;
    }
    if      (!strcmp(argv[1], "overflow")) heap_overflow();
    else if (!strcmp(argv[1], "uaf"))      use_after_free();
    else if (!strcmp(argv[1], "leak"))     leak();
    else if (!strcmp(argv[1], "uninit"))   uninit();
    else if (!strcmp(argv[1], "null"))     null_deref();
    else if (!strcmp(argv[1], "stack"))    stack_overflow();
    else { fprintf(stderr, "unknown case\n"); return 2; }
    return 0;
}
