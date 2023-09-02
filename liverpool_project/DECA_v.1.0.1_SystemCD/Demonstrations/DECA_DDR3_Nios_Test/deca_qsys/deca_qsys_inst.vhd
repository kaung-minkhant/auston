	component deca_qsys is
		port (
			clk_clk                                       : in    std_logic                     := 'X';             -- clk
			ddr3_status_external_connection_export        : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			key_external_connection_export                : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- export
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
			reset_reset_n                                 : in    std_logic                     := 'X'              -- reset_n
		);
	end component deca_qsys;

	u0 : component deca_qsys
		port map (
			clk_clk                                       => CONNECTED_TO_clk_clk,                                       --                             clk.clk
			ddr3_status_external_connection_export        => CONNECTED_TO_ddr3_status_external_connection_export,        -- ddr3_status_external_connection.export
			key_external_connection_export                => CONNECTED_TO_key_external_connection_export,                --         key_external_connection.export
			mem_if_ddr3_emif_pll_ref_clk_clk              => CONNECTED_TO_mem_if_ddr3_emif_pll_ref_clk_clk,              --    mem_if_ddr3_emif_pll_ref_clk.clk
			mem_if_ddr3_emif_pll_sharing_pll_mem_clk      => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_mem_clk,      --    mem_if_ddr3_emif_pll_sharing.pll_mem_clk
			mem_if_ddr3_emif_pll_sharing_pll_write_clk    => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_write_clk,    --                                .pll_write_clk
			mem_if_ddr3_emif_pll_sharing_pll_locked       => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_locked,       --                                .pll_locked
			mem_if_ddr3_emif_pll_sharing_pll_capture0_clk => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_capture0_clk, --                                .pll_capture0_clk
			mem_if_ddr3_emif_pll_sharing_pll_capture1_clk => CONNECTED_TO_mem_if_ddr3_emif_pll_sharing_pll_capture1_clk, --                                .pll_capture1_clk
			mem_if_ddr3_emif_status_local_init_done       => CONNECTED_TO_mem_if_ddr3_emif_status_local_init_done,       --         mem_if_ddr3_emif_status.local_init_done
			mem_if_ddr3_emif_status_local_cal_success     => CONNECTED_TO_mem_if_ddr3_emif_status_local_cal_success,     --                                .local_cal_success
			mem_if_ddr3_emif_status_local_cal_fail        => CONNECTED_TO_mem_if_ddr3_emif_status_local_cal_fail,        --                                .local_cal_fail
			memory_mem_a                                  => CONNECTED_TO_memory_mem_a,                                  --                          memory.mem_a
			memory_mem_ba                                 => CONNECTED_TO_memory_mem_ba,                                 --                                .mem_ba
			memory_mem_ck                                 => CONNECTED_TO_memory_mem_ck,                                 --                                .mem_ck
			memory_mem_ck_n                               => CONNECTED_TO_memory_mem_ck_n,                               --                                .mem_ck_n
			memory_mem_cke                                => CONNECTED_TO_memory_mem_cke,                                --                                .mem_cke
			memory_mem_cs_n                               => CONNECTED_TO_memory_mem_cs_n,                               --                                .mem_cs_n
			memory_mem_dm                                 => CONNECTED_TO_memory_mem_dm,                                 --                                .mem_dm
			memory_mem_ras_n                              => CONNECTED_TO_memory_mem_ras_n,                              --                                .mem_ras_n
			memory_mem_cas_n                              => CONNECTED_TO_memory_mem_cas_n,                              --                                .mem_cas_n
			memory_mem_we_n                               => CONNECTED_TO_memory_mem_we_n,                               --                                .mem_we_n
			memory_mem_reset_n                            => CONNECTED_TO_memory_mem_reset_n,                            --                                .mem_reset_n
			memory_mem_dq                                 => CONNECTED_TO_memory_mem_dq,                                 --                                .mem_dq
			memory_mem_dqs                                => CONNECTED_TO_memory_mem_dqs,                                --                                .mem_dqs
			memory_mem_dqs_n                              => CONNECTED_TO_memory_mem_dqs_n,                              --                                .mem_dqs_n
			memory_mem_odt                                => CONNECTED_TO_memory_mem_odt,                                --                                .mem_odt
			reset_reset_n                                 => CONNECTED_TO_reset_reset_n                                  --                           reset.reset_n
		);

