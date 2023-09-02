	amax10_qsys u0 (
		.button_external_connection_export    (<connected-to-button_external_connection_export>),    //    button_external_connection.export
		.clk_clk                              (<connected-to-clk_clk>),                              //                           clk.clk
		.i2c_opencores_export_scl_pad_io      (<connected-to-i2c_opencores_export_scl_pad_io>),      //          i2c_opencores_export.scl_pad_io
		.i2c_opencores_export_sda_pad_io      (<connected-to-i2c_opencores_export_sda_pad_io>),      //                              .sda_pad_io
		.interrupt_external_connection_export (<connected-to-interrupt_external_connection_export>), // interrupt_external_connection.export
		.led_external_connection_export       (<connected-to-led_external_connection_export>),       //       led_external_connection.export
		.reset_reset_n                        (<connected-to-reset_reset_n>)                         //                         reset.reset_n
	);

