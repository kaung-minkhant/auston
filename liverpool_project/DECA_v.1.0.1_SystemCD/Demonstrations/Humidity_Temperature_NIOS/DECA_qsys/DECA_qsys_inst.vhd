	component DECA_qsys is
		port (
			altpll_0_areset_conduit_export             : in    std_logic := 'X'; -- export
			altpll_0_locked_conduit_export             : out   std_logic;        -- export
			altpll_0_phasedone_conduit_export          : out   std_logic;        -- export
			clk_clk                                    : in    std_logic := 'X'; -- clk
			reset_reset_n                              : in    std_logic := 'X'; -- reset_n
			rh_temp_drdy_n_external_connection_export  : in    std_logic := 'X'; -- export
			rh_temp_i2c_scl_external_connection_export : out   std_logic;        -- export
			rh_temp_i2c_sda_external_connection_export : inout std_logic := 'X'  -- export
		);
	end component DECA_qsys;

	u0 : component DECA_qsys
		port map (
			altpll_0_areset_conduit_export             => CONNECTED_TO_altpll_0_areset_conduit_export,             --             altpll_0_areset_conduit.export
			altpll_0_locked_conduit_export             => CONNECTED_TO_altpll_0_locked_conduit_export,             --             altpll_0_locked_conduit.export
			altpll_0_phasedone_conduit_export          => CONNECTED_TO_altpll_0_phasedone_conduit_export,          --          altpll_0_phasedone_conduit.export
			clk_clk                                    => CONNECTED_TO_clk_clk,                                    --                                 clk.clk
			reset_reset_n                              => CONNECTED_TO_reset_reset_n,                              --                               reset.reset_n
			rh_temp_drdy_n_external_connection_export  => CONNECTED_TO_rh_temp_drdy_n_external_connection_export,  --  rh_temp_drdy_n_external_connection.export
			rh_temp_i2c_scl_external_connection_export => CONNECTED_TO_rh_temp_i2c_scl_external_connection_export, -- rh_temp_i2c_scl_external_connection.export
			rh_temp_i2c_sda_external_connection_export => CONNECTED_TO_rh_temp_i2c_sda_external_connection_export  -- rh_temp_i2c_sda_external_connection.export
		);

