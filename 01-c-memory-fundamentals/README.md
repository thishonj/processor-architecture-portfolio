# 01 — C Memory Fundamentals

**Goal:** understand exactly where C data lives and how pointers address it. Processor
architecture work (assembly, load/store instructions, caches, MMIO registers) builds on this.

**Status:** code complete and tested on Linux (GCC, x86-64) with sanitizers. Exercises are for me to work through.

## What is in here

| File | Concept |
|------|---------|
| `src/01_pointers.c` | address-of, dereference, pointer arithmetic, `const`, NULL, swap |
| `src/02_arrays_strings.c` | array decay, `sizeof` trap, row-major 2D arrays, `\0` strings, safe copy |
| `src/03_structs_padding.c` | alignment, padding, `offsetof`, register-block layout |
| `src/04_memory_layout.c` | `.text` `.rodata` `.data` `.bss` heap stack, endianness |
| `src/05_debug_bugs.c` | six deliberate bugs for ASan / Valgrind / GDB practice |
| `src/06_ring_buffer.c` | capstone: circular buffer with self-checking tests |
| `exercises/EXERCISES.md` | exercises to extend the code |

## Build and run

Requires `gcc` (or `clang`) and `make`. Linux, macOS, or WSL2 on Windows.

```bash
cd 01-c-memory-fundamentals
make            # build everything into build/
make run        # run programs 01–04 and 06
make asan       # build the bug demo with AddressSanitizer + UBSan
make clean
```

Manual compile of a single file:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g src/01_pointers.c -o build/01_pointers
```

## Concepts in plain language

- **Memory is a long row of numbered byte slots.** The number is the *address*.
- **A pointer is a variable that stores an address.** `&x` = "address of x", `*p` = "the thing at address p".
- **`p + 1` moves by one element, not one byte.** For `int *p`, that is 4 bytes.
- **An array name becomes a pointer to its first element when passed to a function.** The length is lost, so pass it separately.
- **A struct is laid out in order, but the compiler inserts padding** so each member sits at a properly aligned address. Order members largest-first to waste less space.
- **Stack** holds local variables (freed automatically on return). **Heap** holds `malloc`ed memory (you must `free` it). **`.data`/`.bss`/`.rodata`** hold globals and constants. **`.text`** holds code.
- **Endianness:** little-endian stores the *least* significant byte at the lowest address. x86, ARM Cortex-M (STM32) and RISC-V are all little-endian in practice.

## Expected output

Addresses change on every run (ASLR); everything else is deterministic.

`./build/01_pointers`
```
x     = 42
*p    = 42   (dereference: the value p points to)
**pp  = 42
p == &x ? yes
after *p = 100 -> x = 100
q[0] = 10   *(q+0) = 10
q[1] = 20   *(q+1) = 20
q[2] = 30   *(q+2) = 30
q[3] = 40   *(q+3) = 40
sizeof(int) = 4 bytes
byte distance between q+1 and q = 4
after swap: m = 2, n = 1
*ro = 100, *fixed = 100
np is NULL, so we do NOT dereference it
address of x (varies per run): 0x7ffe61b1e180
```

`./build/02_arrays_strings`
```
sizeof(data) = 20 bytes, len = 5 elements
  inside sum(): sizeof(arr) = 8 (pointer size!)
sum = 15
grid as flat memory: 1 2 3 4 5 6
name = "RISC-V", strlen = 6, sizeof = 16
bytes: 52 49 53 43 2d 56 00
snprintf into char[4] -> "OVE" (truncated, no overflow)
arr_str = Xbc, lit = abc
```
(`sizeof(arr) = 8` assumes a 64-bit machine; it is 4 on a 32-bit target such as STM32.)

`./build/03_structs_padding`
```
sizeof(struct Bad)  = 12
  offset a=0  b=4  c=8
sizeof(struct Good) = 8
  offset b=0  a=4  c=5
Periph offsets: CTRL=0 STATUS=4 DATA=8 (size 12)
point after move = (4, 2)
original.x = 4, copy.x = 99
```

`./build/04_memory_layout` (last two lines are stable; addresses vary)
```
0x11223344 in memory, lowest address first: 44 33 22 11
This machine is little-endian
```
Look at the address printout: the `.text`, `.rodata`, `.data`, `.bss` addresses are close together, the heap is elsewhere, and the stack is at a much higher address. Each nested call's `local` sits at a *lower* address than the previous one on x86-64 (the stack typically grows downward).

`./build/06_ring_buffer`
```
ring buffer: all tests passed
```

## Debugging practice (`05_debug_bugs.c`)

The compiler already warns about two of the bugs (`use-after-free`, `uninitialized`). Read those warnings first — fixing warnings is the cheapest debugging there is.

### AddressSanitizer (compile-time instrumentation)

```bash
make asan
./build/05_debug_bugs_asan overflow    # heap-buffer-overflow, reports file:line
./build/05_debug_bugs_asan uaf         # heap-use-after-free
./build/05_debug_bugs_asan leak        # LeakSanitizer lists the unfreed 100 bytes
./build/05_debug_bugs_asan null        # SEGV at address 0
./build/05_debug_bugs_asan stack       # stack-buffer-overflow
```

Tested result: ASan/UBSan reported `overflow`, `uaf`, `leak`, `null` and `stack` with the correct source line.
**It did not flag `uninit`** — ASan does not detect reads of uninitialised memory. Use Valgrind (below) or MemorySanitizer (`clang -fsanitize=memory`).

### Valgrind (Linux)

```bash
gcc -g -O0 src/05_debug_bugs.c -o build/05_debug_bugs_vg
valgrind --leak-check=full ./build/05_debug_bugs_vg uninit
valgrind --leak-check=full ./build/05_debug_bugs_vg leak
```

### GDB basics

```bash
gdb --args ./build/05_debug_bugs null
(gdb) run            # program crashes with SIGSEGV
(gdb) bt             # backtrace: which function called which
(gdb) frame 0
(gdb) print p        # inspect variable (p = 0x0)
(gdb) info locals
```
Other useful commands: `break file.c:LINE`, `next`, `step`, `continue`, `watch var`, `x/8xb &var` (dump 8 bytes in hex), `print &var`, `ptype struct Bad`.

> Verification note: I ran the compile and ASan steps and the program outputs above. The Valgrind and GDB commands are standard but not part of the automated check; run them yourself and paste real output into your notes.

## What I learned

*(Fill this in honestly after finishing the exercises. Examples: "I now understand why `sizeof` on a function parameter gives the pointer size", "I found my own off-by-one with ASan".)*

## Limitations

- Tested on x86-64 Linux/GCC only; sizes and addresses differ on 32-bit targets such as STM32.
- Stack-growth direction is observed, not guaranteed by the C standard.
- The ring buffer is single-threaded and not interrupt-safe.

## Exercises

See [`exercises/EXERCISES.md`](exercises/EXERCISES.md).
