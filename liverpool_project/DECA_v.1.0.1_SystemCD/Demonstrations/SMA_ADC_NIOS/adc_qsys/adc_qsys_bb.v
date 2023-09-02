
module adc_qsys (
	altpll_adc_locked_conduit_export,
	clk_clk,
	clk_adc_10_clk,
	clk_adc_10_reset_n_reset_n,
	modular_adc_0_adc_pll_locked_export,
	modular_adc_1_adc_pll_locked_export,
	reset_reset_n);	

	output		altpll_adc_locked_conduit_export;
	input		clk_clk;
	input		clk_adc_10_clk;
	input		clk_adc_10_reset_n_reset_n;
	input		modular_adc_0_adc_pll_locked_export;
	input		modular_adc_1_adc_pll_locked_export;
	input		reset_reset_n;
endmodule
