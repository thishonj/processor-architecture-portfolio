/* 02_arrays_strings.c - Arrays, "decay", 2D arrays, strings. */
#include <stdio.h>
#include <string.h>

/* Inside a function, an array parameter is really just a pointer.
 * sizeof(arr) here is the size of a POINTER, not of the array.
 * That is why we must pass the length separately. */
static int sum(const int *arr, size_t len)
{
    int total = 0;
    for (size_t i = 0; i < len; i++) {
        total += arr[i];
    }
    printf("  inside sum(): sizeof(arr) = %zu (pointer size!)\n", sizeof(arr));
    return total;
}

int main(void)
{
    int data[5] = {1, 2, 3, 4, 5};
    size_t len = sizeof(data) / sizeof(data[0]);   /* only works on the real array */

    printf("sizeof(data) = %zu bytes, len = %zu elements\n", sizeof(data), len);
    printf("sum = %d\n", sum(data, len));

    /* 2D arrays are stored row after row (row-major) in ONE contiguous block. */
    int grid[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int *flat = &grid[0][0];
    printf("grid as flat memory:");
    for (int i = 0; i < 6; i++) {
        printf(" %d", flat[i]);
    }
    printf("\n");

    /* Strings are char arrays ending with '\0'. */
    char name[16] = "RISC-V";
    printf("name = \"%s\", strlen = %zu, sizeof = %zu\n",
           name, strlen(name), sizeof(name));

    /* Bytes of the string, including the terminator: */
    printf("bytes:");
    for (size_t i = 0; i <= strlen(name); i++) {
        printf(" %02x", (unsigned char)name[i]);
    }
    printf("\n");

    /* Safe copying: always bound the size. */
    char small[4];
    const char *long_text = "OVERFLOW?";
    snprintf(small, sizeof(small), "%s", long_text);
    printf("snprintf into char[4] -> \"%s\" (truncated, no overflow)\n", small);

    /* Array vs string literal pointer */
    char arr_str[] = "abc";        /* modifiable copy on the stack        */
    const char *lit = "abc";       /* points to read-only memory (.rodata) */
    arr_str[0] = 'X';
    printf("arr_str = %s, lit = %s\n", arr_str, lit);
    /* lit[0] = 'X';   <- undefined behaviour (usually crashes) */
    return 0;
}
