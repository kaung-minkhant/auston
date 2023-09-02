library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cpu_pipelined_tb is
end cpu_pipelined_tb;

architecture sim of cpu_pipelined_tb is

    component cpu_pipelined IS
        GENERIC (
            DATA_WIDTH : INTEGER := 32
        );
        PORT (
          general_pin_dir: out std_logic_vector(15 downto 0);
            general_pin_write: out std_logic_vector(15 downto 0);
            general_pin_read: in std_logic_vector(15 downto 0);

            -- uart1
            uart1_flags: in std_logic_vector(7 downto 0);
            uart1_controls: out std_logic_vector(7 downto 0);
            uart1_data_write : out std_logic_vector(31 downto 0); 
            uart1_data_read : in std_logic_vector(7 downto 0);
            uart1_data_32_read : in std_logic_vector(31 downto 0);

            -- spi1
            spi1_flags: in std_logic_vector(7 downto 0);
            spi1_controls: out std_logic_vector(7 downto 0);
            spi1_data_write: out std_logic_vector(7 downto 0);
            spi1_data_read: in std_logic_vector(7 downto 0);

            -- i2c1
            i2c1_flags : in std_logic_vector(7 downto 0);
            i2c1_controls: out std_logic_vector(7 downto 0);
            i2c1_data_read : in std_logic_vector(7 downto 0);
            i2c1_data_write : out std_logic_vector(7 downto 0);
            i2c1_addr_write: out std_logic_vector(6 downto 0);

            reset_n, clk, clk_main: std_logic
        );
    END component;
    constant CLK_PERIOD : time := 300 ps;
    constant CLK_MAIN_PERIOD : time := 10 ps;
    constant CLK_HALF_PERIOD : time :=  CLK_PERIOD / 2;
    constant CLK_MAIN_HALF_PERIOD : time :=  CLK_MAIN_PERIOD / 2;

    signal clk, clk_main : std_logic := '0';
    signal reset_n : std_logic := '0';

    signal general_pin_dir, general_pin_write, general_pin_read: std_logic_vector(15 downto 0) := (others => '0');

    -- uart1
    signal uart1_flags, uart1_controls, uart1_data_read: std_logic_vector(7 downto 0) := (others => '0');
    signal uart1_data_write, uart1_data_32_read : std_logic_vector(31 downto 0) := (others => '0');

    -- spi1
    signal spi1_flags, spi1_controls, spi1_data_write, spi1_data_read: std_logic_vector(7 downto 0) := (others => '0');

    -- i2c1
    signal i2c1_flags, i2c1_controls, i2c1_data_read, i2c1_data_write : std_logic_vector(7 downto 0) := (others => '0'); 
    signal i2c1_addr_write: std_logic_vector(6 downto 0) := (others => '0');

    constant DATA_WIDTH : integer := 32;

begin

    clk <= not clk after CLK_HALF_PERIOD;
    clk_main <= not clk_main after CLK_MAIN_HALF_PERIOD;

    reset_n <= '1' after 5*CLK_PERIOD;

    DUT : entity work.cpu_pipelined(rtl)
    generic map (
        DATA_WIDTH =>  DATA_WIDTH
    )
    port map (
        clk => clk,
        clk_main => clk_main,
        reset_n => reset_n,
        -- data memory exports
        general_pin_dir => general_pin_dir,
        general_pin_write => general_pin_write,
        general_pin_read => general_pin_read,

        -- uart1
        uart1_flags => uart1_flags,
        uart1_controls => uart1_controls,
        uart1_data_write => uart1_data_write,
        uart1_data_read => uart1_data_read,
        uart1_data_32_read => uart1_data_32_read,

        -- spi1
        spi1_flags => spi1_flags,
        spi1_controls => spi1_controls,
        spi1_data_write => spi1_data_write,
        spi1_data_read => spi1_data_read,

        -- i2c1
        i2c1_flags => i2c1_flags,
        i2c1_controls => i2c1_controls,
        i2c1_data_read => i2c1_data_read,
        i2c1_data_write => i2c1_data_write,
        i2c1_addr_write => i2c1_addr_write
    );

end architecture;
