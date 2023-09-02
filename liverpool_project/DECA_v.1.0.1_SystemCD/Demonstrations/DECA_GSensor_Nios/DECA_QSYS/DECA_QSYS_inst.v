	DECA_QSYS u0 (
		.clk_clk                            (<connected-to-clk_clk>),                            //                         clk.clk
		.pio_key_external_connection_export (<connected-to-pio_key_external_connection_export>), // pio_key_external_connection.export
		.pio_led_external_connection_export (<connected-to-pio_led_external_connection_export>), // pio_led_external_connection.export
		.pio_sw_external_connection_export  (<connected-to-pio_sw_external_connection_export>),  //  pio_sw_external_connection.export
		.reset_reset_n                      (<connected-to-reset_reset_n>),                      //                       reset.reset_n
		.i2c_opencores_0_export_scl_pad_io  (<connected-to-i2c_opencores_0_export_scl_pad_io>),  //      i2c_opencores_0_export.scl_pad_io
		.i2c_opencores_0_export_sda_pad_io  (<connected-to-i2c_opencores_0_export_sda_pad_io>)   //                            .sda_pad_io
	);

