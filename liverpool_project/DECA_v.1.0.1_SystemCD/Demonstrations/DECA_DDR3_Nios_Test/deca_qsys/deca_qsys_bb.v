
module deca_qsys (
	clk_clk,
	ddr3_status_external_connection_export,
	key_external_connection_export,
	mem_if_ddr3_emif_pll_ref_clk_clk,
	mem_if_ddr3_emif_pll_sharing_pll_mem_clk,
	mem_if_ddr3_emif_pll_sharing_pll_write_clk,
	mem_if_ddr3_emif_pll_sharing_pll_locked,
	mem_if_ddr3_emif_pll_sharing_pll_capture0_clk,
	mem_if_ddr3_emif_pll_sharing_pll_capture1_clk,
	mem_if_ddr3_emif_status_local_init_done,
	mem_if_ddr3_emif_status_local_cal_success,
	mem_if_ddr3_emif_status_local_cal_fail,
	memory_mem_a,
	memory_mem_ba,
	memory_mem_ck,
	memory_mem_ck_n,
	memory_mem_cke,
	memory_mem_cs_n,
	memory_mem_dm,
	memory_mem_ras_n,
	memory_mem_cas_n,
	memory_mem_we_n,
	memory_mem_reset_n,
	memory_mem_dq,
	memory_mem_dqs,
	memory_mem_dqs_n,
	memory_mem_odt,
	reset_reset_n);	

	input		clk_clk;
	input	[3:0]	ddr3_status_external_connection_export;
	input	[1:0]	key_external_connection_export;
	input		mem_if_ddr3_emif_pll_ref_clk_clk;
	output		mem_if_ddr3_emif_pll_sharing_pll_mem_clk;
	output		mem_if_ddr3_emif_pll_sharing_pll_write_clk;
	output		mem_if_ddr3_emif_pll_sharing_pll_locked;
	output		mem_if_ddr3_emif_pll_sharing_pll_capture0_clk;
	output		mem_if_ddr3_emif_pll_sharing_pll_capture1_clk;
	output		mem_if_ddr3_emif_status_local_init_done;
	output		mem_if_ddr3_emif_status_local_cal_success;
	output		mem_if_ddr3_emif_status_local_cal_fail;
	output	[14:0]	memory_mem_a;
	output	[2:0]	memory_mem_ba;
	inout	[0:0]	memory_mem_ck;
	inout	[0:0]	memory_mem_ck_n;
	output	[0:0]	memory_mem_cke;
	output	[0:0]	memory_mem_cs_n;
	output	[1:0]	memory_mem_dm;
	output	[0:0]	memory_mem_ras_n;
	output	[0:0]	memory_mem_cas_n;
	output	[0:0]	memory_mem_we_n;
	output		memory_mem_reset_n;
	inout	[15:0]	memory_mem_dq;
	inout	[1:0]	memory_mem_dqs;
	inout	[1:0]	memory_mem_dqs_n;
	output	[0:0]	memory_mem_odt;
	input		reset_reset_n;
endmodule
