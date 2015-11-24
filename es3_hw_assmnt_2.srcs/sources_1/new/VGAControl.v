`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    
// Design Name: 
// Module Name:   
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module VGAControl(
    clk,
    reset,
    colour_out,
    hs,
    vs,
    reg_0_colour,
    reg_1_colour,
    reg_2_colour,
    reg_3_colour,
    reg_4_colour,
    reg_5_colour,
    reg_6_colour,
    reg_7_colour,
    reg_8_colour,
    reg_9_colour,
    reg_10_colour,
    reg_11_colour
    );

    input clk;
    input reset;
    output [11:0] colour_out;
    output hs;
    output vs;
    input [11:0] reg_0_colour;
    input [11:0] reg_1_colour;
    input [11:0] reg_2_colour;
    input [11:0] reg_3_colour;
    input [11:0] reg_4_colour;
    input [11:0] reg_5_colour;
    input [11:0] reg_6_colour;
    input [11:0] reg_7_colour;
    input [11:0] reg_8_colour;
    input [11:0] reg_9_colour;
    input [11:0] reg_10_colour;
    input [11:0] reg_11_colour;
    
    wire reset;
            
    wire [9:0] addrh;
    wire [8:0] addrv;
    reg  [11:0] colour;

    VGAInterface VGAInterface_instance (
        .CLK(clk), 
        .C_IN(colour), 
        .C_OUT(colour_out), 
        .HS(hs), 
        .VS(vs),
        .ADR_X(addrh),
        .ADR_Y(addrv)
    );

    always @(posedge clk) begin
        if ((addrh == 160) || (addrh == 320) || (addrh == 480) || (addrv == 160)
            || (addrv == 320))
            colour <= 12'b0000_0000_0000;
        else if ((addrh <= 160) && (addrv <= 160))
            colour <= reg_0_colour;
        else if ((addrh >= 160) && (addrv <= 160) && (addrh <= 320))
            colour <= reg_3_colour;
        else if ((addrh >= 320) && (addrv <= 160) && (addrh <= 480))
            colour <= reg_6_colour;
        else if ((addrh >= 320) && (addrh <= 480) && (addrv >= 160) && (addrv <= 320))
            colour <= reg_7_colour;
        else if ((addrh >= 320) && (addrh <= 480) && (addrv >= 320) && (addrv < 480))
            colour <= reg_8_colour;
        else if ((addrh >= 160) && (addrh <= 320) && (addrv >= 320) && (addrv < 480))
            colour <= reg_5_colour;
        else if ((addrh <= 160) && (addrv >= 320) && (addrv < 480))
            colour <= reg_2_colour;
        else if ((addrh <= 160) && (addrv >= 160) && (addrv <= 320))
            colour <= reg_1_colour;
        else if ((addrh >= 160) && (addrh <= 320) && (addrv >= 160) && (addrv <= 320))
            colour <= reg_4_colour;
        else if ((addrh >= 480) && (addrv <= 160) && (addrh <= 639))
            colour <= reg_9_colour;
        else if ((addrh >= 480) && (addrh <= 639) && (addrv >= 160) && (addrv <= 320))
            colour <= reg_10_colour;
        else if ((addrh >= 480) && (addrh <= 639) && (addrv >= 320) && (addrv <= 479))
            colour <= reg_11_colour;
        else
            colour <= 12'b1111_1111_1111;
	end
  
endmodule
