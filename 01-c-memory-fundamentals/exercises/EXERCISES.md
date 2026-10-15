# Exercises

Write your solutions in `exercises/solutions/` (one `.c` file each). Compile with
`-Wall -Wextra -g` and run with `-fsanitize=address,undefined` before you call it done.

1. **Reverse in place.** Write `void reverse(int *arr, size_t len)` using two pointers (one from each end). Test with even and odd lengths and `len == 0`.
2. **Pointer arithmetic only.** Rewrite `sum()` without using `[]`. Then write `int *find_max(int *arr, size_t len)` that returns a *pointer* to the largest element.
3. **Predict, then run.** For `struct { char a; short b; char c; double d; }` predict `sizeof` and every `offsetof` on paper. Then verify. Reorder to minimise size.
4. **Dynamic array.** Implement `struct Vec { int *data; size_t len, cap; }` with `vec_push` (doubling `cap` via `realloc`) and `vec_free`. Check with ASan and Valgrind that there are no leaks. Handle `realloc` returning NULL.
5. **Fix the bugs.** Create `05_debug_bugs_fixed.c` where every case is corrected. Each fix needs a one-line comment saying what was wrong.
6. **String functions.** Implement your own `my_strlen`, `my_strcpy` (bounded), `my_strcmp`. Compare with the standard library on edge cases (empty string, equal prefixes).
7. **Endianness of a struct.** Write a function that prints the bytes of a `uint32_t` and a `float` (1.0f). Explain the result.
8. **Ring buffer, round two.** Add `rb_peek`, `rb_free_space`, and a test that fills, drains, and refills over several wrap-arounds.
9. **GDB session.** Debug the `null` case using only GDB. Save your command transcript as `gdb_notes.md`.
10. **Challenge.** Write a function-pointer table `int (*ops[])(int,int)` for add/sub/mul and call through it. Look up why this matters for interrupt vector tables.
