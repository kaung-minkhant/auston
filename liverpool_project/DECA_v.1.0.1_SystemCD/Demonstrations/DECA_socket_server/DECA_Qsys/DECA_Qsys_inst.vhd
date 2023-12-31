	component DECA_Qsys is
		port (
			clk_clk                                       : in    std_logic                     := 'X';             -- clk
			ddr3_status_external_connection_export        : in    std_logic_vector(2 downto 0)  := (others => 'X'); -- export
			dipsw_pio_export                              : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			led_pio_export                                : out   std_logic_vector(7 downto 0);                     -- export
			mem_if_ddr3_emif_pll_ref_clk_clk              : in    std_logic                     := 'X';             -- clk
			mem_if_ddr3_emif_pll_sharing_pll_mem_clk      : out   std_logic;                                        -- pll_mem_clk
			mem_if_ddr3_emif_pll_sharing_pll_write_clk    : out   std_logic;                                        -- pll_write_clk
			mem_if_ddr3_emif_pll_sharing_pll_locked       : out   std_logic;                                        -- pll_locked
			mem_if_ddr3_emif_pll_sharing_pll_capture0_clk : out   std_logic;                                        -- pll_capture0_clk
			mem_if_ddr3_emif_pll_sharing_pll_capture1_clk : out   std_logic;                                        -- pll_capture1_clk
			mem_if_ddr3_emif_status_local_init_done       : out   std_logic;                                        -- local_init_done
			mem_if_ddr3_emif_status_local_cal_success     : out   std_logic;                                        -- local_cal_success
			mem_if_ddr3_emif_status_local_cal_fail        : out   std_logic;                                        -- local_cal_fail
			memory_mem_a                                  : out   std_logic_vector(14 downto 0);                    -- mem_a
			memory_mem_ba                                 : out   std_logic_vector(2 downto 0);                     -- mem_ba
			memory_mem_ck                                 : inout std_logic_vector(0 downto 0)  := (others => 'X'); -- mem_ck
			memory_mem_ck_n                               : inout std_logic_vector(0 downto 0)  := (others => 'X'); -- mem_ck_n
			memory_mem_cke                                : out   std_logic_vector(0 downto 0);                     -- mem_cke
			memory_mem_cs_n                               : out   std_logic_vector(0 downto 0);                     -- mem_cs_n
			memory_mem_dm                                 : out   std_logic_vector(1 downto 0);                     -- mem_dm
			memory_mem_ras_n                              : out   std_logic_vector(0 downto 0);                     -- mem_ras_n
			memory_mem_cas_n                              : out   std_logic_vector(0 downto 0);                     -- mem_cas_n
			memory_mem_we_n                               : out   std_logic_vector(0 downto 0);                     -- mem_we_n
			memory_mem_reset_n                            : out   std_logic;                                        -- mem_reset_n
			memory_mem_dq                                 : inout std_logic_vector(15 downto 0) := (others => 'X'); -- mem_dq
			memory_mem_dqs                                : inout std_logic_vector(1 downto 0)  := (others => 'X'); -- mem_dqs
			memory_mem_dqs_n                              : inout std_logic_vector(1 downto 0)  := (others => 'X'); -- mem_dqs_n
			memory_mem_odt                                : out   std_logic_vector(0 downto 0);                     -- mem_odt
			nenet_reg_reset_export                        : out   std_logic;                                        -- export
			pll_areset_conduit_export                     : in    std_logic                     := 'X';             -- export
			pll_locked_conduit_export                     : out   std_logic;                                        -- export
			pll_phasedone_conduit_export                  : out   std_logic;                                        -- export
			reset_reset_n                                 : in    std_logic                     := 'X';             -- reset_n
			tse_mac_mac_mdio_connection_mdc               : out   std_logic;                                        -- mdc
			tse_mac_mac_mdio_connection_mdio_in           : in    std_logic                     := 'X';             -- mdio_in
			tse_mac_mac_mdio_connection_mdio_out          : out   std_logic;                                        -- mdio_out
			tse_mac_mac_mdio_connection_mdio_oen          : out   std_logic;                                        -- mdio_oen
			tse_mac_mac_mii_connection_mii_rx_d           : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- mii_rx_d
			tse_mac_mac_mii_connection_mii_rx_dv          : in    std_logic                     := 'X';             -- mii_rx_dv
			tse_mac_mac_mii_connection_mii_rx_err         : in    std_logic                     := 'X';             -- mii_rx_err
			tse_mac_mac_mii_connection_mii_tx_d           : out   std_logic_vector(3 downto 0);                     -- mii_tx_d
			tse_mac_mac_mii_connection_mii_tx_en          : out   std_logic;                                        -- mii_tx_en
			tse_mac_mac_mii_connection_mii_tx_err         : out   std_logic;                                        -- mii_tx_err
			tse_mac_mac_mii_connection_mii_crs            : in    std_logic                     := 'X';             -- mii_crs
			tse_mac_mac_mii_connection_mii_col            : in    std_logic                     := 'X';             -- mii_col
			tse_mac_mac_misc_connection_ff_tx_crc_fwd     : in    std_logic                     := 'X';             -- ff_tx_crc_fwd
			tse_mac_mac_misc_connection_ff_tx_septy       : out   std_logic;                                        -- ff_tx_septy
			tse_mac_mac_misc_connection_tx_ff_uflow       : out   std_logic;                                        -- tx_ff_uflow
			tse_mac_mac_misc_connection_ff_tx_a_full      : out   std_logic;                                        -- ff_tx_a_full
			tse_mac_mac_misc_connection_ff_tx_a_empty     : out   std_logic;                                        -- ff_tx_a_empty
			tse_mac_mac_misc_connection_rx_err_stat       : out   std_logic_vector(17 downto 0);                    -- rx_err_stat
			tse_mac_mac_misc_connection_rx_frm_type       : out   std_logic_vector(3 downto 0);                     -- rx_frm_type
			tse_mac_mac_misc_connection_ff_rx_dsav        : out   std_logic;                                        -- ff_rx_dsav
			tse_mac_mac_misc_connection_ff_rx_a_full      : out   std_logic;                                        -- ff_rx_a_full
			tse_mac_mac_misc_connection_ff_rx_a_empty     : out   std_logic;                                        -- ff_rx_a_empty
			tse_mac_mac_status_connection_set_10          : in    std_logic                     := 'X';             -- set_10
			tse_mac_mac_status_connection_set_1000        : in    std_logic                     := 'X';             -- set_1000
			tse_mac_mac_status_connection_eth_mode        : out   std_logic;                                        -- eth_mode
			tse_mac_mac_status_connection_ena_10          : out   std_logic;                                        -- ena_10
			tse_mac_pcs_mac_rx_clock_connection_clk       : in    std_logic                     := 'X';             -- clk
			tse_mac_pcs_mac_tx_clock_connection_clk       : in    std_logic                     := 'X';             -- clk
			user_pio_pushbtn_export                       : in    std_logic                     := 'X'              -- export
		);
	end component DECA_Qsys;

	u0 : component DECA_Qsys
		port map (
			clk_clk                                       => CONNECTED_TO_clk_clk,                                       --                                 clk.clk
			ddr3_status_external_connection_export        => CONNECTED_TO_ddr3_status_external_connection_export,        --     ddr3_status_external_connection.export
			dipsw_pio_export                              => CONNECTED_TO_dipsw_pio_export,                              --                           dipsw_pio.export
			led_pio_export                                => CONNECTED_TO_led_pio_export,                                --                             led_pio.export
			mem_if_ddr3_emif_pll_ref_clk_clk              => CONNECTED_TO_mem_if_ddr3_emif_pll_ref_clk_clk,              --        mem_if_ddr3_emif_pll_ref_clk.clk
			mem_if_ddr3_emif_pll_sharing_pll_mem_clk      => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_mem_clk,      --        mem_if_ddr3_emif_pll_sharing.pll_mem_clk
			mem_if_ddr3_emif_pll_sharing_pll_write_clk    => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_write_clk,    --                                    .pll_write_clk
			mem_if_ddr3_emif_pll_sharing_pll_locked       => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_locked,       --                                    .pll_locked
			mem_if_ddr3_emif_pll_sharing_pll_capture0_clk => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_capture0_clk, --                                    .pll_capture0_clk
			mem_if_ddr3_emif_pll_sharing_pll_capture1_clk => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_capture1_clk, --                                    .pll_capture1_clk
			mem_if_ddr3_emif_status_local_init_done       => CONNECTED_TO_mem_if_ddr3_emif_status_local_init_done,       --             mem_if_ddr3_emif_status.local_init_done
			mem_if_ddr3_emif_status_local_cal_success     => CONNECTED_TO_mem_if_ddr3_emif_status_local_cal_success,     --                                    .local_cal_success
			mem_if_ddr3_emif_status_local_cal_fail        => CONNECTED_TO_mem_if_ddr3_emif_status_local_cal_fail,        --                                    .local_cal_fail
			memory_mem_a                                  => CONNECTED_TO_memory_mem_a,                                  --                              memory.mem_a
			memory_mem_ba                                 => CONNECTED_TO_memory_mem_ba,                                 --                                    .mem_ba
			memory_mem_ck                                 => CONNECTED_TO_memory_mem_ck,                                 --                                    .mem_ck
			memory_mem_ck_n                               => CONNECTED_TO_memory_mem_ck_n,                               --                                    .mem_ck_n
			memory_mem_cke                                => CONNECTED_TO_memory_mem_cke,                                --                                    .mem_cke
			memory_mem_cs_n                               => CONNECTED_TO_memory_mem_cs_n,                               --                                    .mem_cs_n
			memory_mem_dm                                 => CONNECTED_TO_memory_mem_dm,                                 --                                    .mem_dm
			memory_mem_ras_n                              => CONNECTED_TO_memory_mem_ras_n,                              --                                    .mem_ras_n
			memory_mem_cas_n                              => CONNECTED_TO_memory_mem_cas_n,                              --                                    .mem_cas_n
			memory_mem_we_n                               => CONNECTED_TO_memory_mem_we_n,                               --                                    .mem_we_n
			memory_mem_reset_n                            => CONNECTED_TO_memory_mem_reset_n,                            --                                    .mem_reset_n
			memory_mem_dq                                 => CONNECTED_TO_memory_mem_dq,                                 --                                    .mem_dq
			memory_mem_dqs                                => CONNECTED_TO_memory_mem_dqs,                                --                                    .mem_dqs
			memory_mem_dqs_n                              => CONNECTED_TO_memory_mem_dqs_n,                              --                                    .mem_dqs_n
			memory_mem_odt                                => CONNECTED_TO_memory_mem_odt,                                --                                    .mem_odt
			nenet_reg_reset_export                        => CONNECTED_TO_nenet_reg_reset_export,                        --                     nenet_reg_reset.export
			pll_areset_conduit_export                     => CONNECTED_TO_pll_areset_conduit_export,                     --                  pll_areset_conduit.export
			pll_locked_conduit_export                     => CONNECTED_TO_pll_locked_conduit_export,                     --                  pll_locked_conduit.export
			pll_phasedone_conduit_export                  => CONNECTED_TO_pll_phasedone_conduit_export,                  --               pll_phasedone_conduit.export
			reset_reset_n                                 => CONNECTED_TO_reset_reset_n,                                 --                               reset.reset_n
			tse_mac_mac_mdio_connection_mdc               => CONNECTED_TO_tse_mac_mac_mdio_connection_mdc,               --         tse_mac_mac_mdio_connection.mdc
			tse_mac_mac_mdio_connection_mdio_in           => CONNECTED_TO_tse_mac_mac_mdio_connection_mdio_in,           --                                    .mdio_in
			tse_mac_mac_mdio_connection_mdio_out          => CONNECTED_TO_tse_mac_mac_mdio_connection_mdio_out,          --                                    .mdio_out
			tse_mac_mac_mdio_connection_mdio_oen          => CONNECTED_TO_tse_mac_mac_mdio_connection_mdio_oen,          --                                    .mdio_oen
			tse_mac_mac_mii_connection_mii_rx_d           => CONNECTED_TO_tse_mac_mac_mii_connection_mii_rx_d,           --          tse_mac_mac_mii_connection.mii_rx_d
			tse_mac_mac_mii_connection_mii_rx_dv          => CONNECTED_TO_tse_mac_mac_mii_connection_mii_rx_dv,          --                                    .mii_rx_dv
			tse_mac_mac_mii_connection_mii_rx_err         => CONNECTED_TO_tse_mac_mac_mii_connection_mii_rx_err,         --                                    .mii_rx_err
			tse_mac_mac_mii_connection_mii_tx_d           => CONNECTED_TO_tse_mac_mac_mii_connection_mii_tx_d,           --                                    .mii_tx_d
			tse_mac_mac_mii_connection_mii_tx_en          => CONNECTED_TO_tse_mac_mac_mii_connection_mii_tx_en,          --                                    .mii_tx_en
			tse_mac_mac_mii_connection_mii_tx_err         => CONNECTED_TO_tse_mac_mac_mii_connection_mii_tx_err,         --                                    .mii_tx_err
			tse_mac_mac_mii_connection_mii_crs            => CONNECTED_TO_tse_mac_mac_mii_connection_mii_crs,            --                                    .mii_crs
			tse_mac_mac_mii_connection_mii_col            => CONNECTED_TO_tse_mac_mac_mii_connection_mii_col,            --                                    .mii_col
			tse_mac_mac_misc_connection_ff_tx_crc_fwd     => CONNECTED_TO_tse_mac_mac_misc_connection_ff_tx_crc_fwd,     --         tse_mac_mac_misc_connection.ff_tx_crc_fwd
			tse_mac_mac_misc_connection_ff_tx_septy       => CONNECTED_TO_tse_mac_mac_misc_connection_ff_tx_septy,       --                                    .ff_tx_septy
			tse_mac_mac_misc_connection_tx_ff_uflow       => CONNECTED_TO_tse_mac_mac_misc_connection_tx_ff_uflow,       --                                    .tx_ff_uflow
			tse_mac_mac_misc_connection_ff_tx_a_full      => CONNECTED_TO_tse_mac_mac_misc_connection_ff_tx_a_full,      --                                    .ff_tx_a_full
			tse_mac_mac_misc_connection_ff_tx_a_empty     => CONNECTED_TO_tse_mac_mac_misc_connection_ff_tx_a_empty,     --                                    .ff_tx_a_empty
			tse_mac_mac_misc_connection_rx_err_stat       => CONNECTED_TO_tse_mac_mac_misc_connection_rx_err_stat,       --                                    .rx_err_stat
			tse_mac_mac_misc_connection_rx_frm_type       => CONNECTED_TO_tse_mac_mac_misc_connection_rx_frm_type,       --                                    .rx_frm_type
			tse_mac_mac_misc_connection_ff_rx_dsav        => CONNECTED_TO_tse_mac_mac_misc_connection_ff_rx_dsav,        --                                    .ff_rx_dsav
			tse_mac_mac_misc_connection_ff_rx_a_full      => CONNECTED_TO_tse_mac_mac_misc_connection_ff_rx_a_full,      --                                    .ff_rx_a_full
			tse_mac_mac_misc_connection_ff_rx_a_empty     => CONNECTED_TO_tse_mac_mac_misc_connection_ff_rx_a_empty,     --                                    .ff_rx_a_empty
			tse_mac_mac_status_connection_set_10          => CONNECTED_TO_tse_mac_mac_status_connection_set_10,          --       tse_mac_mac_status_connection.set_10
			tse_mac_mac_status_connection_set_1000        => CONNECTED_TO_tse_mac_mac_status_connection_set_1000,        --                                    .set_1000
			tse_mac_mac_status_connection_eth_mode        => CONNECTED_TO_tse_mac_mac_status_connection_eth_mode,        --                                    .eth_mode
			tse_mac_mac_status_connection_ena_10          => CONNECTED_TO_tse_mac_mac_status_connection_ena_10,          --                                    .ena_10
			tse_mac_pcs_mac_rx_clock_connection_clk       => CONNECTED_TO_tse_mac_pcs_mac_rx_clock_connection_clk,       -- tse_mac_pcs_mac_rx_clock_connection.clk
			tse_mac_pcs_mac_tx_clock_connection_clk       => CONNECTED_TO_tse_mac_pcs_mac_tx_clock_connection_clk,       -- tse_mac_pcs_mac_tx_clock_connection.clk
			user_pio_pushbtn_export                       => CONNECTED_TO_user_pio_pushbtn_export                        --                    user_pio_pushbtn.export
		);

