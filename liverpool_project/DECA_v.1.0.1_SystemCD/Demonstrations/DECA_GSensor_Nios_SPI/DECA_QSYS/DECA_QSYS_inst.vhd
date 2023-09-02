	component DECA_QSYS is
		port (
			clk_clk                            : in  std_logic                    := 'X';             -- clk
			pio_key_external_connection_export : in  std_logic_vector(1 downto 0) := (others => 'X'); -- export
			pio_led_external_connection_export : out std_logic_vector(7 downto 0);                    -- export
			pio_sw_external_connection_export  : in  std_logic_vector(1 downto 0) := (others => 'X'); -- export
			reset_reset_n                      : in  std_logic                    := 'X';             -- reset_n
			spi_gsensor_external_MISO          : in  std_logic                    := 'X';             -- MISO
			spi_gsensor_external_MOSI          : out std_logic;                                       -- MOSI
			spi_gsensor_external_SCLK          : out std_logic;                                       -- SCLK
			spi_gsensor_external_SS_n          : out std_logic                                        -- SS_n
		);
	end component DECA_QSYS;

	u0 : component DECA_QSYS
		port map (
			clk_clk                            => CONNECTED_TO_clk_clk,                            --                         clk.clk
			pio_key_external_connection_export => CONNECTED_TO_pio_key_external_connection_export, -- pio_key_external_connection.export
			pio_led_external_connection_export => CONNECTED_TO_pio_led_external_connection_export, -- pio_led_external_connection.export
			pio_sw_external_connection_export  => CONNECTED_TO_pio_sw_external_connection_export,  --  pio_sw_external_connection.export
			reset_reset_n                      => CONNECTED_TO_reset_reset_n,                      --                       reset.reset_n
			spi_gsensor_external_MISO          => CONNECTED_TO_spi_gsensor_external_MISO,          --        spi_gsensor_external.MISO
			spi_gsensor_external_MOSI          => CONNECTED_TO_spi_gsensor_external_MOSI,          --                            .MOSI
			spi_gsensor_external_SCLK          => CONNECTED_TO_spi_gsensor_external_SCLK,          --                            .SCLK
			spi_gsensor_external_SS_n          => CONNECTED_TO_spi_gsensor_external_SS_n           --                            .SS_n
		);

