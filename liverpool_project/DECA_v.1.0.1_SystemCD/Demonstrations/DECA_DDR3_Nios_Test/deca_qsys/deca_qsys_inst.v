	deca_qsys u0 (
		.clk_clk                                       (<connected-to-clk_clk>),                                       //                             clk.clk
		.ddr3_status_external_connection_export        (<connected-to-ddr3_status_external_connection_export>),        // ddr3_status_external_connection.export
		.key_external_connection_export                (<connected-to-key_external_connection_export>),                //         key_external_connection.export
		.mem_if_ddr3_emif_pll_ref_clk_clk              (<connected-to-mem_if_ddr3_emif_pll_ref_clk_clk>),              //    mem_if_ddr3_emif_pll_ref_clk.clk
		.mem_if_ddr3_emif_pll_sharing_pll_mem_clk      (<connected-to-mem_if_ddr3_emif_pll_sharing_pll_mem_clk>),      //    mem_if_ddr3_emif_pll_sharing.pll_mem_clk
		.mem_if_ddr3_emif_pll_sharing_pll_write_clk    (<connected-to-mem_if_ddr3_emif_pll_sharing_pll_write_clk>),    //                                .pll_write_clk
		.mem_if_ddr3_emif_pll_sharing_pll_locked       (<connected-to-mem_if_ddr3_emif_pll_sharing_pll_locked>),       //                                .pll_locked
		.mem_if_ddr3_emif_pll_sharing_pll_capture0_clk (<connected-to-mem_if_ddr3_emif_pll_sharing_pll_capture0_clk>), //                                .pll_capture0_clk
		.mem_if_ddr3_emif_pll_sharing_pll_capture1_clk (<connected-to-mem_if_ddr3_emif_pll_sharing_pll_capture1_clk>), //                                .pll_capture1_clk
		.mem_if_ddr3_emif_status_local_init_done       (<connected-to-mem_if_ddr3_emif_status_local_init_done>),       //         mem_if_ddr3_emif_status.local_init_done
		.mem_if_ddr3_emif_status_local_cal_success     (<connected-to-mem_if_ddr3_emif_status_local_cal_success>),     //                                .local_cal_success
		.mem_if_ddr3_emif_status_local_cal_fail        (<connected-to-mem_if_ddr3_emif_status_local_cal_fail>),        //                                .local_cal_fail
		.memory_mem_a                                  (<connected-to-memory_mem_a>),                                  //                          memory.mem_a
		.memory_mem_ba                                 (<connected-to-memory_mem_ba>),                                 //                                .mem_ba
		.memory_mem_ck                                 (<connected-to-memory_mem_ck>),                                 //                                .mem_ck
		.memory_mem_ck_n                               (<connected-to-memory_mem_ck_n>),                               //                                .mem_ck_n
		.memory_mem_cke                                (<connected-to-memory_mem_cke>),                                //                                .mem_cke
		.memory_mem_cs_n                               (<connected-to-memory_mem_cs_n>),                               //                                .mem_cs_n
		.memory_mem_dm                                 (<connected-to-memory_mem_dm>),                                 //                                .mem_dm
		.memory_mem_ras_n                              (<connected-to-memory_mem_ras_n>),                              //                                .mem_ras_n
		.memory_mem_cas_n                              (<connected-to-memory_mem_cas_n>),                              //                                .mem_cas_n
		.memory_mem_we_n                               (<connected-to-memory_mem_we_n>),                               //                                .mem_we_n
		.memory_mem_reset_n                            (<connected-to-memory_mem_reset_n>),                            //                                .mem_reset_n
		.memory_mem_dq                                 (<connected-to-memory_mem_dq>),                                 //                                .mem_dq
		.memory_mem_dqs                                (<connected-to-memory_mem_dqs>),                                //                                .mem_dqs
		.memory_mem_dqs_n                              (<connected-to-memory_mem_dqs_n>),                              //                                .mem_dqs_n
		.memory_mem_odt                                (<connected-to-memory_mem_odt>),                                //                                .mem_odt
		.reset_reset_n                                 (<connected-to-reset_reset_n>)                                  //                           reset.reset_n
	);

