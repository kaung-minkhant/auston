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

//`define ENABLE_DDR3

module DECA_golden_top(

      ///////// ADC /////////
      input              ADC_CLK_10,

      ///////// AUDIO /////////
      inout              AUDIO_BCLK,
      output             AUDIO_DIN_MFP1,
      input              AUDIO_DOUT_MFP2,
      inout              AUDIO_GPIO_MFP5,
      output             AUDIO_MCLK,
      input              AUDIO_MISO_MFP4,
      inout              AUDIO_RESET_n,
      output             AUDIO_SCLK_MFP3,
      output             AUDIO_SCL_SS_n,
      output             AUDIO_SDA_MOSI,
      output             AUDIO_SPI_SELECT,
      inout              AUDIO_WCLK,

      ///////// CAP /////////
      inout              CAP_SENSE_I2C_SCL,
      inout              CAP_SENSE_I2C_SDA,

`ifdef ENABLE_DDR3
      ///////// DDR3 /////////
      output      [14:0] DDR3_A,
      output      [2:0]  DDR3_BA,
      output             DDR3_CAS_n,
      output             DDR3_CKE,
      output             DDR3_CK_n,
      output             DDR3_CK_p,
      input              DDR3_CLK_50,
      output             DDR3_CS_n,
      output      [1:0]  DDR3_DM,
      inout       [15:0] DDR3_DQ,
      inout       [1:0]  DDR3_DQS_n,
      inout       [1:0]  DDR3_DQS_p,
      output             DDR3_ODT,
      output             DDR3_RAS_n,
      output             DDR3_RESET_n,
      output             DDR3_WE_n,
`endif /*ENABLE_DDR3*/

      ///////// FLASH /////////
      inout       [3:0]  FLASH_DATA,
      output             FLASH_DCLK,
      output             FLASH_NCSO,
      output             FLASH_RESET_n,

      ///////// GPIO0 /////////
      inout       [43:0] GPIO0_D,

      ///////// GPIO1 /////////
      inout       [22:0] GPIO1_D,

      ///////// GSENSOR /////////
      output             GSENSOR_PD,
      output             GSENSOR_ST,

      ///////// HDMI /////////
      inout              HDMI_I2C_SCL,
      inout              HDMI_I2C_SDA,
      inout       [3:0]  HDMI_I2S,
      inout              HDMI_LRCLK,
      inout              HDMI_MCLK,
      inout              HDMI_SCLK,
      output             HDMI_TX_CLK,
      output      [23:0] HDMI_TX_D,
      output             HDMI_TX_DE,
      output             HDMI_TX_HS,
      input              HDMI_TX_INT,
      output             HDMI_TX_VS,

      ///////// KEY /////////
      input       [1:0]  KEY,

      ///////// LED /////////
      output      [7:0]  LED,

      ///////// LIGHT /////////
      output             LIGHT_I2C_SCL,
      inout              LIGHT_I2C_SDA,
      inout              LIGHT_INT,

      ///////// MAX10 /////////
      input              MAX10_CLK1_50,
      input              MAX10_CLK2_50,

//      ///////// MIPI /////////
//      output             MIPI_CORE_EN,
//      output             MIPI_I2C_SCL,
//      inout              MIPI_I2C_SDA,
//      input              MIPI_LP_MC_n,
//      input              MIPI_LP_MC_p,
//      input       [3:0]  MIPI_LP_MD_n,
//      input       [3:0]  MIPI_LP_MD_p,
//      output             MIPI_MCLK,
//      input              MIPI_MC_p,
//      input       [3:0]  MIPI_MD_p,
//      output             MIPI_RESET_n,
//      output             MIPI_WP,
//
//      ///////// NET /////////
//      input              NET_COL,
//      input              NET_CRS,
//      output             NET_MDC,
//      inout              NET_MDIO,
//      output             NET_PCF_EN,
//      output             NET_RESET_n,
//      input       [3:0]  NET_RXD,
//      input              NET_RX_CLK,
//      input              NET_RX_DV,
//      input              NET_RX_ER,
//      output      [3:0]  NET_TXD,
//      input              NET_TX_CLK,
//      output             NET_TX_EN,

      ///////// PMONITOR /////////
      input              PMONITOR_ALERT,
      output             PMONITOR_I2C_SCL,
      inout              PMONITOR_I2C_SDA,

      ///////// BBB /////////
      input              BBB_PWR_BUT,

      ///////// RH /////////
      input              RH_TEMP_DRDY_n,
      output             RH_TEMP_I2C_SCL,
      inout              RH_TEMP_I2C_SDA,

      ///////// SD /////////
      output             SD_CLK,
      inout              SD_CMD,
      output             SD_CMD_DIR,
      output             SD_D0_DIR,
      inout              SD_D123_DIR,
      inout       [3:0]  SD_DAT,
      input              SD_FB_CLK,
      output             SD_SEL,

      ///////// SW /////////
      input       [1:0]  SW,

      ///////// BBB /////////
      input              BBB_SYS_RESET_n,

      ///////// TEMP /////////
      output             TEMP_CS_n,
      output             TEMP_SC,
      inout              TEMP_SIO,

      ///////// USB /////////
      input              USB_CLKIN,
      output             USB_CLKOUT,
      output             USB_CLKOUT_NOPLL,
      output             USB_CS,
      inout       [7:0]  USB_DATA,
      input              USB_DIR,
      input              USB_FAULT_n,
      input              USB_NXT,
      output             USB_RESET_n,
      output             USB_STP
);


//=======================================================
//  REG/WIRE declarations
//=======================================================

wire     ext_reset_pio;
wire     usb_phy_cs_n;
wire     usb_phy_reset_n;   //Active Low

wire     usb_clk_input_60M;
wire     pll_locked;

reg            delayed_reset_n;
reg    [23:0]  delay_count;

//=======================================================
//  Structural coding
//=======================================================

  
  wire           sys_reset_n = (delayed_reset_n & KEY[0] );


  always @ (posedge MAX10_CLK1_50 or negedge KEY[0])
  begin
    if(~KEY[0])
      begin
        delayed_reset_n <= 1'b0;
        delay_count     <= 24'h0;
      end
    else
      begin
        if(delay_count!=24'hFFFFFF)
          begin
            delayed_reset_n <=  1'b0;
            delay_count     <=  delay_count + 24'b1;        
          end
        else
          delayed_reset_n  <=  1'b1;
      end
  end

  

usb_pll u1(
   .areset(~KEY[0]),
   .inclk0(USB_CLKIN),
	.c0(usb_clk_input_60M),
	.locked(pll_locked)

);

//note: usb work in clcok output mode(output clock into FPGA)
    usb20sr_refdes u0 (
        .osc_clk_clk                         (MAX10_CLK1_50),                                      //                      osc_clk.clk
        .reset_reset_n                       (sys_reset_n),                                             //                        reset.reset_n
        //usb20sr
        .u20_clk_out_clk                     (usb_clk_input_60M),                                          //                  u20_clk_out.clk
        .usb20sr_conduit_end_Data            (USB_DATA),                                           //          usb20sr_conduit_end.Data
        .usb20sr_conduit_end_Stp             (USB_STP),                                            //                             .Stp
        .usb20sr_conduit_end_Dir             (USB_DIR),                                            //                             .Dir
        .usb20sr_conduit_end_Nxt             (USB_NXT),                                            //                             .Nxt
        .usb20sr_conduit_end_phy_clk         (usb_clk_input_60M),                                          //                             .phy_clk
        .usb20sr_conduit_end_phy_reset_n     (usb_phy_reset_n),                                        //                             .phy_reset_n
        .usb20sr_conduit_end_phy_cs_n        (usb_phy_cs_n),                                       //                             .phy_cs_n
        .usb20sr_conduit_end_Ext_reset_in    (ext_reset_pio),                                      //                             .Ext_reset_in
        //ext reset
		  .rst_pio_external_connection_export  (ext_reset_pio),                                      //  rst_pio_external_connection.export
        .user_led_external_connection_export (),                                                // user_led_external_connection.export
       //pll interface 
		  .altpll_0_areset_conduit_export      (1'b0),                                                   //      altpll_0_areset_conduit.export
        .altpll_0_locked_conduit_export      (),                                                   //      altpll_0_locked_conduit.export
        .altpll_0_phasedone_conduit_export   ()                                                    //   altpll_0_phasedone_conduit.export
    );

//assign USB_CS = ~usb_phy_cs_n;
assign USB_CS = 1'b1;
assign USB_RESET_n = usb_phy_reset_n | KEY[1];
assign LED[0] = ~pll_locked;
assign LED[7:1] = 7'b1111111;


endmodule
