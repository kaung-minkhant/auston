library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

entity instruction_memory is
  generic (
    DATA_WIDTH : integer := 32
    );
  port (
    -- byte address
    instruction_address : in std_logic_vector(DATA_WIDTH - 1 downto 0);

    -- instruction
    instruction : out std_logic_vector(DATA_WIDTH - 1 downto 0);

    -- clock and resets
    clk : in std_logic;
    reset_n : in std_logic
    );
end instruction_memory;

architecture rtl of instruction_memory is
  component address_decoder is
    generic (
      DATA_WIDTH : integer := 32
      );
    port (
      byte_address : in std_logic_vector(DATA_WIDTH - 1 downto 0);
      double_word_address : out std_logic_vector(DATA_WIDTH - 1 downto 0)
      );
  end component;

  -- (2 ** DATA_WIDTH) * 8 - 1

  type INSTRUCTION_t is array (0 to 3) of std_logic_vector(7 downto 0);
  type INSTRUCTION_ARRAY_t is array (0 to 2**8 - 1) of INSTRUCTION_t;
  signal instruction_array : INSTRUCTION_ARRAY_t := (others => (others => (others => '0')));
  signal doubleword_address : std_logic_vector(DATA_WIDTH - 1 downto 0) := (others => '0');

begin
  instruction_array(0)(0) <= "10010011";
  instruction_array(0)(1) <= "00000000";
  instruction_array(0)(2) <= "00110000";
  instruction_array(0)(3) <= "00000000";
  instruction_array(1)(0) <= "00010011";
  instruction_array(1)(1) <= "00000001";
  instruction_array(1)(2) <= "00010000";
  instruction_array(1)(3) <= "00000000";
  instruction_array(2)(0) <= "10010011";
  instruction_array(2)(1) <= "00000001";
  instruction_array(2)(2) <= "00010000";
  instruction_array(2)(3) <= "00000000";
  instruction_array(3)(0) <= "10010011";
  instruction_array(3)(1) <= "10000000";
  instruction_array(3)(2) <= "11100000";
  instruction_array(3)(3) <= "11111111";
  instruction_array(4)(0) <= "00110011";
  instruction_array(4)(1) <= "00000010";
  instruction_array(4)(2) <= "00110001";
  instruction_array(4)(3) <= "00000000";
  instruction_array(5)(0) <= "00110011";
  instruction_array(5)(1) <= "00000001";
  instruction_array(5)(2) <= "00110000";
  instruction_array(5)(3) <= "00000000";
  instruction_array(6)(0) <= "10110011";
  instruction_array(6)(1) <= "00000001";
  instruction_array(6)(2) <= "01000000";
  instruction_array(6)(3) <= "00000000";
  instruction_array(7)(0) <= "10010011";
  instruction_array(7)(1) <= "10000000";
  instruction_array(7)(2) <= "11110000";
  instruction_array(7)(3) <= "11111111";
  instruction_array(8)(0) <= "11100011";
  instruction_array(8)(1) <= "10011000";
  instruction_array(8)(2) <= "00000000";
  instruction_array(8)(3) <= "11111110";
  
  address_decoder_u : address_decoder
    generic map(
      DATA_WIDTH => DATA_WIDTH
      ) port map (
        byte_address => instruction_address, double_word_address => doubleword_address
        );
  instruction <= instruction_array(to_integer(unsigned(doubleword_address)))(3) &
                 instruction_array(to_integer(unsigned(doubleword_address)))(2) &
                 instruction_array(to_integer(unsigned(doubleword_address)))(1) &
                 instruction_array(to_integer(unsigned(doubleword_address)))(0);
end architecture;
