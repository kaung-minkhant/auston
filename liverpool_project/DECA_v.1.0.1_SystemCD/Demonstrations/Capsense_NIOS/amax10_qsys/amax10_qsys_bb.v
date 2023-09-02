
module amax10_qsys (
	clk_clk,
	i2c_opencores_export_scl_pad_io,
	i2c_opencores_export_sda_pad_io,
	led_external_connection_export,
	reset_reset_n);	

	input		clk_clk;
	inout		i2c_opencores_export_scl_pad_io;
	inout		i2c_opencores_export_sda_pad_io;
	output	[7:0]	led_external_connection_export;
	input		reset_reset_n;
endmodule
