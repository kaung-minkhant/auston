
module amax10_qsys (
	clk_clk,
	hdmi_tx_int_external_connection_export,
	i2c_scl_external_connection_export,
	i2c_sda_external_connection_export,
	reset_reset_n);	

	input		clk_clk;
	input		hdmi_tx_int_external_connection_export;
	output		i2c_scl_external_connection_export;
	inout		i2c_sda_external_connection_export;
	input		reset_reset_n;
endmodule
