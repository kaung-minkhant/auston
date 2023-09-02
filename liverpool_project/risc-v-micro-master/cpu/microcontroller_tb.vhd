library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity microcontroller_tb is
end microcontroller_tb;

architecture sim of microcontroller_tb is

    constant clk_hz : integer := 100e6;
    constant clk_period : time := 1 us;  -- 1_000_000
    constant clk_main_period: time := 20 ns; -- 50_000_000

    signal clk : std_logic := '0';
    signal clk_main: std_logic := '0';
    signal reset_n : std_logic := '0';

    component microcontroller is
        generic (
            FREQ : integer := 50_000_000
        );
        port (
            clk, clk_main,  reset_n : in std_logic;
    
            general_io : inout std_logic_vector(15 downto 0);
            special_io : inout std_logic_vector(15 downto 0)
        );
    end component;
    
    signal general_io, special_io : std_logic_vector(15 downto 0) := (others => '0');

begin

    clk <= not clk after clk_period / 2;
    clk_main <= not clk_main after clk_main_period / 2;
    reset_n <= '1' after 5*clk_period;
    general_io(4) <= '1';

    DUT: microcontroller
    generic map (
        FREQ => 50_000_000
    )
    port map(
        clk => clk, reset_n => reset_n, clk_main => clk_main,
        general_io => general_io, special_io => special_io
    );


end architecture;