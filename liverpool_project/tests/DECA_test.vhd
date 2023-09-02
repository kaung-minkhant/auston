library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity DECA_test is
    port (
        LED: out std_logic_vector(7 downto 0);
        SW: in std_logic_vector(1 downto 0)
    );
end DECA_test;

architecture rtl of DECA_test is

begin
    LED(7 downto 1) <= "1111111";
    LED(0) <= not(SW(0) and SW(1));

end architecture;