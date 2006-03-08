-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #017



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.



set_actor("ac-blackball", 1.5,1.5)



fill_floor("fl-bluegreenx",0,0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-door_b", i-1,line)

      elseif c=="D" then

	 set_stone("st-death",i-1,line)         

      elseif c=="F" then

	 set_item("it-flagblack", i-1,line)  

      elseif c=="N" then

	 document(i-1,line,"Please remember:    42")

      elseif c=="O" then

	 oxyd(i-1,line)

        end

    end  

end

--               01234567890123456789

renderLine(00 , "####################")

renderLine(01 , "#F       DN        #")

renderLine(02 , "#######O #######D  #")

renderLine(03 , "#        #      #  #")

renderLine(04 , "#  D###### O#D  #  #")  

renderLine(05 , "#  #     #   #  #  #")

renderLine(06 , "#  #  O  ##D #  #  #")

renderLine(07 , "#  #  #  #   #  #  #")

renderLine(08 , "#  #  #  # ###  #  #")

renderLine(09 , "#  O  #  #   #  #  #")

renderLine(10 , "#     #  D#O D  O  #")

renderLine(11 , "#     #      #     #")

renderLine(12 , "####################")

--               01234567890123456789

oxyd_shuffle()











