# Requires Icarus Verilog (sudo apt install iverilog) and optionally GTKWave.
.PHONY: hex sim wave clean
hex:
	./prog/build_hex.sh
sim:
	mkdir -p build
	iverilog -g2012 -o build/cpu_tb.vvp rtl/cpu.sv tb/cpu_tb.sv
	vvp build/cpu_tb.vvp
wave: sim
	gtkwave cpu_tb.vcd
clean:
	rm -rf build cpu_tb.vcd
