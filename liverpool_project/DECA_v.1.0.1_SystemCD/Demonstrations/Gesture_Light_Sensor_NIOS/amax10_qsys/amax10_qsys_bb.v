
module amax10_qsys (
	button_external_connection_export,
	clk_clk,
	i2c_opencores_export_scl_pad_io,
	i2c_opencores_export_sda_pad_io,
	interrupt_external_connection_export,
	led_external_connection_export,
	reset_reset_n);	

	input		button_external_connection_export;
	input		clk_clk;
	inout		i2c_opencores_export_scl_pad_io;
	inout		i2c_opencores_export_sda_pad_io;
	input		interrupt_external_connection_export;
	output	[7:0]	led_external_connection_export;
	input		reset_reset_n;
endmodule
