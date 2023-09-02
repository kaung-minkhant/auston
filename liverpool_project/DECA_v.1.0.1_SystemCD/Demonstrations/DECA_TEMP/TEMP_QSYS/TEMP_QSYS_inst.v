	TEMP_QSYS u0 (
		.altpll_areset_conduit_export       (<connected-to-altpll_areset_conduit_export>),       //       altpll_areset_conduit.export
		.altpll_locked_conduit_export       (<connected-to-altpll_locked_conduit_export>),       //       altpll_locked_conduit.export
		.altpll_phasedone_conduit_export    (<connected-to-altpll_phasedone_conduit_export>),    //    altpll_phasedone_conduit.export
		.clk_50                             (<connected-to-clk_50>),                             //               clk_50_clk_in.clk
		.reset_n                            (<connected-to-reset_n>),                            //         clk_50_clk_in_reset.reset_n
		.pio_led_external_connection_export (<connected-to-pio_led_external_connection_export>), // pio_led_external_connection.export
		.temp_controller_conduit_end_cs_n   (<connected-to-temp_controller_conduit_end_cs_n>),   // temp_controller_conduit_end.cs_n
		.temp_controller_conduit_end_sc     (<connected-to-temp_controller_conduit_end_sc>),     //                            .sc
		.temp_controller_conduit_end_sio    (<connected-to-temp_controller_conduit_end_sio>)     //                            .sio
	);

