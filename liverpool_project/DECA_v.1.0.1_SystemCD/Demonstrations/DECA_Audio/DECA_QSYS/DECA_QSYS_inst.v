	DECA_QSYS u0 (
		.audio_i2c_export_scl_pad_io              (<connected-to-audio_i2c_export_scl_pad_io>),              //                  audio_i2c_export.scl_pad_io
		.audio_i2c_export_sda_pad_io              (<connected-to-audio_i2c_export_sda_pad_io>),              //                                  .sda_pad_io
		.audio_reset_n_external_connection_export (<connected-to-audio_reset_n_external_connection_export>), // audio_reset_n_external_connection.export
		.clk_clk                                  (<connected-to-clk_clk>),                                  //                               clk.clk
		.pio_key_external_connection_export       (<connected-to-pio_key_external_connection_export>),       //       pio_key_external_connection.export
		.pio_led_external_connection_export       (<connected-to-pio_led_external_connection_export>),       //       pio_led_external_connection.export
		.pio_sw_external_connection_export        (<connected-to-pio_sw_external_connection_export>),        //        pio_sw_external_connection.export
		.pll_audio_clk                            (<connected-to-pll_audio_clk>),                            //                         pll_audio.clk
		.reset_reset_n                            (<connected-to-reset_reset_n>)                             //                             reset.reset_n
	);

