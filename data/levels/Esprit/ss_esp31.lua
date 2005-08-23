-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #031



levelw = 20

levelh = 13

create_world(levelw, levelh)

enigma.ConserveLevel = FALSE

oxyd_default_flavor = "b"

set_actor("ac-blackball", 10,5.5, {player=0})

set_actor("ac-rotor", 10,11.5, {range=25, force=50})

fill_floor("fl-samba",0,0,levelw,levelh)

function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-bigbrick", i-1,line)

      elseif c=="." then

         set_floor("fl-leaves", i-1,line)

      elseif c=="O" then

	 oxyd(i-1,line)

        end

    end  

end

--               01234567890123456789

renderLine(00 , "####################")

renderLine(01 , "#.   .   .   .   . #")

renderLine(02 , "# . . . O . . . . .#")

renderLine(03 , "#  .O  .   .   .   #")

renderLine(04 , "#.   .   .   O   . #")  

renderLine(05 , "# . . . . . . . . .#")

renderLine(06 , "#  .   .   .   .   #")

renderLine(07 , "#.   .   .   .   . #")

renderLine(08 , "# . .O. . . . . .O.#")

renderLine(09 , "#  .   .   .   .   #")

renderLine(10 , "#.   .   .  O.   . #")

renderLine(11 , "# . . . . . . . . .#")

renderLine(12 , "####################")

--               01234567890123456789



oxyd_shuffle()

