
module DECA_QSYS (
	clk_clk,
	pio_key_external_connection_export,
	pio_led_external_connection_export,
	pio_sw_external_connection_export,
	reset_reset_n,
	i2c_opencores_0_export_scl_pad_io,
	i2c_opencores_0_export_sda_pad_io);	

	input		clk_clk;
	input	[1:0]	pio_key_external_connection_export;
	output	[7:0]	pio_led_external_connection_export;
	input	[1:0]	pio_sw_external_connection_export;
	input		reset_reset_n;
	inout		i2c_opencores_0_export_scl_pad_io;
	inout		i2c_opencores_0_export_sda_pad_io;
endmodule
