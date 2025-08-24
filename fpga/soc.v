module soc(
    input wire clk25,
    input wire rst,
    output wire user_led,
    output wire sd_clk,
    output wire sd_mosi,
    input  wire sd_miso,
    output wire sd_cs_n
);

    // --- CPU (VexRiscv Lite) ---
    wire [31:0] instr;
    wire [31:0] data_wr;
    wire [31:0] data_rd;
    wire [31:0] addr;
    wire        mem_wr;
    wire        mem_rd;

    VexRiscv cpu (
        .clk(clk25),
        .reset(rst),
        .instr(instr),
        .data_wr(data_wr),
        .data_rd(data_rd),
        .addr(addr),
        .mem_wr(mem_wr),
        .mem_rd(mem_rd)
    );

    // --- RAM simples 4 KB ---
    reg [31:0] ram [0:1023];
    assign instr = ram[addr[11:2]];   // acesso instrucao
    always @(posedge clk25) begin
        if(mem_wr) ram[addr[11:2]] <= data_wr;
    end
    assign data_rd = ram[addr[11:2]];

    // --- SPI Master ---
    spi_sd spi0 (
        .clk(clk25),
        .rst(rst),
        .sd_clk(sd_clk),
        .sd_mosi(sd_mosi),
        .sd_miso(sd_miso),
        .sd_cs_n(sd_cs_n)
    );

    // --- LED simples (heartbeat) ---
    reg [23:0] counter;
    always @(posedge clk25) begin
        if(rst) counter <= 0;
        else counter <= counter + 1;
    end
    assign user_led = counter[23];

endmodule
