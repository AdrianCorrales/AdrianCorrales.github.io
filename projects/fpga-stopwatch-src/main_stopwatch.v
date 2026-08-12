`timescale 1ns / 1ps

module main_stopwatch(
    input clk,
    input btn_start,
    input btn_reset,
    input [9:0] sw,
    output reg [3:0] an,
    output [6:0] seg,
    output reg dp
);

    // internal wires
    wire tick;
    wire start_pulse, reset_pulse;
    wire enable, load, c_up;
    wire [3:0] load_sec_tens, load_sec_ones;
    wire [3:0] load_ms_tens, load_ms_ones;
    wire [3:0] sec_tens, sec_ones, ms_tens, ms_ones;
    wire start, max;

    // display refresh
    reg [15:0] refresh_counter;
    wire [1:0] digit;
    reg [3:0] current;

    assign digit = refresh_counter[15:14];

    // clock divider for 10 ms tick
    clockdiv u_clkdiv(
        .clk(clk),
        .reset(btn_reset),
        .tick(tick)
    );

    // button pulse generators
    button_pul u_start(
        .clk(clk),
        .btn(btn_start),
        .pulse(start_pulse)
    );

    button_pul u_reset(
        .clk(clk),
        .btn(btn_reset),
        .pulse(reset_pulse)
    );

    // controller
    cont u_ctrl(
        .clk(clk),
        .reset(1'b0),
        .start_pulse(start_pulse),
        .reset_pulse(reset_pulse),
        .mode(sw[9:8]),
        .set_tens(sw[7:4]),
        .set_ones(sw[3:0]),
        .start(start),
        .max(max),
        .enable(enable),
        .load(load),
        .c_up(c_up),
        .load_sec_tens(load_sec_tens),
        .load_sec_ones(load_sec_ones),
        .load_ms_tens(load_ms_tens),
        .load_ms_ones(load_ms_ones)
    );

    // time counter
    time_counter u_counter(
        .clk(clk),
        .tick(tick),
        .reset(1'b0),
        .enable(enable),
        .c_up(c_up),
        .load(load),
        .sec_tens_in(load_sec_tens),
        .sec_ones_in(load_sec_ones),
        .ms_tens_in(load_ms_tens),
        .ms_ones_in(load_ms_ones),
        .sec_tens(sec_tens),
        .sec_ones(sec_ones),
        .ms_tens(ms_tens),
        .ms_ones(ms_ones),
        .start(start),
        .max(max)
    );

    // display refresh counter
    always @(posedge clk) begin
        refresh_counter <= refresh_counter + 1;
    end

    // digit select logic
    always @(*) begin
        case(digit)
            2'b00: begin
                an = 4'b1110;
                current = ms_ones;
                dp = 1'b1;
            end

            2'b01: begin
                an = 4'b1101;
                current = ms_tens;
                dp = 1'b1;   // decimal point here
            end

            2'b10: begin
                an = 4'b1011;
                current = sec_ones;
                dp = 1'b0;
            end

            2'b11: begin
                an = 4'b0111;
                current = sec_tens;
                dp = 1'b1;
            end
        endcase
    end

    // 7-segment decoder
    val_to_7seg u_seg(
        .val(current),
        .seg(seg)
    );

endmodule