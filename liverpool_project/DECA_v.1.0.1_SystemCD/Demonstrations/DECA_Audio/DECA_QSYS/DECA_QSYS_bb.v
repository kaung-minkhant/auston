
module DECA_QSYS (
	audio_i2c_export_scl_pad_io,
	audio_i2c_export_sda_pad_io,
	audio_reset_n_external_connection_export,
	clk_clk,
	pio_key_external_connection_export,
	pio_led_external_connection_export,
	pio_sw_external_connection_export,
	pll_audio_clk,
	reset_reset_n);	

	inout		audio_i2c_export_scl_pad_io;
	inout		audio_i2c_export_sda_pad_io;
	output		audio_reset_n_external_connection_export;
	input		clk_clk;
	input	[1:0]	pio_key_external_connection_export;
	output	[7:0]	pio_led_external_connection_export;
	input	[1:0]	pio_sw_external_connection_export;
	output		pll_audio_clk;
	input		reset_reset_n;
endmodule
