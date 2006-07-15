-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #041



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "a"

set_actor("ac-blackball", 10,3.5)

set_actor("ac-rotor", 10,9.5, {mouseforce=1, range=10, force=-100}) 

fill_floor("fl-floor_001",0,0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-actorimpulse", i-1,line)

      elseif c=="a" then

	 set_floor("fl-dunes", i-1,line)         

      elseif c=="b" then

	 set_floor("fl-space", i-1,line) 

      elseif c=="O" then

	 oxyd(i-1,line)         

        end

    end  

end

--               01234567890123456789

renderLine(00 , "#OO##############OO#")

renderLine(01 , "#                  #")

renderLine(02 , "# #              # #")

renderLine(03 , "#    aaaaaaaaaa    #")

renderLine(04 , "#    abbbbbbbba    #")  

renderLine(05 , "#    abbbbbbbba    #")

renderLine(06 , "#    abbbbbbbba    #")

renderLine(07 , "#    abbbbbbbba    #")

renderLine(08 , "#    abbbbbbbba    #")

renderLine(09 , "#    aaaaaaaaaa    #")

renderLine(10 , "O #              # O")

renderLine(11 , "O                  O")

renderLine(12 , "####################") 

--               01234567890123456789



oxyd_shuffle()


























