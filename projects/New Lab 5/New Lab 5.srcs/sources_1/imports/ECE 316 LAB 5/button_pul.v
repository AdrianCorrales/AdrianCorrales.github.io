`timescale 1ns / 1ps


module button_pul(
    input clk,
    input btn,
    output pulse
);
    reg action;

    always @(posedge clk) begin
        action <= btn;
    end

    assign pulse = btn & ~action;
endmodule