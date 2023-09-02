	component DECA_QSYS is
		port (
			clk_clk                                     : in    std_logic                    := 'X';             -- clk
			pio_key_external_connection_export          : in    std_logic_vector(1 downto 0) := (others => 'X'); -- export
			pio_led_external_connection_export          : out   std_logic_vector(7 downto 0);                    -- export
			pio_sw_external_connection_export           : in    std_logic_vector(1 downto 0) := (others => 'X'); -- export
			pmonitor_alert_external_connection_export   : in    std_logic                    := 'X';             -- export
			pmonitor_i2c_scl_external_connection_export : out   std_logic;                                       -- export
			pmonitor_i2c_sda_external_connection_export : inout std_logic                    := 'X';             -- export
			reset_reset_n                               : in    std_logic                    := 'X'              -- reset_n
		);
	end component DECA_QSYS;

	u0 : component DECA_QSYS
		port map (
			clk_clk                                     => CONNECTED_TO_clk_clk,                                     --                                  clk.clk
			pio_key_external_connection_export          => CONNECTED_TO_pio_key_external_connection_export,          --          pio_key_external_connection.export
			pio_led_external_connection_export          => CONNECTED_TO_pio_led_external_connection_export,          --          pio_led_external_connection.export
			pio_sw_external_connection_export           => CONNECTED_TO_pio_sw_external_connection_export,           --           pio_sw_external_connection.export
			pmonitor_alert_external_connection_export   => CONNECTED_TO_pmonitor_alert_external_connection_export,   --   pmonitor_alert_external_connection.export
			pmonitor_i2c_scl_external_connection_export => CONNECTED_TO_pmonitor_i2c_scl_external_connection_export, -- pmonitor_i2c_scl_external_connection.export
			pmonitor_i2c_sda_external_connection_export => CONNECTED_TO_pmonitor_i2c_sda_external_connection_export, -- pmonitor_i2c_sda_external_connection.export
			reset_reset_n                               => CONNECTED_TO_reset_reset_n                                --                                reset.reset_n
		);

