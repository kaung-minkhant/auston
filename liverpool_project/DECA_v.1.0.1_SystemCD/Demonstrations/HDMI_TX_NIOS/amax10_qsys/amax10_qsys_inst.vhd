	component amax10_qsys is
		port (
			clk_clk                                : in    std_logic := 'X'; -- clk
			hdmi_tx_int_external_connection_export : in    std_logic := 'X'; -- export
			i2c_scl_external_connection_export     : out   std_logic;        -- export
			i2c_sda_external_connection_export     : inout std_logic := 'X'; -- export
			reset_reset_n                          : in    std_logic := 'X'  -- reset_n
		);
	end component amax10_qsys;

	u0 : component amax10_qsys
		port map (
			clk_clk                                => CONNECTED_TO_clk_clk,                                --                             clk.clk
			hdmi_tx_int_external_connection_export => CONNECTED_TO_hdmi_tx_int_external_connection_export, -- hdmi_tx_int_external_connection.export
			i2c_scl_external_connection_export     => CONNECTED_TO_i2c_scl_external_connection_export,     --     i2c_scl_external_connection.export
			i2c_sda_external_connection_export     => CONNECTED_TO_i2c_sda_external_connection_export,     --     i2c_sda_external_connection.export
			reset_reset_n                          => CONNECTED_TO_reset_reset_n                           --                           reset.reset_n
		);

