-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #021



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "a"       -- Default flavor for oxyd stones.



set_actor("ac-blackball", 10,6.5)

fill_floor( "fl-bluegreenx", 0, 0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-bluegray", i-1,line)

      elseif c=="H" then

         set_stone("st-bluegray_hole",i-1,line)

      elseif c=="." then

         set_floor("fl-abyss",i-1,line)         

      elseif c=="G" then                                                                

         set_stone("st-grate1", i-1,  line)

      elseif c=="N" then                   

         document(i-1,  line,"OOPS!" )

      elseif c=="O" then

	 oxyd(i-1,line)

        end

    end  

end

--               01234567890123456789

renderLine(00 , "###..............###")

renderLine(01 , "#OG####......####GO#")

renderLine(02 , "#GGG  O######O  GGG#")

renderLine(03 , ".#G              G#.")

renderLine(04 , "..#              #..")  

renderLine(05 , "..H              H..")

renderLine(06 , "..HN             H..")

renderLine(07 , "..H              H..")

renderLine(08 , "..#              #..")

renderLine(09 , ".#G              G#.")

renderLine(10 , "#GGG  O######O  GGG#")

renderLine(11 , "#OG####......####GO#")

renderLine(12 , "###..............###")

--               01234567890123456789

oxyd_shuffle()











