// (C) 2001-2015 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// megafunction wizard: %ALTDDIO_IN%
// GENERATION: STANDARD
// VERSION: WM1.0
// MODULE: altddio_in 

// ============================================================
// File Name: rgmii_in4.v
// Megafunction Name(s):
// 			altddio_in
// ============================================================
// ************************************************************
// THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
//
// 6.0 Build 176 04/19/2006 SJ Full Version
// ************************************************************


//Copyright (C) 1991-2006 Altera Corporation
//Your use of Altera Corporation's design tools, logic functions 
//and other software and tools, and its AMPP partner logic 
//functions, and any output files any of the foregoing 
//(including device programming or simulation files), and any 
//associated documentation or information are expressly subject 
//to the terms and conditions of the Altera Program License 
//Subscription Agreement, Altera MegaCore Function License 
//Agreement, or other applicable license agreement, including, 
//without limitation, that your use is for the sole purpose of 
//programming logic devices manufactured by Altera and sold by 
//Altera or its authorized distributors.  Please refer to the 
//applicable agreement for further details.


// synopsys translate_off
`timescale 1 ps / 1 ps
// synopsys translate_on
module altera_tse_rgmii_in4 (
	aclr,
	datain,
	inclock,
	dataout_h,
	dataout_l);

	input	  aclr;
	input	[3:0]  datain;
	input	  inclock;
	output	[3:0]  dataout_h;
	output	[3:0]  dataout_l;

	wire [3:0] sub_wire0;
	wire [3:0] sub_wire1;
	wire [3:0] dataout_h = sub_wire0[3:0];
	wire [3:0] dataout_l = sub_wire1[3:0];

	altddio_in	altddio_in_component (
				.datain (datain),
				.inclock (inclock),
				.aclr (aclr),
				.dataout_h (sub_wire0),
				.dataout_l (sub_wire1),
				.aset (1'b0),
				.inclocken (1'b1));
	defparam
		altddio_in_component.intended_device_family = "Stratix II",
		altddio_in_component.invert_input_clocks = "OFF",
		altddio_in_component.lpm_type = "altddio_in",
		altddio_in_component.width = 4;


endmodule

// ============================================================
// CNX file retrieval info
// ============================================================
// Retrieval info: PRIVATE: ARESET_MODE NUMERIC "0"
// Retrieval info: PRIVATE: CLKEN NUMERIC "0"
// Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "Stratix II"
// Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "Stratix II"
// Retrieval info: PRIVATE: INVERT_INPUT_CLOCKS NUMERIC "0"
// Retrieval info: PRIVATE: POWER_UP_HIGH NUMERIC "0"
// Retrieval info: PRIVATE: WIDTH NUMERIC "4"
// Retrieval info: CONSTANT: INTENDED_DEVICE_FAMILY STRING "Stratix II"
// Retrieval info: CONSTANT: INVERT_INPUT_CLOCKS STRING "OFF"
// Retrieval info: CONSTANT: LPM_TYPE STRING "altddio_in"
// Retrieval info: CONSTANT: WIDTH NUMERIC "4"
// Retrieval info: USED_PORT: aclr 0 0 0 0 INPUT GND aclr
// Retrieval info: USED_PORT: datain 0 0 4 0 INPUT NODEFVAL datain[3..0]
// Retrieval info: USED_PORT: dataout_h 0 0 4 0 OUTPUT NODEFVAL dataout_h[3..0]
// Retrieval info: USED_PORT: dataout_l 0 0 4 0 OUTPUT NODEFVAL dataout_l[3..0]
// Retrieval info: USED_PORT: inclock 0 0 0 0 INPUT_CLK_EXT NODEFVAL inclock
// Retrieval info: CONNECT: @datain 0 0 4 0 datain 0 0 4 0
// Retrieval info: CONNECT: dataout_h 0 0 4 0 @dataout_h 0 0 4 0
// Retrieval info: CONNECT: dataout_l 0 0 4 0 @dataout_l 0 0 4 0
// Retrieval info: CONNECT: @inclock 0 0 0 0 inclock 0 0 0 0
// Retrieval info: CONNECT: @aclr 0 0 0 0 aclr 0 0 0 0
// Retrieval info: LIBRARY: altera_mf altera_mf.altera_mf_components.all
// Retrieval info: GEN_FILE: TYPE_NORMAL rgmii_in4.v TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL rgmii_in4.ppf TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL rgmii_in4.inc FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL rgmii_in4.cmp FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL rgmii_in4.bsf TRUE FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL rgmii_in4_inst.v FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL rgmii_in4_bb.v TRUE
