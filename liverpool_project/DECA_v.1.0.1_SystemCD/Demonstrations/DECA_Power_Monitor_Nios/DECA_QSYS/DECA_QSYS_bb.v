
module DECA_QSYS (
	clk_clk,
	pio_key_external_connection_export,
	pio_led_external_connection_export,
	pio_sw_external_connection_export,
	pmonitor_alert_external_connection_export,
	pmonitor_i2c_scl_external_connection_export,
	pmonitor_i2c_sda_external_connection_export,
	reset_reset_n);	

	input		clk_clk;
	input	[1:0]	pio_key_external_connection_export;
	output	[7:0]	pio_led_external_connection_export;
	input	[1:0]	pio_sw_external_connection_export;
	input		pmonitor_alert_external_connection_export;
	output		pmonitor_i2c_scl_external_connection_export;
	inout		pmonitor_i2c_sda_external_connection_export;
	input		reset_reset_n;
endmodule
