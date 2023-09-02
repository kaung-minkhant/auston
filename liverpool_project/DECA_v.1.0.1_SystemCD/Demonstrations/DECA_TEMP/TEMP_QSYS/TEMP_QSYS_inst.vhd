	component TEMP_QSYS is
		port (
			altpll_areset_conduit_export       : in    std_logic                    := 'X'; -- export
			altpll_locked_conduit_export       : out   std_logic;                           -- export
			altpll_phasedone_conduit_export    : out   std_logic;                           -- export
			clk_50                             : in    std_logic                    := 'X'; -- clk
			reset_n                            : in    std_logic                    := 'X'; -- reset_n
			pio_led_external_connection_export : out   std_logic_vector(7 downto 0);        -- export
			temp_controller_conduit_end_cs_n   : out   std_logic;                           -- cs_n
			temp_controller_conduit_end_sc     : out   std_logic;                           -- sc
			temp_controller_conduit_end_sio    : inout std_logic                    := 'X'  -- sio
		);
	end component TEMP_QSYS;

	u0 : component TEMP_QSYS
		port map (
			altpll_areset_conduit_export       => CONNECTED_TO_altpll_areset_conduit_export,       --       altpll_areset_conduit.export
			altpll_locked_conduit_export       => CONNECTED_TO_altpll_locked_conduit_export,       --       altpll_locked_conduit.export
			altpll_phasedone_conduit_export    => CONNECTED_TO_altpll_phasedone_conduit_export,    --    altpll_phasedone_conduit.export
			clk_50                             => CONNECTED_TO_clk_50,                             --               clk_50_clk_in.clk
			reset_n                            => CONNECTED_TO_reset_n,                            --         clk_50_clk_in_reset.reset_n
			pio_led_external_connection_export => CONNECTED_TO_pio_led_external_connection_export, -- pio_led_external_connection.export
			temp_controller_conduit_end_cs_n   => CONNECTED_TO_temp_controller_conduit_end_cs_n,   -- temp_controller_conduit_end.cs_n
			temp_controller_conduit_end_sc     => CONNECTED_TO_temp_controller_conduit_end_sc,     --                            .sc
			temp_controller_conduit_end_sio    => CONNECTED_TO_temp_controller_conduit_end_sio     --                            .sio
		);

