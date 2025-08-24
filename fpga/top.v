module top(
    input wire clk25,
    input wire rst_n,
    output wire user_led,
    
    // SD Card SPI
    output wire sd_clk,
    output wire sd_mosi,
    input  wire sd_miso,
    output wire sd_cs_n
);

    wire cpu_reset;
    assign cpu_reset = ~rst_n;

    // Instancia SoC
    soc u_soc (
        .clk25(clk25),
        .rst(cpu_reset),
        .user_led(user_led),
        .sd_clk(sd_clk),
        .sd_mosi(sd_mosi),
        .sd_miso(sd_miso),
        .sd_cs_n(sd_cs_n)
    );

endmodule
