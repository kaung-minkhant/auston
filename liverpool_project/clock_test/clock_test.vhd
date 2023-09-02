library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity clock_test is
    port (
        ADC_CLK_10: in std_logic;
        MAX10_CLK1_50: in std_logic;
        MAX10_CLK2_50: in std_logic;
        LED : buffer std_logic_vector(7 downto 0);
        SW: in std_logic_vector(1 downto 0)
    );
end clock_test;

architecture rtl of clock_test is
    signal clock_divider: unsigned(31 downto 0) := (others => '0');
begin
    CLOCK_DIVIDER_PROC : process(MAX10_CLK1_50)
    begin
        if (MAX10_CLK1_50'event and MAX10_CLK1_50='1') then
            clock_divider <= clock_divider + 1;
        end if;
    end process;
    LED(7 downto 1) <= (others => '1');
    LED(0) <= clock_divider(24);
end architecture;