	component adc_qsys is
		port (
			altpll_adc_locked_conduit_export    : out std_logic;        -- export
			clk_clk                             : in  std_logic := 'X'; -- clk
			clk_adc_10_clk                      : in  std_logic := 'X'; -- clk
			clk_adc_10_reset_n_reset_n          : in  std_logic := 'X'; -- reset_n
			modular_adc_0_adc_pll_locked_export : in  std_logic := 'X'; -- export
			modular_adc_1_adc_pll_locked_export : in  std_logic := 'X'; -- export
			reset_reset_n                       : in  std_logic := 'X'  -- reset_n
		);
	end component adc_qsys;

	u0 : component adc_qsys
		port map (
			altpll_adc_locked_conduit_export    => CONNECTED_TO_altpll_adc_locked_conduit_export,    --    altpll_adc_locked_conduit.export
			clk_clk                             => CONNECTED_TO_clk_clk,                             --                          clk.clk
			clk_adc_10_clk                      => CONNECTED_TO_clk_adc_10_clk,                      --                   clk_adc_10.clk
			clk_adc_10_reset_n_reset_n          => CONNECTED_TO_clk_adc_10_reset_n_reset_n,          --           clk_adc_10_reset_n.reset_n
			modular_adc_0_adc_pll_locked_export => CONNECTED_TO_modular_adc_0_adc_pll_locked_export, -- modular_adc_0_adc_pll_locked.export
			modular_adc_1_adc_pll_locked_export => CONNECTED_TO_modular_adc_1_adc_pll_locked_export, -- modular_adc_1_adc_pll_locked.export
			reset_reset_n                       => CONNECTED_TO_reset_reset_n                        --                        reset.reset_n
		);

