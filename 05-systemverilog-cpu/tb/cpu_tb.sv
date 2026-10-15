// cpu_tb.sv - self-checking testbench. Prints PASS/FAIL and returns a non-zero exit code on failure.
`timescale 1ns/1ps
module cpu_tb;
    logic clk = 0, rst = 1;
    logic [31:0] pc;
    logic illegal;
    int errors = 0;
    int illegal_seen = 0;

    cpu #(.PROGRAM("prog/test.hex")) dut (.clk, .rst, .pc_out(pc), .illegal);

    always #5 clk = ~clk;                       // 100 MHz

    always @(posedge clk) if (!rst && illegal) illegal_seen++;

    task automatic check_reg(input int n, input logic [31:0] expected);
        if (dut.regs[n] !== expected) begin
            $display("FAIL: x%0d = %h, expected %h", n, dut.regs[n], expected);
            errors++;
        end else $display("ok   : x%0d = %h", n, dut.regs[n]);
    endtask

    task automatic check_mem(input int w, input logic [31:0] expected);
        if (dut.dmem[w] !== expected) begin
            $display("FAIL: dmem[%0d] = %h, expected %h", w, dut.dmem[w], expected);
            errors++;
        end else $display("ok   : dmem[%0d] = %h", w, dut.dmem[w]);
    endtask

    initial begin
        $dumpfile("cpu_tb.vcd"); $dumpvars(0, cpu_tb);
        repeat (2) @(posedge clk);
        rst = 0;
        repeat (40) @(posedge clk);             // program is ~15 instructions, then spins in `halt`

        check_reg(0, 32'h0);                    // x0 must stay zero
        check_reg(1, 32'd5);   check_reg(2, 32'd7);
        check_reg(3, 32'd12);  check_reg(4, 32'd2);
        check_reg(5, 32'd12);  check_reg(6, 32'd2);
        check_reg(7, 32'd0);                    // skipped by taken BEQ
        check_reg(8, 32'd1);
        check_reg(9, 32'd42);                   // not-taken BEQ falls through
        check_reg(10, 32'hFFFF_FFFF);           // sign-extended immediate
        check_mem(0, 32'd12);  check_mem(1, 32'd2);

        if (illegal_seen != 0) begin
            $display("FAIL: illegal flag raised %0d times", illegal_seen); errors++;
        end
        if (pc !== 32'h3c) begin                // address of `halt` (check against disassembly)
            $display("FAIL: pc = %h, expected 0000003c (halt loop)", pc); errors++;
        end

        if (errors == 0) $display("PASS: all checks passed");
        else             $display("FAILED: %0d error(s)", errors);
        $finish(errors == 0 ? 0 : 1);
    end
endmodule
