
module TEMP_QSYS (
	altpll_areset_conduit_export,
	altpll_locked_conduit_export,
	altpll_phasedone_conduit_export,
	clk_50,
	reset_n,
	pio_led_external_connection_export,
	temp_controller_conduit_end_cs_n,
	temp_controller_conduit_end_sc,
	temp_controller_conduit_end_sio);	

	input		altpll_areset_conduit_export;
	output		altpll_locked_conduit_export;
	output		altpll_phasedone_conduit_export;
	input		clk_50;
	input		reset_n;
	output	[7:0]	pio_led_external_connection_export;
	output		temp_controller_conduit_end_cs_n;
	output		temp_controller_conduit_end_sc;
	inout		temp_controller_conduit_end_sio;
endmodule
