
module DECA_QSYS (
	clk_clk,
	pio_key_external_connection_export,
	pio_led_external_connection_export,
	pio_sw_external_connection_export,
	reset_reset_n,
	spi_gsensor_external_MISO,
	spi_gsensor_external_MOSI,
	spi_gsensor_external_SCLK,
	spi_gsensor_external_SS_n);	

	input		clk_clk;
	input	[1:0]	pio_key_external_connection_export;
	output	[7:0]	pio_led_external_connection_export;
	input	[1:0]	pio_sw_external_connection_export;
	input		reset_reset_n;
	input		spi_gsensor_external_MISO;
	output		spi_gsensor_external_MOSI;
	output		spi_gsensor_external_SCLK;
	output		spi_gsensor_external_SS_n;
endmodule
