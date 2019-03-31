----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/28/2018 12:06:49 PM
-- Design Name: 
-- Module Name: seven_segment_display - Behavioral
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
use IEEE.std_logic_unsigned.all;
use ieee.numeric_std.all; 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity seven_segment_display is
Port ( clock_100Mhz : in STD_LOGIC;-- 100Mhz clock on Basys 3 FPGA board
            display_number: in STD_LOGIC_VECTOR(15 downto 0);
            reset : in STD_LOGIC; -- reset
            Anode_Activate : out STD_LOGIC_VECTOR (3 downto 0);-- 4 Anode signals            
            LED_out : out STD_LOGIC_VECTOR (6 downto 0));-- Cathode patterns of 7-s end seven_segment_display_VHDL; 
end seven_segment_display;

architecture Behavioral of seven_segment_display is 
    signal one_second_counter: STD_LOGIC_VECTOR (27 downto 0); -- counter for generating 1-second clock enable 
    signal one_second_enable: std_logic; -- one second enable for counting numbers 
    signal displayed_number: STD_LOGIC_VECTOR (15 downto 0); -- counting decimal number to be displayed on 4-digit 7-segment display 
    signal LED_BCD: STD_LOGIC_VECTOR (3 downto 0);
    signal refresh_counter: STD_LOGIC_VECTOR (19 downto 0); -- creating 10.5ms refresh period 
    signal LED_activating_counter: std_logic_vector(1 downto 0); -- the other 2-bit for creating 4 LED-activating signals 
    signal intRecieved : integer range 0 to 9999;
    signal digit1: std_logic_vector(3 downto 0);
    signal digit2: std_logic_vector(3 downto 0);
    signal digit3: std_logic_vector(3 downto 0);
    signal digit4: std_logic_vector(3 downto 0);
    
    -- count         0    ->  1  ->  2  ->  3 
    -- activates    LED1    LED2   LED3   LED4 
    -- and repeat 
    function fIntToStringLeading0 (a : natural; d : integer range 1 to 9) return string is
      variable vString : string(1 to d);
    begin
      if(a >= 10**d) then
        return integer'image(a);
      else
        for i in 0 to d-1 loop
          vString(d-i to d-i) := integer'image(a/(10**i) mod 10);
        end loop;
        return vString;
      end if;
    end function;
    
    begin -- VHDL code for BCD to 7-segment decoder 
    -- Cathode patterns of the 7-segment LED display  
--    process(display_number)
--     variable intt : integer range 0 to 9999 := to_integer(unsigned(displayed_number));
--     variable number : string := integer'image(intt);
--     variable in1 : string := "5";
--   --  variable in2 : string := 
--   --  variable in3 : string :=
--   --  variable in4 : string := 
--        begin
--        --    intRecieved <= to_integer(unsigned(display_number));
--           --number := integer'image(intt);
--          --  number := fIntToStringLeading0(intt, 4);
--            digit1 <=  std_logic_vector(to_unsigned(integer'value(in1),4));
--            digit2 <=  std_logic_vector(to_unsigned(integer'value("5"),4));
--            digit3 <=  std_logic_vector(to_unsigned(integer'value("3"),4));
--            digit4 <=  std_logic_vector(to_unsigned(integer'value("2"),4));
--            displayed_number <= digit1 & digit2 & digit3 & digit4;
--    end process;
    process(LED_BCD) 
        begin     
            case LED_BCD is    
                 when "0000" => LED_out <= "0000001"; -- "0"          
                 when "0001" => LED_out <= "1001111"; -- "1"      
                 when "0010" => LED_out <= "0010010"; -- "2"      
                 when "0011" => LED_out <= "0000110"; -- "3"      
                 when "0100" => LED_out <= "1001100"; -- "4"      
                 when "0101" => LED_out <= "0100100"; -- "5"      
                 when "0110" => LED_out <= "0100000"; -- "6"      
                 when "0111" => LED_out <= "0001111"; -- "7"      
                 when "1000" => LED_out <= "0000000"; -- "8"      
                 when "1001" => LED_out <= "0000100"; -- "9"      
                 when others => LED_out <= "0000000";   
                 end case; 
      end process; -- 7-segment display controller 
      -- generate refresh period of 10.5ms 
      process(clock_100Mhz,reset) 
            begin      
                if(reset='1') then         
                    refresh_counter <= (others => '0');    
                elsif(rising_edge(clock_100Mhz)) then         
                    refresh_counter <= refresh_counter + 1;     
                end if; 
      end process;  
      LED_activating_counter <= refresh_counter(19 downto 18); 
      -- 4-to-1 MUX to generate anode activating signals for 4 LEDs  
      
      displayed_number <= display_number;
      process(LED_activating_counter, display_number) 
        begin     
             case LED_activating_counter is     
                when "00" =>         
                    Anode_Activate <= "0111";          -- activate LED1 and Deactivate LED2, LED3, LED4         
                    LED_BCD <= display_number(15 downto 12);         -- the first hex digit of the 16-bit number     
                when "01" =>
                        Anode_Activate <= "1011";          -- activate LED2 and Deactivate LED1, LED3, LED4         
                        LED_BCD <= display_number(11 downto 8);         -- the second hex digit of the 16-bit number     
                when "10" =>         
                        Anode_Activate <= "1101";          -- activate LED3 and Deactivate LED2, LED1, LED4        
                        LED_BCD <= display_number(7 downto 4);         -- the third hex digit of the 16-bit number     
                when "11" =>         
                        Anode_Activate <= "1110";          -- activate LED4 and Deactivate LED2, LED3, LED1         
                        LED_BCD <= display_number(3 downto 0);         -- the fourth hex digit of the 16-bit number         
             end case; 
      end process; 
      

end Behavioral;

