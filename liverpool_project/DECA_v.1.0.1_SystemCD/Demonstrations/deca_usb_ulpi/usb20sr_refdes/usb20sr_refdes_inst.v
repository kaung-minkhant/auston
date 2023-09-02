	usb20sr_refdes u0 (
		.altpll_0_areset_conduit_export      (<connected-to-altpll_0_areset_conduit_export>),      //      altpll_0_areset_conduit.export
		.altpll_0_locked_conduit_export      (<connected-to-altpll_0_locked_conduit_export>),      //      altpll_0_locked_conduit.export
		.altpll_0_phasedone_conduit_export   (<connected-to-altpll_0_phasedone_conduit_export>),   //   altpll_0_phasedone_conduit.export
		.osc_clk_clk                         (<connected-to-osc_clk_clk>),                         //                      osc_clk.clk
		.reset_reset_n                       (<connected-to-reset_reset_n>),                       //                        reset.reset_n
		.rst_pio_external_connection_export  (<connected-to-rst_pio_external_connection_export>),  //  rst_pio_external_connection.export
		.u20_clk_out_clk                     (<connected-to-u20_clk_out_clk>),                     //                  u20_clk_out.clk
		.usb20sr_conduit_end_Data            (<connected-to-usb20sr_conduit_end_Data>),            //          usb20sr_conduit_end.Data
		.usb20sr_conduit_end_Stp             (<connected-to-usb20sr_conduit_end_Stp>),             //                             .Stp
		.usb20sr_conduit_end_Dir             (<connected-to-usb20sr_conduit_end_Dir>),             //                             .Dir
		.usb20sr_conduit_end_Nxt             (<connected-to-usb20sr_conduit_end_Nxt>),             //                             .Nxt
		.usb20sr_conduit_end_phy_clk         (<connected-to-usb20sr_conduit_end_phy_clk>),         //                             .phy_clk
		.usb20sr_conduit_end_phy_reset_n     (<connected-to-usb20sr_conduit_end_phy_reset_n>),     //                             .phy_reset_n
		.usb20sr_conduit_end_phy_cs_n        (<connected-to-usb20sr_conduit_end_phy_cs_n>),        //                             .phy_cs_n
		.usb20sr_conduit_end_Ext_reset_in    (<connected-to-usb20sr_conduit_end_Ext_reset_in>),    //                             .Ext_reset_in
		.user_led_external_connection_export (<connected-to-user_led_external_connection_export>)  // user_led_external_connection.export
	);

