	component usb20sr_refdes is
		port (
			altpll_0_areset_conduit_export      : in    std_logic                    := 'X';             -- export
			altpll_0_locked_conduit_export      : out   std_logic;                                       -- export
			altpll_0_phasedone_conduit_export   : out   std_logic;                                       -- export
			osc_clk_clk                         : in    std_logic                    := 'X';             -- clk
			reset_reset_n                       : in    std_logic                    := 'X';             -- reset_n
			rst_pio_external_connection_export  : out   std_logic;                                       -- export
			u20_clk_out_clk                     : in    std_logic                    := 'X';             -- clk
			usb20sr_conduit_end_Data            : inout std_logic_vector(7 downto 0) := (others => 'X'); -- Data
			usb20sr_conduit_end_Stp             : out   std_logic;                                       -- Stp
			usb20sr_conduit_end_Dir             : in    std_logic                    := 'X';             -- Dir
			usb20sr_conduit_end_Nxt             : in    std_logic                    := 'X';             -- Nxt
			usb20sr_conduit_end_phy_clk         : in    std_logic                    := 'X';             -- phy_clk
			usb20sr_conduit_end_phy_reset_n     : out   std_logic;                                       -- phy_reset_n
			usb20sr_conduit_end_phy_cs_n        : out   std_logic;                                       -- phy_cs_n
			usb20sr_conduit_end_Ext_reset_in    : in    std_logic                    := 'X';             -- Ext_reset_in
			user_led_external_connection_export : out   std_logic_vector(7 downto 0)                     -- export
		);
	end component usb20sr_refdes;

	u0 : component usb20sr_refdes
		port map (
			altpll_0_areset_conduit_export      => CONNECTED_TO_altpll_0_areset_conduit_export,      --      altpll_0_areset_conduit.export
			altpll_0_locked_conduit_export      => CONNECTED_TO_altpll_0_locked_conduit_export,      --      altpll_0_locked_conduit.export
			altpll_0_phasedone_conduit_export   => CONNECTED_TO_altpll_0_phasedone_conduit_export,   --   altpll_0_phasedone_conduit.export
			osc_clk_clk                         => CONNECTED_TO_osc_clk_clk,                         --                      osc_clk.clk
			reset_reset_n                       => CONNECTED_TO_reset_reset_n,                       --                        reset.reset_n
			rst_pio_external_connection_export  => CONNECTED_TO_rst_pio_external_connection_export,  --  rst_pio_external_connection.export
			u20_clk_out_clk                     => CONNECTED_TO_u20_clk_out_clk,                     --                  u20_clk_out.clk
			usb20sr_conduit_end_Data            => CONNECTED_TO_usb20sr_conduit_end_Data,            --          usb20sr_conduit_end.Data
			usb20sr_conduit_end_Stp             => CONNECTED_TO_usb20sr_conduit_end_Stp,             --                             .Stp
			usb20sr_conduit_end_Dir             => CONNECTED_TO_usb20sr_conduit_end_Dir,             --                             .Dir
			usb20sr_conduit_end_Nxt             => CONNECTED_TO_usb20sr_conduit_end_Nxt,             --                             .Nxt
			usb20sr_conduit_end_phy_clk         => CONNECTED_TO_usb20sr_conduit_end_phy_clk,         --                             .phy_clk
			usb20sr_conduit_end_phy_reset_n     => CONNECTED_TO_usb20sr_conduit_end_phy_reset_n,     --                             .phy_reset_n
			usb20sr_conduit_end_phy_cs_n        => CONNECTED_TO_usb20sr_conduit_end_phy_cs_n,        --                             .phy_cs_n
			usb20sr_conduit_end_Ext_reset_in    => CONNECTED_TO_usb20sr_conduit_end_Ext_reset_in,    --                             .Ext_reset_in
			user_led_external_connection_export => CONNECTED_TO_user_led_external_connection_export  -- user_led_external_connection.export
		);

