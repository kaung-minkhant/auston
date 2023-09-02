	component amax10_qsys is
		port (
			clk_clk                                   : in    std_logic                    := 'X';             -- clk
			key_external_connection_export            : in    std_logic_vector(1 downto 0) := (others => 'X'); -- export
			led_external_connection_export            : out   std_logic_vector(7 downto 0);                    -- export
			reset_reset_n                             : in    std_logic                    := 'X';             -- reset_n
			sd_clk_export                             : out   std_logic;                                       -- export
			sd_cmd_export                             : inout std_logic                    := 'X';             -- export
			sd_cmd_dir_external_connection_export     : out   std_logic;                                       -- export
			sd_dat_export                             : inout std_logic_vector(3 downto 0) := (others => 'X'); -- export
			sd_data0_dir_external_connection_export   : out   std_logic;                                       -- export
			sd_data123_dir_external_connection_export : out   std_logic;                                       -- export
			sd_sel_external_connection_export         : out   std_logic                                        -- export
		);
	end component amax10_qsys;

	u0 : component amax10_qsys
		port map (
			clk_clk                                   => CONNECTED_TO_clk_clk,                                   --                                clk.clk
			key_external_connection_export            => CONNECTED_TO_key_external_connection_export,            --            key_external_connection.export
			led_external_connection_export            => CONNECTED_TO_led_external_connection_export,            --            led_external_connection.export
			reset_reset_n                             => CONNECTED_TO_reset_reset_n,                             --                              reset.reset_n
			sd_clk_export                             => CONNECTED_TO_sd_clk_export,                             --                             sd_clk.export
			sd_cmd_export                             => CONNECTED_TO_sd_cmd_export,                             --                             sd_cmd.export
			sd_cmd_dir_external_connection_export     => CONNECTED_TO_sd_cmd_dir_external_connection_export,     --     sd_cmd_dir_external_connection.export
			sd_dat_export                             => CONNECTED_TO_sd_dat_export,                             --                             sd_dat.export
			sd_data0_dir_external_connection_export   => CONNECTED_TO_sd_data0_dir_external_connection_export,   --   sd_data0_dir_external_connection.export
			sd_data123_dir_external_connection_export => CONNECTED_TO_sd_data123_dir_external_connection_export, -- sd_data123_dir_external_connection.export
			sd_sel_external_connection_export         => CONNECTED_TO_sd_sel_external_connection_export          --         sd_sel_external_connection.export
		);

