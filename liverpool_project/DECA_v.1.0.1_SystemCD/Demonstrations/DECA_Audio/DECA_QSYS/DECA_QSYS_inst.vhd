	component DECA_QSYS is
		port (
			audio_i2c_export_scl_pad_io              : inout std_logic                    := 'X';             -- scl_pad_io
			audio_i2c_export_sda_pad_io              : inout std_logic                    := 'X';             -- sda_pad_io
			audio_reset_n_external_connection_export : out   std_logic;                                       -- export
			clk_clk                                  : in    std_logic                    := 'X';             -- clk
			pio_key_external_connection_export       : in    std_logic_vector(1 downto 0) := (others => 'X'); -- export
			pio_led_external_connection_export       : out   std_logic_vector(7 downto 0);                    -- export
			pio_sw_external_connection_export        : in    std_logic_vector(1 downto 0) := (others => 'X'); -- export
			pll_audio_clk                            : out   std_logic;                                       -- clk
			reset_reset_n                            : in    std_logic                    := 'X'              -- reset_n
		);
	end component DECA_QSYS;

	u0 : component DECA_QSYS
		port map (
			audio_i2c_export_scl_pad_io              => CONNECTED_TO_audio_i2c_export_scl_pad_io,              --                  audio_i2c_export.scl_pad_io
			audio_i2c_export_sda_pad_io              => CONNECTED_TO_audio_i2c_export_sda_pad_io,              --                                  .sda_pad_io
			audio_reset_n_external_connection_export => CONNECTED_TO_audio_reset_n_external_connection_export, -- audio_reset_n_external_connection.export
			clk_clk                                  => CONNECTED_TO_clk_clk,                                  --                               clk.clk
			pio_key_external_connection_export       => CONNECTED_TO_pio_key_external_connection_export,       --       pio_key_external_connection.export
			pio_led_external_connection_export       => CONNECTED_TO_pio_led_external_connection_export,       --       pio_led_external_connection.export
			pio_sw_external_connection_export        => CONNECTED_TO_pio_sw_external_connection_export,        --        pio_sw_external_connection.export
			pll_audio_clk                            => CONNECTED_TO_pll_audio_clk,                            --                         pll_audio.clk
			reset_reset_n                            => CONNECTED_TO_reset_reset_n                             --                             reset.reset_n
		);

