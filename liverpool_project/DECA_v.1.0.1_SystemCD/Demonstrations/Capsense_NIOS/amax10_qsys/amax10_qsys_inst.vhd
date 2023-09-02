	component amax10_qsys is
		port (
			clk_clk                         : in    std_logic                    := 'X'; -- clk
			i2c_opencores_export_scl_pad_io : inout std_logic                    := 'X'; -- scl_pad_io
			i2c_opencores_export_sda_pad_io : inout std_logic                    := 'X'; -- sda_pad_io
			led_external_connection_export  : out   std_logic_vector(7 downto 0);        -- export
			reset_reset_n                   : in    std_logic                    := 'X'  -- reset_n
		);
	end component amax10_qsys;

	u0 : component amax10_qsys
		port map (
			clk_clk                         => CONNECTED_TO_clk_clk,                         --                     clk.clk
			i2c_opencores_export_scl_pad_io => CONNECTED_TO_i2c_opencores_export_scl_pad_io, --    i2c_opencores_export.scl_pad_io
			i2c_opencores_export_sda_pad_io => CONNECTED_TO_i2c_opencores_export_sda_pad_io, --                        .sda_pad_io
			led_external_connection_export  => CONNECTED_TO_led_external_connection_export,  -- led_external_connection.export
			reset_reset_n                   => CONNECTED_TO_reset_reset_n                    --                   reset.reset_n
		);

