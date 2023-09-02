
module DECA_qsys (
	altpll_0_areset_conduit_export,
	altpll_0_locked_conduit_export,
	altpll_0_phasedone_conduit_export,
	clk_clk,
	reset_reset_n,
	rh_temp_drdy_n_external_connection_export,
	rh_temp_i2c_scl_external_connection_export,
	rh_temp_i2c_sda_external_connection_export);	

	input		altpll_0_areset_conduit_export;
	output		altpll_0_locked_conduit_export;
	output		altpll_0_phasedone_conduit_export;
	input		clk_clk;
	input		reset_reset_n;
	input		rh_temp_drdy_n_external_connection_export;
	output		rh_temp_i2c_scl_external_connection_export;
	inout		rh_temp_i2c_sda_external_connection_export;
endmodule
