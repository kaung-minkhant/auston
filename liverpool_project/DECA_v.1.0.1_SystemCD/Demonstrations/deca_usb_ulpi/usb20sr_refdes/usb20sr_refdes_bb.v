
module usb20sr_refdes (
	altpll_0_areset_conduit_export,
	altpll_0_locked_conduit_export,
	altpll_0_phasedone_conduit_export,
	osc_clk_clk,
	reset_reset_n,
	rst_pio_external_connection_export,
	u20_clk_out_clk,
	usb20sr_conduit_end_Data,
	usb20sr_conduit_end_Stp,
	usb20sr_conduit_end_Dir,
	usb20sr_conduit_end_Nxt,
	usb20sr_conduit_end_phy_clk,
	usb20sr_conduit_end_phy_reset_n,
	usb20sr_conduit_end_phy_cs_n,
	usb20sr_conduit_end_Ext_reset_in,
	user_led_external_connection_export);	

	input		altpll_0_areset_conduit_export;
	output		altpll_0_locked_conduit_export;
	output		altpll_0_phasedone_conduit_export;
	input		osc_clk_clk;
	input		reset_reset_n;
	output		rst_pio_external_connection_export;
	input		u20_clk_out_clk;
	inout	[7:0]	usb20sr_conduit_end_Data;
	output		usb20sr_conduit_end_Stp;
	input		usb20sr_conduit_end_Dir;
	input		usb20sr_conduit_end_Nxt;
	input		usb20sr_conduit_end_phy_clk;
	output		usb20sr_conduit_end_phy_reset_n;
	output		usb20sr_conduit_end_phy_cs_n;
	input		usb20sr_conduit_end_Ext_reset_in;
	output	[7:0]	user_led_external_connection_export;
endmodule
