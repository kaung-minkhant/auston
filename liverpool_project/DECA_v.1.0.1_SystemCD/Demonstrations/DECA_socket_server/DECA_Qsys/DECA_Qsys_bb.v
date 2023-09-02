
module DECA_Qsys (
	clk_clk,
	ddr3_status_external_connection_export,
	dipsw_pio_export,
	led_pio_export,
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
	nenet_reg_reset_export,
	pll_areset_conduit_export,
	pll_locked_conduit_export,
	pll_phasedone_conduit_export,
	reset_reset_n,
	tse_mac_mac_mdio_connection_mdc,
	tse_mac_mac_mdio_connection_mdio_in,
	tse_mac_mac_mdio_connection_mdio_out,
	tse_mac_mac_mdio_connection_mdio_oen,
	tse_mac_mac_mii_connection_mii_rx_d,
	tse_mac_mac_mii_connection_mii_rx_dv,
	tse_mac_mac_mii_connection_mii_rx_err,
	tse_mac_mac_mii_connection_mii_tx_d,
	tse_mac_mac_mii_connection_mii_tx_en,
	tse_mac_mac_mii_connection_mii_tx_err,
	tse_mac_mac_mii_connection_mii_crs,
	tse_mac_mac_mii_connection_mii_col,
	tse_mac_mac_misc_connection_ff_tx_crc_fwd,
	tse_mac_mac_misc_connection_ff_tx_septy,
	tse_mac_mac_misc_connection_tx_ff_uflow,
	tse_mac_mac_misc_connection_ff_tx_a_full,
	tse_mac_mac_misc_connection_ff_tx_a_empty,
	tse_mac_mac_misc_connection_rx_err_stat,
	tse_mac_mac_misc_connection_rx_frm_type,
	tse_mac_mac_misc_connection_ff_rx_dsav,
	tse_mac_mac_misc_connection_ff_rx_a_full,
	tse_mac_mac_misc_connection_ff_rx_a_empty,
	tse_mac_mac_status_connection_set_10,
	tse_mac_mac_status_connection_set_1000,
	tse_mac_mac_status_connection_eth_mode,
	tse_mac_mac_status_connection_ena_10,
	tse_mac_pcs_mac_rx_clock_connection_clk,
	tse_mac_pcs_mac_tx_clock_connection_clk,
	user_pio_pushbtn_export);	

	input		clk_clk;
	input	[2:0]	ddr3_status_external_connection_export;
	input	[1:0]	dipsw_pio_export;
	output	[7:0]	led_pio_export;
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
	output		nenet_reg_reset_export;
	input		pll_areset_conduit_export;
	output		pll_locked_conduit_export;
	output		pll_phasedone_conduit_export;
	input		reset_reset_n;
	output		tse_mac_mac_mdio_connection_mdc;
	input		tse_mac_mac_mdio_connection_mdio_in;
	output		tse_mac_mac_mdio_connection_mdio_out;
	output		tse_mac_mac_mdio_connection_mdio_oen;
	input	[3:0]	tse_mac_mac_mii_connection_mii_rx_d;
	input		tse_mac_mac_mii_connection_mii_rx_dv;
	input		tse_mac_mac_mii_connection_mii_rx_err;
	output	[3:0]	tse_mac_mac_mii_connection_mii_tx_d;
	output		tse_mac_mac_mii_connection_mii_tx_en;
	output		tse_mac_mac_mii_connection_mii_tx_err;
	input		tse_mac_mac_mii_connection_mii_crs;
	input		tse_mac_mac_mii_connection_mii_col;
	input		tse_mac_mac_misc_connection_ff_tx_crc_fwd;
	output		tse_mac_mac_misc_connection_ff_tx_septy;
	output		tse_mac_mac_misc_connection_tx_ff_uflow;
	output		tse_mac_mac_misc_connection_ff_tx_a_full;
	output		tse_mac_mac_misc_connection_ff_tx_a_empty;
	output	[17:0]	tse_mac_mac_misc_connection_rx_err_stat;
	output	[3:0]	tse_mac_mac_misc_connection_rx_frm_type;
	output		tse_mac_mac_misc_connection_ff_rx_dsav;
	output		tse_mac_mac_misc_connection_ff_rx_a_full;
	output		tse_mac_mac_misc_connection_ff_rx_a_empty;
	input		tse_mac_mac_status_connection_set_10;
	input		tse_mac_mac_status_connection_set_1000;
	output		tse_mac_mac_status_connection_eth_mode;
	output		tse_mac_mac_status_connection_ena_10;
	input		tse_mac_pcs_mac_rx_clock_connection_clk;
	input		tse_mac_pcs_mac_tx_clock_connection_clk;
	input		user_pio_pushbtn_export;
endmodule
