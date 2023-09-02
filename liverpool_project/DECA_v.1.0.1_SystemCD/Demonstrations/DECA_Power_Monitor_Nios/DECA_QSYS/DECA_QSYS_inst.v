	DECA_QSYS u0 (
		.clk_clk                                     (<connected-to-clk_clk>),                                     //                                  clk.clk
		.pio_key_external_connection_export          (<connected-to-pio_key_external_connection_export>),          //          pio_key_external_connection.export
		.pio_led_external_connection_export          (<connected-to-pio_led_external_connection_export>),          //          pio_led_external_connection.export
		.pio_sw_external_connection_export           (<connected-to-pio_sw_external_connection_export>),           //           pio_sw_external_connection.export
		.pmonitor_alert_external_connection_export   (<connected-to-pmonitor_alert_external_connection_export>),   //   pmonitor_alert_external_connection.export
		.pmonitor_i2c_scl_external_connection_export (<connected-to-pmonitor_i2c_scl_external_connection_export>), // pmonitor_i2c_scl_external_connection.export
		.pmonitor_i2c_sda_external_connection_export (<connected-to-pmonitor_i2c_sda_external_connection_export>), // pmonitor_i2c_sda_external_connection.export
		.reset_reset_n                               (<connected-to-reset_reset_n>)                                //                                reset.reset_n
	);

