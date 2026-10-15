/* 06_ring_buffer.c - Capstone: a fixed-size circular buffer with self-checking tests.
 * Typical use on microcontrollers: UART receive queues, sample buffers.
 * No dynamic memory; the caller owns the storage. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t *buf;       /* storage provided by caller */
    size_t   capacity;
    size_t   head;      /* next write index */
    size_t   tail;      /* next read index  */
    size_t   count;     /* items stored     */
} RingBuffer;

static void rb_init(RingBuffer *rb, uint8_t *storage, size_t capacity)
{
    rb->buf = storage;
    rb->capacity = capacity;
    rb->head = rb->tail = rb->count = 0;
}

static bool rb_push(RingBuffer *rb, uint8_t value)
{
    if (rb->count == rb->capacity) {
        return false;                       /* full: refuse, never overwrite */
    }
    rb->buf[rb->head] = value;
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count++;
    return true;
}

static bool rb_pop(RingBuffer *rb, uint8_t *out)
{
    if (rb->count == 0) {
        return false;                       /* empty */
    }
    *out = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;
    return true;
}

int main(void)
{
    uint8_t storage[4];
    RingBuffer rb;
    uint8_t v;

    rb_init(&rb, storage, sizeof storage);

    assert(!rb_pop(&rb, &v));               /* empty buffer */
    for (uint8_t i = 1; i <= 4; i++) {
        assert(rb_push(&rb, i));
    }
    assert(!rb_push(&rb, 5));               /* full buffer rejects */
    assert(rb_pop(&rb, &v) && v == 1);      /* FIFO order */
    assert(rb_push(&rb, 6));                /* wrap-around works */
    assert(rb_pop(&rb, &v) && v == 2);
    assert(rb_pop(&rb, &v) && v == 3);
    assert(rb_pop(&rb, &v) && v == 4);
    assert(rb_pop(&rb, &v) && v == 6);
    assert(!rb_pop(&rb, &v));

    printf("ring buffer: all tests passed\n");
    return 0;
}
