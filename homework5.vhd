-- ECE532 HW5
-- Zackary McClamma
-- 18-Nov-2019

library ieee;
  use ieee.std_logic_1164.all;

entity homework5 is
  port
  (
			i_clk         : in  std_logic := 'X'; -- clk
			i_reset_n     : in  std_logic := 'X'; -- reset_n
      i_uart_rxd      : in  std_logic := 'X'; -- rxd
      o_uart_txd      : out std_logic;         -- txd
      b_i2c_scl       : inout std_logic;
      b_i2c_sda       : inout std_logic;
      green_led	      : out std_logic;
      red_led         : out std_logic

  );
end homework5;

architecture sch of homework5 is

  component hw5_cpu is
	port (
        clk_clk           : in  std_logic := 'X'; -- clk
        reset_reset_n     : in  std_logic := 'X'; -- reset_n
				uart_rxd	: in  std_logic := 'X';
				uart_txd    : out std_logic;
        i2c_sda_in      : in std_logic;
        i2c_scl_in      : in std_logic;
        i2c_sda_oe      : out std_logic;
        i2c_scl_oe      : out std_logic;
        green_led_export : out std_logic;
        red_led_export   : out std_logic

        );
  end component hw5_cpu;

  signal w_i2c_sda_in     : std_logic;
  signal w_i2c_scl_in     : std_logic;
  signal w_i2c_sda_oe     : std_logic;
  signal w_i2c_scl_oe     : std_logic;

begin

    b_i2c_scl <= '0' when w_i2c_scl_oe = '1' else 'Z';
    b_i2c_sda <= '0' when w_i2c_sda_oe = '1' else 'Z';
    w_i2c_scl_in <= b_i2c_scl;
    w_i2c_sda_in <= b_i2c_sda;

    u0 : component hw5_cpu
        port map (
        clk_clk           => i_clk,           --        clk.clk
        reset_reset_n     => i_reset_n,     --      reset.reset_n
				uart_rxd    => i_uart_rxd,
				uart_txd   => o_uart_txd,
        green_led_export => green_led,
        red_led_export => red_led,
        i2c_sda_in => w_i2c_sda_in,
        i2c_scl_in => w_i2c_scl_in,
        i2c_sda_oe => w_i2c_sda_oe,
        i2c_scl_oe => w_i2c_scl_oe

        );
end sch;
