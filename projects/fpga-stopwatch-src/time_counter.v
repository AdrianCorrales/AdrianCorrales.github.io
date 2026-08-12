`timescale 1ns / 1ps


module time_counter(
    input clk,
    input tick,
    input reset,
    input enable,
    input c_up, 
    input load,
    input [3:0] sec_tens_in,
    input [3:0] sec_ones_in,
    input [3:0] ms_tens_in,
    input [3:0] ms_ones_in,
    output reg [3:0] sec_tens,
    output reg [3:0] sec_ones,
    output reg [3:0] ms_tens,
    output reg [3:0] ms_ones,
    output start,
    output max 
);
assign start = (sec_tens == 0 && sec_ones == 0 && ms_tens == 0 && ms_ones == 0);
assign max  = (sec_tens == 9 && sec_ones == 9 && ms_tens == 9 && ms_ones == 9);

always @(posedge clk) 
begin
    if (reset) begin
        sec_tens <= 0;
        sec_ones <= 0;
        ms_tens <= 0;
        ms_ones <= 0;
    end 
    else if (load) begin
        sec_tens <= sec_tens_in;
        sec_ones <= sec_ones_in;
        ms_tens  <= ms_tens_in;
        ms_ones  <= ms_ones_in;
    end 
    else if (tick && enable) begin
        if (c_up) begin 
            if (sec_tens == 9 && sec_ones == 9 && ms_tens == 9 && ms_ones == 9) begin
            end
            else if (ms_ones == 9) begin
                ms_ones <= 0;
                if (ms_tens == 9) begin
                    ms_tens <= 0;
                    if (sec_ones == 9) begin
                        sec_ones <= 0;
                        sec_tens <= sec_tens + 1;
                    end 
                    else sec_ones <= sec_ones + 1;
                end 
                else ms_tens <= ms_tens + 1;
            end 
            else ms_ones <= ms_ones + 1;
        end 
        else begin 
            if (sec_tens == 0 && sec_ones == 0 && ms_tens == 0 && ms_ones == 0) begin
            end
            else if (ms_ones == 0) begin
                ms_ones <= 9;
                if (ms_tens == 0) begin
                    ms_tens <= 9;
                    if (sec_ones == 0) begin
                        sec_ones <= 9;
                        sec_tens <= sec_tens - 1;
                    end 
                    else sec_ones <= sec_ones - 1;
                end 
                else ms_tens <= ms_tens - 1;
            end 
            else ms_ones <= ms_ones - 1;
        end
    end
end

endmodule