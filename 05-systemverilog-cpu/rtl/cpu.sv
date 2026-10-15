// cpu.sv - Single-cycle RV32 subset CPU (educational).
// Supported: ADD, SUB, ADDI, LW, SW, BEQ.  Anything else is treated as a NOP and flagged by `illegal`.
// Not supported: other ALU ops, shifts, JAL/JALR, LUI/AUIPC, byte/half accesses, CSRs, exceptions.
module cpu #(
    parameter IMEM_WORDS = 256,
    parameter DMEM_WORDS = 256,
    parameter string PROGRAM = "prog/test.hex"
) (
    input  logic        clk,
    input  logic        rst,
    output logic [31:0] pc_out,
    output logic        illegal
);
    // ---------------- memories ----------------
    logic [31:0] imem [0:IMEM_WORDS-1];
    logic [31:0] dmem [0:DMEM_WORDS-1];
    logic [31:0] regs [0:31];

    initial begin
        for (int i = 0; i < IMEM_WORDS; i++) imem[i] = 32'h0000_0013; // NOP (addi x0,x0,0)
        for (int i = 0; i < DMEM_WORDS; i++) dmem[i] = 32'h0;
        $readmemh(PROGRAM, imem);
    end

    // ---------------- fetch ----------------
    logic [31:0] pc, instr, pc_next;
    assign instr  = imem[pc[31:2]];
    assign pc_out = pc;

    // ---------------- decode ----------------
    logic [6:0] opcode; logic [2:0] funct3; logic [6:0] funct7;
    logic [4:0] rs1, rs2, rd;
    assign opcode = instr[6:0];   assign rd     = instr[11:7];
    assign funct3 = instr[14:12]; assign rs1    = instr[19:15];
    assign rs2    = instr[24:20]; assign funct7 = instr[31:25];

    logic [31:0] imm_i, imm_s, imm_b;
    assign imm_i = {{20{instr[31]}}, instr[31:20]};
    assign imm_s = {{20{instr[31]}}, instr[31:25], instr[11:7]};
    assign imm_b = {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0};

    // ---------------- control ----------------
    localparam OP_R = 7'b0110011, OP_I = 7'b0010011, OP_LW = 7'b0000011,
               OP_SW = 7'b0100011, OP_BR = 7'b1100011;

    logic reg_write, mem_write, mem_to_reg, alu_src_imm, is_branch, alu_sub;
    always_comb begin
        reg_write = 0; mem_write = 0; mem_to_reg = 0; alu_src_imm = 0;
        is_branch = 0; alu_sub = 0; illegal = 0;
        case (opcode)
            OP_R:  begin
                if (funct3 == 3'b000 && funct7 == 7'b0000000)      reg_write = 1;           // ADD
                else if (funct3 == 3'b000 && funct7 == 7'b0100000) begin reg_write = 1; alu_sub = 1; end // SUB
                else illegal = 1;
            end
            OP_I:  if (funct3 == 3'b000) begin reg_write = 1; alu_src_imm = 1; end           // ADDI
                   else illegal = 1;
            OP_LW: if (funct3 == 3'b010) begin reg_write = 1; alu_src_imm = 1; mem_to_reg = 1; end
                   else illegal = 1;
            OP_SW: if (funct3 == 3'b010) begin mem_write = 1; alu_src_imm = 1; end
                   else illegal = 1;
            OP_BR: if (funct3 == 3'b000) begin is_branch = 1; alu_sub = 1; end                // BEQ
                   else illegal = 1;
            default: illegal = 1;
        endcase
    end

    // ---------------- execute ----------------
    logic [31:0] rd1, rd2, alu_b, alu_y, mem_rdata, wb_data;
    assign rd1 = (rs1 == 0) ? 32'h0 : regs[rs1];
    assign rd2 = (rs2 == 0) ? 32'h0 : regs[rs2];
    assign alu_b = alu_src_imm ? (opcode == OP_SW ? imm_s : imm_i) : rd2;
    assign alu_y = alu_sub ? (rd1 - alu_b) : (rd1 + alu_b);

    logic zero, take_branch;
    assign zero        = (alu_y == 32'h0);
    assign take_branch = is_branch & zero;
    assign pc_next     = take_branch ? (pc + imm_b) : (pc + 32'd4);

    // ---------------- memory & write-back ----------------
    // For LW/SW the ALU adds base + offset, so alu_y is the byte address.
    logic [31:0] dmem_addr;
    assign dmem_addr = alu_y;
    assign mem_rdata = dmem[dmem_addr[31:2]];
    assign wb_data   = mem_to_reg ? mem_rdata : alu_y;

    always_ff @(posedge clk) begin
        if (rst) begin
            pc <= 32'h0;
            for (int i = 0; i < 32; i++) regs[i] <= 32'h0;
        end else begin
            pc <= pc_next;
            if (reg_write && rd != 0) regs[rd] <= wb_data;
            if (mem_write)            dmem[dmem_addr[31:2]] <= rd2;
        end
    end
endmodule
