module top (
    input wire clk,
    output reg led
);

    reg [31:0] counter;
    reg [31:0] duty;

    PWM p (
        .clk(clk),
        .rst_n(1'b1),
        .duty_cycle(duty),
        .period(100),
        .pwm_out(led)
    );

    initial begin
        counter = 32'h0;
        duty <= 32'h0;
    end

    always @( posedge clk ) begin

        if ( counter < 32'd25000000/2 ) begin
            counter <= counter + 1'b1 ;
        end else begin
            counter = 0;

            if (duty < 100) begin
                duty <= duty + 1'b1;
            end else begin
                duty <= 0;
            end
        end
    end



    // reg [31:0] counter ;

    // initial begin
    //     counter = 32'h0 ;
    //     led = 1'b0 ;
    // end

    // always @( posedge clk ) begin
    //     if ( counter < 32'd25000000 * 32'd5 ) begin
    //         counter <= counter + 1'b1 ;
    //     end else begin
    //         counter <= 32'h0 ;
    //         led <= ~ led ;
    //     end
    // end
endmodule