	adc_qsys u0 (
		.altpll_adc_locked_conduit_export    (<connected-to-altpll_adc_locked_conduit_export>),    //    altpll_adc_locked_conduit.export
		.clk_clk                             (<connected-to-clk_clk>),                             //                          clk.clk
		.clk_adc_10_clk                      (<connected-to-clk_adc_10_clk>),                      //                   clk_adc_10.clk
		.clk_adc_10_reset_n_reset_n          (<connected-to-clk_adc_10_reset_n_reset_n>),          //           clk_adc_10_reset_n.reset_n
		.modular_adc_0_adc_pll_locked_export (<connected-to-modular_adc_0_adc_pll_locked_export>), // modular_adc_0_adc_pll_locked.export
		.modular_adc_1_adc_pll_locked_export (<connected-to-modular_adc_1_adc_pll_locked_export>), // modular_adc_1_adc_pll_locked.export
		.reset_reset_n                       (<connected-to-reset_reset_n>)                        //                        reset.reset_n
	);

