`timescale 1ns / 1ps
module clockdiv(
    input clk,
    input reset,
    output reg tick
);
    reg [19:0] count;

    always @(posedge clk) 
    begin
        if (reset) begin
            count <= 0;
            tick <= 0;
        end 
        else if (count == 999999) begin
            count <= 0;
            tick <= 1;
        end 
        else begin
            count <= count + 1;
            tick <= 0;
        end
    end
endmodule

