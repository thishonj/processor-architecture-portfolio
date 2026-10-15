/* 01_pointers.c - What a pointer is, and how to use one safely. */
#include <stdio.h>
#include <stddef.h>

/* C passes arguments BY VALUE. To modify the caller's variable,
 * we pass its ADDRESS (a pointer) and write through it. */
static void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(void)
{
    int x = 42;
    int *p = &x;          /* p holds the ADDRESS of x */
    int **pp = &p;        /* pp holds the address of p (pointer to pointer) */

    printf("x     = %d\n", x);
    printf("*p    = %d   (dereference: the value p points to)\n", *p);
    printf("**pp  = %d\n", **pp);
    printf("p == &x ? %s\n", (p == &x) ? "yes" : "no");

    *p = 100;             /* writing through the pointer changes x */
    printf("after *p = 100 -> x = %d\n", x);

    /* Pointer arithmetic: p + 1 moves by sizeof(*p) BYTES, not 1 byte. */
    int a[4] = {10, 20, 30, 40};
    int *q = a;
    for (int i = 0; i < 4; i++) {
        printf("q[%d] = %d   *(q+%d) = %d\n", i, q[i], i, *(q + i));
    }
    printf("sizeof(int) = %zu bytes\n", sizeof(int));
    printf("byte distance between q+1 and q = %td\n",
           (char *)(q + 1) - (char *)q);

    /* swap via pointers */
    int m = 1, n = 2;
    swap(&m, &n);
    printf("after swap: m = %d, n = %d\n", m, n);

    /* const correctness: who is allowed to change what? */
    const int *ro = &x;        /* pointer to const int: cannot write *ro   */
    int *const fixed = &x;     /* const pointer to int: cannot reassign it */
    printf("*ro = %d, *fixed = %d\n", *ro, *fixed);
    /* *ro = 5;      <- compile error: read-only through ro  */
    /* fixed = &m;   <- compile error: fixed cannot move      */

    /* NULL means "points to nothing". Always check before dereferencing. */
    int *np = NULL;
    if (np == NULL) {
        printf("np is NULL, so we do NOT dereference it\n");
    }

    printf("address of x (varies per run): %p\n", (void *)&x);
    return 0;
}
