# Requires: riscv64-unknown-elf-gcc toolchain (Ubuntu: sudo apt install gcc-riscv64-unknown-elf)
AS := riscv64-unknown-elf-as
LD := riscv64-unknown-elf-ld
OC := riscv64-unknown-elf-objcopy
OD := riscv64-unknown-elf-objdump
GCC := riscv64-unknown-elf-gcc
ARCH := -march=rv32i -mabi=ilp32
PROGS := sum_array gcd mem_copy

.SECONDARY:
.PHONY: all check disasm c-asm clean
all: $(PROGS:%=build/%.bin)

build/%.o: asm/%.s | build
	$(AS) $(ARCH) $< -o $@
build/%.elf: build/%.o
	$(LD) -m elf32lriscv --no-relax -Ttext=0x0 -Tdata=0x1000 $< -o $@
build/%.bin: build/%.elf
	$(OC) -O binary $< $@
build:
	mkdir -p build

check: all
	@for p in $(PROGS); do printf "%-10s " $$p; python3 tools/rv32sim.py build/$$p.bin; done

disasm: all
	$(OD) -d -M no-aliases build/sum_array.elf

c-asm:
	@for o in O0 O2; do $(GCC) $(ARCH) -$$o -S c/sum_array.c -o build/sum_array_$$o.s; done
	@echo "wrote build/sum_array_O0.s and build/sum_array_O2.s"

clean:
	rm -rf build
