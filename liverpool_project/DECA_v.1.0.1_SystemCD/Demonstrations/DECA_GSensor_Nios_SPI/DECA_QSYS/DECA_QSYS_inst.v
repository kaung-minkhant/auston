	DECA_QSYS u0 (
		.clk_clk                            (<connected-to-clk_clk>),                            //                         clk.clk
		.pio_key_external_connection_export (<connected-to-pio_key_external_connection_export>), // pio_key_external_connection.export
		.pio_led_external_connection_export (<connected-to-pio_led_external_connection_export>), // pio_led_external_connection.export
		.pio_sw_external_connection_export  (<connected-to-pio_sw_external_connection_export>),  //  pio_sw_external_connection.export
		.reset_reset_n                      (<connected-to-reset_reset_n>),                      //                       reset.reset_n
		.spi_gsensor_external_MISO          (<connected-to-spi_gsensor_external_MISO>),          //        spi_gsensor_external.MISO
		.spi_gsensor_external_MOSI          (<connected-to-spi_gsensor_external_MOSI>),          //                            .MOSI
		.spi_gsensor_external_SCLK          (<connected-to-spi_gsensor_external_SCLK>),          //                            .SCLK
		.spi_gsensor_external_SS_n          (<connected-to-spi_gsensor_external_SS_n>)           //                            .SS_n
	);

