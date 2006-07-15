-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #076



levelw = 20

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "b"

set_actor("ac-blackball", 10,2.5)

fill_floor("fl-black",0,0,levelw,levelh)



function renderLine( line, pattern)

    for i=1, strlen(pattern) do

      local c = strsub( pattern, i, i)

      if c ==   "G" then

         set_stone("st-grate2", i-1,line)

      elseif c=="D" then

	 set_stone("st-death",i-1,line)

      elseif c=="F" then

	 set_stone("st-fakeoxyd",i-1,line)	 

      elseif c=="U" then
	 set_item("it-umbrella",i-1,line)		 

      elseif c=="B" then                                                                

	 set_stone("st-actorimpulse", i-1,line)

      elseif c=="O" then

	 oxyd(i-1,line)         

        end

    end  

end

--               01234567890123456789

renderLine(00 , "DDDDDDDDDDDDDDDDDDDD")

renderLine(01 , "DOGGGGOGGGGGGFGGGGOD")

renderLine(02 , "DG                GD")

renderLine(03 , "DG BBBB BBBB BBBB GD")

renderLine(04 , "DG B            B GD")  

renderLine(05 , "DG B U B UU B U B GD")

renderLine(06 , "DG B U   UU   U B GD")

renderLine(07 , "DG B U B UU B U B GD")

renderLine(08 , "DG B            B GD")

renderLine(09 , "DG BBBB BBBB BBBB GD")

renderLine(10 , "DG                GD")

renderLine(11 , "DOGGGGOGGGGGGFGGGGOD")

renderLine(12 , "DDDDDDDDDDDDDDDDDDDD") 

--               01234567890123456789



oxyd_shuffle()


























