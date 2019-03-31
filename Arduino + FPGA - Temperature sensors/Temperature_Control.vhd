----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/28/2018 07:18:45 PM
-- Design Name: 
-- Module Name: Temperature_Control - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
--use IEEE.STD_LOGIC_ARITH.ALL;
use ieee.numeric_std.all; 


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Temperature_Control is
    Port ( 
         --  Read : in STD_LOGIC;
         --  Send : in STD_LOGIC;
           clock_100Mhz: in std_logic;
           reset: in std_logic;
           Recieved_Value : in STD_LOGIC_VECTOR (0 to 9);
           Transmitted_Value : out STD_LOGIC_VECTOR (0 to 9);
           Anode_Activate : out STD_LOGIC_VECTOR (3 downto 0);-- 4 Anode signals            
           LED_out : out STD_LOGIC_VECTOR (6 downto 0)-- Cathode patterns of 7-s end seven_segment_display_VHDL; 
           );
end Temperature_Control;

architecture Behavioral of Temperature_Control is

signal display_number: std_logic_vector(15 downto 0);
signal value_to_display: std_logic_vector(15 downto 0) := "0000000000000000";
signal value_to_send: std_logic_vector(9 downto 0);
signal aux5000: std_logic_vector(15 downto 0) := "0001001110001000";  -- 5000 volts
signal aux1024: std_logic_vector(15 downto 0) := "0000010000000000"; -- 1024
signal aux1: std_logic_vector(15 downto 0):= "0000000000000000";
signal milivolts: std_logic_vector(15 downto 0):= "0000000000000000";

signal auxIntRecieved: integer range 0 to 9999;
signal auxInt5000: integer range 0 to 9999;
signal auxInt1024: integer range 0 to 2048;
signal auxInt10: integer range 0 to 255;
signal auxIntMilivolts: integer range 0 to 9999;
signal auxResult : integer range 0 to 9999;


component seven_segment_display
Port ( clock_100Mhz : in STD_LOGIC;-- 100Mhz clock on Basys 3 FPGA board
            display_number: in STD_LOGIC_VECTOR(15 downto 0);
            reset : in STD_LOGIC; -- reset
            Anode_Activate : out STD_LOGIC_VECTOR (3 downto 0);-- 4 Anode signals            
            LED_out : out STD_LOGIC_VECTOR (6 downto 0));-- Cathode patterns of 7-s end seven_segment_display_VHDL; 
end component;


begin

Display: seven_segment_display port map(clock_100Mhz, value_to_display , reset, Anode_Activate, LED_out);
-- Display
--value_to_display <= "000000" & Recieved_Value;
process(Recieved_value)
    begin
      -- aux<= "0000000000000100"; -- 5000/1024
      --aux1 <= ((unsigned) aux5000) / ((unsigned) aux1024);
     --  milivolts <= ("000000" & Recieved_value) * aux1;
     --  milivolts <= "000" & milivolts;
      -- milivolts <= milivolts 
      auxIntRecieved <= to_integer(unsigned("000000" & Recieved_Value));
    --  auxInt5000 <= to_integer(unsigned(aux5000));
    --  auxInt1024 <= to_integer(unsigned(aux1024));
    --  auxIntMilivolts <= auxIntRecieved * (auxInt5000 / auxInt1024);
       auxIntMilivolts <= auxIntRecieved * (5000 / 1024);
      auxResult <= (auxIntRecieved - 500) / 10;
      
       --value_to_display <= "000000" & Recieved_Value;
       value_to_display <= std_logic_vector(to_unsigned(auxResult, 16));
       
       Transmitted_Value <= value_to_display(9 downto 0);
      -- value_to_display <= "0001000100010001";
end process;

--process(Read,Send)
--begin
--    if(Read = '1') then
        
--    end if;
--    if(Send = '1') then
--        Transmitted_value <= "1111111111";
--    end if;
--end process;
        
                      
end Behavioral;
