-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #025



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "b"

set_actor("ac-blackball", 4,6.5)

fill_floor( "fl-brick", 0, 0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-marble", i-1,line)

      elseif c=="S" then

         set_item("it-umbrella",i-1,line)

      elseif c=="D" then                                                                

         set_stone("st-death", i-1,  line)

      elseif c=="G" then                                                                

         set_stone("st-grate2",i-1,line)

      elseif c=="O" then

	 oxyd(i-1,line)

        end

    end  

end

--               01234567890123456789

renderLine(00 , "####################")

renderLine(01 , "#      S ##        #")

renderLine(02 , "# O      ##      O #")

renderLine(03 , "#    O   ##   O    #")

renderLine(04 , "#        ##        #")  

renderLine(05 , "#    DDDDDDDDDD    #")

renderLine(06 , "#    GGGGGGGGGG    #")

renderLine(07 , "#    DDDDDDDDDD    #")

renderLine(08 , "#        ##        #")

renderLine(09 , "#    O   ##   O    #")

renderLine(10 , "# O      ##      O #")

renderLine(11 , "#        ##S       #")

renderLine(12 , "####################")

--               01234567890123456789



oxyd_shuffle()

