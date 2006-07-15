-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #93



levelw = 20

levelh = 13



create_world(levelw, levelh)

oxyd_default_flavor = "b"

set_actor("ac-blackball", 10,6.5)

fill_floor("fl-space", 0,0, levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "#" then

         set_stone("st-invisible", i-1,line)

      elseif c =="D" then

         set_stone("st-death",i-1,line)

      elseif c =="d" then

         set_stone("st-death_invisible",i-1,line)

      elseif c =="f" then

         set_floor("fl-black",i-1,line)

      elseif c=="O" then                                                               

	 oxyd(i-1,line) 

        end

    end  

end

--               01234567890123456789

renderLine(00 , "####################")

renderLine(01 , "#                  #")

renderLine(02 , "#  d d        d d  #")

renderLine(03 , "#ffDfDffffffffDfDff#")

renderLine(04 , "#                  #")  

renderLine(05 , "#                  #")

renderLine(06 , "# O O O DffD O O O #")

renderLine(07 , "#                  #")

renderLine(08 , "#                  #")

renderLine(09 , "#ffDfDffffffffDfDff#")

renderLine(10 , "#  d d        d d  #")

renderLine(11 , "#                  #")

renderLine(12 , "####################") 

--               01234567890123456789



oxyd_shuffle()










