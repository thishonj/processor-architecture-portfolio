#!/bin/sh
# Assemble prog/test.s into prog/test.hex (one 32-bit word per line) for $readmemh.
set -e
cd "$(dirname "$0")"
riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 test.s -o test.o
riscv64-unknown-elf-ld -m elf32lriscv --no-relax -Ttext=0 test.o -o test.elf
riscv64-unknown-elf-objcopy -O binary test.elf test.bin
python3 - <<'PY'
import struct
d = open("test.bin","rb").read()
d += b"\0" * (-len(d) % 4)
open("test.hex","w").write("".join(f"{w:08x}\n" for w in struct.unpack("<%dI" % (len(d)//4), d)))
PY
rm -f test.o test.elf test.bin
echo "wrote prog/test.hex"
