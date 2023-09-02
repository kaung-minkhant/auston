
module amax10_qsys (
	clk_clk,
	key_external_connection_export,
	led_external_connection_export,
	reset_reset_n,
	sd_clk_export,
	sd_cmd_export,
	sd_cmd_dir_external_connection_export,
	sd_dat_export,
	sd_data0_dir_external_connection_export,
	sd_data123_dir_external_connection_export,
	sd_sel_external_connection_export);	

	input		clk_clk;
	input	[1:0]	key_external_connection_export;
	output	[7:0]	led_external_connection_export;
	input		reset_reset_n;
	output		sd_clk_export;
	inout		sd_cmd_export;
	output		sd_cmd_dir_external_connection_export;
	inout	[3:0]	sd_dat_export;
	output		sd_data0_dir_external_connection_export;
	output		sd_data123_dir_external_connection_export;
	output		sd_sel_external_connection_export;
endmodule
