module led_blink(
    input wire clk,
    output reg led
);

    reg [23:0] counter;
    always @(posedge clk) begin
        counter <= counter + 1;
        led <= counter[23];
    end
endmodule
