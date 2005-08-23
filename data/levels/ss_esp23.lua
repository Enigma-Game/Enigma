-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #023



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "d"       -- Default flavor for oxyd stones.



set_actor("ac-blackball", 10,6.5)

fill_floor( "fl-bluegreenx", 0, 0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-rock3_break", i-1,line)

      elseif c=="H" then

         hammer(i-1,line)

      elseif c=="B" then                                                                

         set_stone("st-actorimpulse", i-1,  line)

      elseif c=="O" then

	 oxyd(i-1,line)

        end

    end  

end

--               01234567890123456789

renderLine(00 , "BBBBBBBBBBBBBBBBBBBB")

renderLine(01 , "BO################OB")

renderLine(02 , "B#B#B#B#B##B#B#B#B#B")

renderLine(03 , "B##################B")

renderLine(04 , "B#B#B#B#B##B#B#B#B#B")  

renderLine(05 , "B##################B")

renderLine(06 , "B#B#B#B##H ##B#B#B#B")

renderLine(07 , "B##################B")

renderLine(08 , "B#B#B#B#B##B#B#B#B#B")

renderLine(09 , "B##################B")

renderLine(10 , "B#B#B#B#B##B#B#B#B#B")

renderLine(11 , "BO################OB")

renderLine(12 , "BBBBBBBBBBBBBBBBBBBB")

--               01234567890123456789

oxyd_shuffle()

