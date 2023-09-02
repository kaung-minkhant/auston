	component DECA_QSYS is
		port (
			clk_clk                            : in    std_logic                    := 'X';             -- clk
			pio_key_external_connection_export : in    std_logic_vector(1 downto 0) := (others => 'X'); -- export
			pio_led_external_connection_export : out   std_logic_vector(7 downto 0);                    -- export
			pio_sw_external_connection_export  : in    std_logic_vector(1 downto 0) := (others => 'X'); -- export
			reset_reset_n                      : in    std_logic                    := 'X';             -- reset_n
			i2c_opencores_0_export_scl_pad_io  : inout std_logic                    := 'X';             -- scl_pad_io
			i2c_opencores_0_export_sda_pad_io  : inout std_logic                    := 'X'              -- sda_pad_io
		);
	end component DECA_QSYS;

	u0 : component DECA_QSYS
		port map (
			clk_clk                            => CONNECTED_TO_clk_clk,                            --                         clk.clk
			pio_key_external_connection_export => CONNECTED_TO_pio_key_external_connection_export, -- pio_key_external_connection.export
			pio_led_external_connection_export => CONNECTED_TO_pio_led_external_connection_export, -- pio_led_external_connection.export
			pio_sw_external_connection_export  => CONNECTED_TO_pio_sw_external_connection_export,  --  pio_sw_external_connection.export
			reset_reset_n                      => CONNECTED_TO_reset_reset_n,                      --                       reset.reset_n
			i2c_opencores_0_export_scl_pad_io  => CONNECTED_TO_i2c_opencores_0_export_scl_pad_io,  --      i2c_opencores_0_export.scl_pad_io
			i2c_opencores_0_export_sda_pad_io  => CONNECTED_TO_i2c_opencores_0_export_sda_pad_io   --                            .sda_pad_io
		);

