/* 03_structs_padding.c - Struct layout, alignment, padding.
 * Why it matters: hardware registers, network packets and cache
 * behaviour all depend on exact byte layout. */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

struct Bad {            /* members ordered carelessly */
    uint8_t  a;         /* 1 byte  */
    uint32_t b;         /* 4 bytes, needs 4-byte alignment */
    uint8_t  c;         /* 1 byte  */
};

struct Good {           /* same members, largest first */
    uint32_t b;
    uint8_t  a;
    uint8_t  c;
};

/* A made-up peripheral, laid out like a memory-mapped register block. */
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t STATUS;
    volatile uint32_t DATA;
} Periph;

struct Point { int x; int y; };

static void move(struct Point *pt, int dx, int dy)
{
    pt->x += dx;        /* pt->x is shorthand for (*pt).x */
    pt->y += dy;
}

int main(void)
{
    printf("sizeof(struct Bad)  = %zu\n", sizeof(struct Bad));
    printf("  offset a=%zu  b=%zu  c=%zu\n",
           offsetof(struct Bad, a), offsetof(struct Bad, b), offsetof(struct Bad, c));
    printf("sizeof(struct Good) = %zu\n", sizeof(struct Good));
    printf("  offset b=%zu  a=%zu  c=%zu\n",
           offsetof(struct Good, b), offsetof(struct Good, a), offsetof(struct Good, c));

    printf("Periph offsets: CTRL=%zu STATUS=%zu DATA=%zu (size %zu)\n",
           offsetof(Periph, CTRL), offsetof(Periph, STATUS),
           offsetof(Periph, DATA), sizeof(Periph));

    struct Point pt = {3, 4};
    move(&pt, 1, -2);
    printf("point after move = (%d, %d)\n", pt.x, pt.y);

    /* Structs can be copied with '=' (a byte-wise copy). */
    struct Point copy = pt;
    copy.x = 99;
    printf("original.x = %d, copy.x = %d\n", pt.x, copy.x);
    return 0;
}
