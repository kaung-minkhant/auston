// ============================================================================
// Copyright (c) 2014 by Terasic Technologies Inc.
// ============================================================================
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// ============================================================================
//           
//  Terasic Technologies Inc
//  9F., No.176, Sec.2, Gongdao 5th Rd, East Dist, Hsinchu City, 30070. Taiwan
//  
//  
//                     web: http://www.terasic.com/  
//                     email: support@terasic.com
//
// ============================================================================
//Date:  Fri Oct 24 14:09:29 2014
// ============================================================================

//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

//`define ENABLE_DDR3

module DECA(

	//////////// CLOCK //////////
	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	//////////// KEY //////////
	input 		     [1:0]		KEY,

	//////////// LED //////////
	output		     [7:0]		LED,

	//////////// CapSense Button //////////
	inout 		          		CAP_SENSE_I2C_SCL,
	inout 		          		CAP_SENSE_I2C_SDA,

	//////////// Audio //////////
	inout 		          		AUDIO_BCLK,
	output		          		AUDIO_DIN_MFP1,
	input 		          		AUDIO_DOUT_MFP2,
	inout 		          		AUDIO_GPIO_MFP5,
	output		          		AUDIO_MCLK,
	input 		          		AUDIO_MISO_MFP4,
	inout 		          		AUDIO_RESET_n,
	output		          		AUDIO_SCL_SS_n,
	output		          		AUDIO_SCLK_MFP3,
	inout 		          		AUDIO_SDA_MOSI,
	output		          		AUDIO_SPI_SELECT,
	inout 		          		AUDIO_WCLK,

`ifdef ENABLE_DDR3	
	//////////// SDRAM //////////
	output		    [14:0]		DDR3_A,
	output		     [2:0]		DDR3_BA,
	output		          		DDR3_CAS_n,
	inout 		          		DDR3_CK_n,
	inout 		          		DDR3_CK_p,
	output		          		DDR3_CKE,
	input 		          		DDR3_CLK_50,
	output		          		DDR3_CS_n,
	output		     [1:0]		DDR3_DM,
	inout 		    [15:0]		DDR3_DQ,
	inout 		     [1:0]		DDR3_DQS_n,
	inout 		     [1:0]		DDR3_DQS_p,
	output		          		DDR3_ODT,
	output		          		DDR3_RAS_n,
	output		          		DDR3_RESET_n,
	output		          		DDR3_WE_n,
	
`endif /*ENABLE_DDR3*/	

	//////////// Flash //////////
	inout 		     [3:0]		FLASH_DATA,
	output		          		FLASH_DCLK,
	output		          		FLASH_NCSO,
	output		          		FLASH_RESET_n,

	//////////// G-Sensor //////////
	output		          		G_SENSOR_CS_n,
	input 		          		G_SENSOR_INT1,
	input 		          		G_SENSOR_INT2,
	inout 		          		G_SENSOR_SCLK,
	inout 		          		G_SENSOR_SDI,
	inout 		          		G_SENSOR_SDO,

	//////////// HDMI-TX //////////
	inout 		          		HDMI_I2C_SCL,
	inout 		          		HDMI_I2C_SDA,
	inout 		     [3:0]		HDMI_I2S,
	inout 		          		HDMI_LRCLK,
	inout 		          		HDMI_MCLK,
	inout 		          		HDMI_SCLK,
	output		          		HDMI_TX_CLK,
	output		    [23:0]		HDMI_TX_D,
	output		          		HDMI_TX_DE,
	output		          		HDMI_TX_HS,
	input 		          		HDMI_TX_INT,
	output		          		HDMI_TX_VS,

	//////////// Light Sensor //////////
	output		          		LIGHT_I2C_SCL,
	inout 		          		LIGHT_I2C_SDA,
	inout 		          		LIGHT_INT,

	//////////// MIPI //////////
	output		          		MIPI_CORE_EN,
	output		          		MIPI_I2C_SCL,
	inout 		          		MIPI_I2C_SDA,
	input 		          		MIPI_LP_MC_n,
	input 		          		MIPI_LP_MC_p,
	input 		     [3:0]		MIPI_LP_MD_n,
	input 		     [3:0]		MIPI_LP_MD_p,
	input 		          		MIPI_MC_p,
	output		          		MIPI_MCLK,
	input 		     [3:0]		MIPI_MD_p,
	output		          		MIPI_RESET_n,
	output		          		MIPI_WP,

	//////////// Ethernet //////////
	input 		          		NET_COL,
	input 		          		NET_CRS,
	output		          		NET_MDC,
	inout 		          		NET_MDIO,
	output		          		NET_PCF_EN,
	output		          		NET_RESET_n,
	input 		          		NET_RX_CLK,
	input 		          		NET_RX_DV,
	input 		          		NET_RX_ER,
	input 		     [3:0]		NET_RXD,
	input 		          		NET_TX_CLK,
	output		          		NET_TX_EN,
	output		     [3:0]		NET_TXD,

	//////////// Power Monitor //////////
	input 		          		PMONITOR_ALERT,
	output		          		PMONITOR_I2C_SCL,
	inout 		          		PMONITOR_I2C_SDA,

	//////////// Humidity and Temperature Sensor //////////
	input 		          		RH_TEMP_DRDY_n,
	output		          		RH_TEMP_I2C_SCL,
	inout 		          		RH_TEMP_I2C_SDA,

	//////////// MicroSD Card //////////
	output		          		SD_CLK,
	inout 		          		SD_CMD,
	output		          		SD_CMD_DIR,
	output		          		SD_D0_DIR,
	inout 		          		SD_D123_DIR,
	inout 		     [3:0]		SD_DAT,
	input 		          		SD_FB_CLK,
	output		          		SD_SEL,

	//////////// SW //////////
	input 		     [1:0]		SW,

	//////////// Board Temperature Sensor //////////
	output		          		TEMP_CS_n,
	output		          		TEMP_SC,
	inout 		          		TEMP_SIO,

	//////////// USB //////////
	input 		          		USB_CLKIN,
	output		          		USB_CS,
	inout 		     [7:0]		USB_DATA,
	input 		          		USB_DIR,
	input 		          		USB_FAULT_n,
	input 		          		USB_NXT,
	output		          		USB_RESET_n,
	output		          		USB_STP,

	//////////// BBB Conector //////////
	input 		          		BBB_PWR_BUT,
	input 		          		BBB_SYS_RESET_n,
	inout 		    [43:0]		GPIO0_D,
	inout 		    [22:0]		GPIO1_D
);



//=======================================================
//  REG/WIRE declarations
//=======================================================

wire reset_n;
wire sys_clk;


//=======================================================
//  Structural coding
//=======================================================

assign reset_n = 1'b1;



    adc_qsys u0 (
        .clk_clk                              (MAX10_CLK1_50),                              //                    clk.clk
        .reset_reset_n                        (reset_n),                        //                  reset.reset_n
        .modular_adc_0_command_valid          (command_valid),          //  modular_adc_0_command.valid
        .modular_adc_0_command_channel        (command_channel),        //                       .channel
        .modular_adc_0_command_startofpacket  (command_startofpacket),  //                       .startofpacket
        .modular_adc_0_command_endofpacket    (command_endofpacket),    //                       .endofpacket
        .modular_adc_0_command_ready          (command_ready),          //                       .ready
        .modular_adc_0_response_valid         (response_valid),         // modular_adc_0_response.valid
        .modular_adc_0_response_channel       (response_channel),       //                       .channel
        .modular_adc_0_response_data          (response_data),          //                       .data
        .modular_adc_0_response_startofpacket (response_startofpacket), //                       .startofpacket
        .modular_adc_0_response_endofpacket   (response_endofpacket),    //                       .endofpacket
        .clock_bridge_sys_out_clk_clk         (sys_clk)          // clock_bridge_sys_out_clk.clk
    );

	 
////////////////////////////////////////////
// command
wire  command_valid;
wire  [4:0] command_channel;
wire  command_startofpacket;
wire  command_endofpacket;
wire command_ready;

// continused send command
assign command_startofpacket = 1'b1; // // ignore in altera_adc_control core
assign command_endofpacket = 1'b1; // ignore in altera_adc_control core
assign command_valid = 1'b1; // 
//assign command_channel = 5; // ADC2IN5 for M50
//assign command_channel = 13; // ADC1IN13 for M08
assign command_channel = 5; // ADC1IN13 for M08 ??? work on 5 ????

////////////////////////////////////////////
// response
wire response_valid;
wire [4:0] response_channel;
wire [11:0] response_data;
wire response_startofpacket;
wire response_endofpacket;
reg [4:0]  cur_adc_ch;
reg [11:0] adc_sample_data;
reg [10:0] pcm_abs;


always @ (posedge sys_clk)
begin
	if (response_valid)
	begin
		adc_sample_data <= response_data;
		cur_adc_ch <= response_channel;
	end
end			

// adc_sample_data: hold 12-bit adc sample value
// Vout = Vin + 1.25V?	 
	 
////////////////////////////////////////////
// response	 
`define PCM_ZERO	12'h800

always @ (posedge sys_clk)
begin
	if (adc_sample_data >= `PCM_ZERO)
		pcm_abs <= adc_sample_data - `PCM_ZERO;
	else if (adc_sample_data == 0)
		pcm_abs <= `PCM_ZERO - 1;
	else
		pcm_abs <= `PCM_ZERO - adc_sample_data;
end	

reg [7:0] vol;

always @ (posedge sys_clk)
begin
	case(pcm_abs[10:8])
	 3'b000:  vol = 8'b00000001;
	 3'b001:  vol = 8'b00000011;
	 3'b010:  vol = 8'b00000111;
	 3'b011:  vol = 8'b00001111;
	 3'b100:  vol = 8'b00011111;
	 3'b101:  vol = 8'b00111111;
	 3'b110:  vol = 8'b01111111;
	 3'b111:  vol = 8'b11111111;
	endcase
end

assign LED[7:0] = ~vol[7:0];  // led is low active





endmodule
