`timescale 1ns / 1ps


module cont(
 input clk,
    input reset,              
    input start_pulse,        
    input reset_pulse,        
    input [1:0] mode,         
    input [3:0] set_tens,  
    input [3:0] set_ones, 
    input start,
    input max,

    output reg enable,
    output reg load,
    output reg c_up,
    output reg [3:0] load_sec_tens,
    output reg [3:0] load_sec_ones,
    output reg [3:0] load_ms_tens,
    output reg [3:0] load_ms_ones
);

    reg [1:0] state, next_state;

    localparam 
    done    = 2'b00,
    continue = 2'b01,
    stop  = 2'b10;

    // state register
    always @(posedge clk or posedge reset) begin
        if (reset)
            state <= done;
        else
            state <= next_state;
    end

    // mode decode
    always @(*) begin
        case(mode)
            2'b00: begin
                c_up = 1'b1;          // count up
                load_sec_tens = 4'b0000;
                load_sec_ones = 4'b0000;
                load_ms_tens = 4'b0000;
                load_ms_ones = 4'b0000;
            end

            2'b01: begin
                c_up = 1'b0;          // count down
                load_sec_tens = 4'b1001;
                load_sec_ones = 4'b1001;
                load_ms_tens = 4'b1001;
                load_ms_ones = 4'b1001;
            end

            2'b10: begin
                c_up = 1'b1;          // count up
                load_sec_tens = set_tens;
                load_sec_ones = set_ones;
                load_ms_tens = 4'b0000;
                load_ms_ones = 4'b0000;
            end

            2'b11: begin
                c_up = 1'b0;          // count down
                load_sec_tens = set_tens;
                load_sec_ones = set_ones;
                load_ms_tens = 4'b0000;
                load_ms_ones = 4'b0000;
            end
        endcase
    end

    // next-state logic
    always @(*) begin
        next_state = state;

        case(state)
            done: begin
                if (start_pulse)
                    next_state = continue;
            end

            continue: begin
                if (reset_pulse)
                    next_state = done;
                else if (start_pulse)
                    next_state = stop;
                else if ((c_up == 1'b0 && start) || (c_up == 1'b1 && max))
                    next_state = stop;
            end

            stop: begin
                if (reset_pulse)
                    next_state = done;
                else if (start_pulse)
                    next_state = continue;
            end
        endcase
    end

    // output logic
    always @(*) begin
        enable = (state == continue);
        load   = reset_pulse;
    end

endmodule
