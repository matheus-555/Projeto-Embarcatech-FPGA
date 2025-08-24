module spi_sd(
    input wire clk,
    input wire rst,
    output reg sd_clk,
    output reg sd_mosi,
    input  wire sd_miso,
    output reg sd_cs_n
);

    // Clock divider simples para SPI (~400 kHz se clk25 = 25 MHz)
    reg [5:0] div;
    always @(posedge clk) div <= div + 1;
    assign sd_clk = div[5];

    initial begin
        sd_cs_n = 1;
        sd_mosi = 0;
    end

    // Exemplo mínimo: não transmite dados ainda
endmodule
