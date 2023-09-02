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

`define ENABLE_DDR3

module DECA_socket_server(

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
//=======================================================
wire  mdio_oen;
wire  mdio_out;

//=======================================================
//  Structural coding
//=======================================================

// assign outputs
assign NET_MDIO = (mdio_oen) ? 1'bz : mdio_out;		// modified as per GBI 
assign NET_PCF_EN = 1'b0;


DECA_Qsys u0 (
        .clk_clk                                       (MAX10_CLK1_50),             // clk.clk
		  .reset_reset_n                                 (1'b1),                      // reset.reset_n
			
        .dipsw_pio_export                              (SW),                        // dipsw_pio.export
        .led_pio_export                                (LED),                       // led_pio.export
		  .nenet_reg_reset_export                        (NET_RESET_n),               // nenet_reg_reset.export 
		  .user_pio_pushbtn_export                       (),                          // user_pio_pushbtn.export 
		  
		  .pll_areset_conduit_export                     (1'b0),                      // pll_areset_conduit.export
        .pll_locked_conduit_export                     (),                          // pll_locked_conduit.export
        .pll_phasedone_conduit_export                  (),                          // pll_phasedone_conduit.export
		  
		  .memory_mem_a                                  (DDR3_A),                    // memory.mem_a
        .memory_mem_ba                                 (DDR3_BA),                   //.mem_ba
        .memory_mem_ck                                 (DDR3_CK_p),                 //.mem_ck
        .memory_mem_ck_n                               (DDR3_CK_n),                 //.mem_ck_n
        .memory_mem_cke                                (DDR3_CKE),                  //.mem_cke
        .memory_mem_cs_n                               (DDR3_CS_n),                 //.mem_cs_n
        .memory_mem_dm                                 (DDR3_DM),                   //.mem_dm
        .memory_mem_ras_n                              (DDR3_RAS_n),                //.mem_ras_n
        .memory_mem_cas_n                              (DDR3_CAS_n),                //.mem_cas_n
        .memory_mem_we_n                               (DDR3_WE_n),                 //.mem_we_n
        .memory_mem_reset_n                            (DDR3_RESET_n),              //.mem_reset_n
        .memory_mem_dq                                 (DDR3_DQ),                   //.mem_dq
        .memory_mem_dqs                                (DDR3_DQS_p),                //.mem_dqs
        .memory_mem_dqs_n                              (DDR3_DQS_n),                //.mem_dqs_n
        .memory_mem_odt                                (DDR3_ODT),                  //.mem_odt
		  .mem_if_ddr3_emif_pll_ref_clk_clk              (DDR3_CLK_50),               // mem_if_ddr3_emif_pll_ref_clk.clk
        .mem_if_ddr3_emif_status_local_init_done       (ddr3_local_init_done),      // mem_if_ddr3_emif_status.local_init_done
        .mem_if_ddr3_emif_status_local_cal_success     (ddr3_local_cal_success),    //.local_cal_success
        .mem_if_ddr3_emif_status_local_cal_fail        (ddr3_local_cal_fail),       //.local_cal_fail
        .ddr3_status_external_connection_export        ({ddr3_local_cal_success, ddr3_local_cal_fail, ddr3_local_init_done}), // ddr3_status_external_connection.export

        .tse_mac_pcs_mac_rx_clock_connection_clk       (NET_RX_CLK),                // tse_mac_pcs_mac_rx_clock_connection.clk
        .tse_mac_pcs_mac_tx_clock_connection_clk       (NET_TX_CLK),                // tse_mac_pcs_mac_tx_clock_connection.clk
        .tse_mac_mac_mdio_connection_mdc               (NET_MDC),             	   // tse_mac_mac_mdio_connection.mdc
        .tse_mac_mac_mdio_connection_mdio_in           (NET_MDIO),          		   //.mdio_in
        .tse_mac_mac_mdio_connection_mdio_out          (mdio_out),         		   //.mdio_out
        .tse_mac_mac_mdio_connection_mdio_oen          (mdio_oen),         		   //.mdio_oen
        .tse_mac_mac_mii_connection_mii_rx_d           (NET_RXD),          		   // tse_mac_mac_mii_connection.mii_rx_d
        .tse_mac_mac_mii_connection_mii_rx_dv          (NET_RX_DV),        		   //.mii_rx_dv
        .tse_mac_mac_mii_connection_mii_rx_err         (NET_RX_ER),        		   //.mii_rx_err
        .tse_mac_mac_mii_connection_mii_tx_d           (NET_TXD),          			//.mii_tx_d
        .tse_mac_mac_mii_connection_mii_tx_en          (NET_TX_EN),        			//.mii_tx_en
        .tse_mac_mac_mii_connection_mii_tx_err         (),         			         //.mii_tx_err
        .tse_mac_mac_mii_connection_mii_crs            (NET_CRS),           			//.mii_crs
        .tse_mac_mac_mii_connection_mii_col            (NET_COL),            		   //.mii_col
		  .tse_mac_mac_misc_connection_ff_tx_crc_fwd     (1'b0),                      // tse_mac_mac_misc_connection.ff_tx_crc_fwd
        .tse_mac_mac_status_connection_set_10          (1'b0),                      // tse_mac_mac_status_connection.set_10
        .tse_mac_mac_status_connection_set_1000        (1'b0),                      //.set_1000
        .tse_mac_mac_status_connection_eth_mode        (),                          //.eth_mode
        .tse_mac_mac_status_connection_ena_10          ()                           //.ena_10
    );

endmodule
