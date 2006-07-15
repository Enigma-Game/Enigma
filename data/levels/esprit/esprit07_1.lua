-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #007

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.

set_actor("ac-blackball", 2.5,6.5)

fill_floor("fl-ice_001")
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==     "#" then
         set_stone("st-glass",i-1,line)
      elseif c=="L" then
	 set_item("it-extralife",i-1,line)
      elseif c=="D" then
	 set_stone("st-death",i-1,line)
      elseif c=="B" then
	 set_stone("st-actorimpulse",i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)
      elseif c=="C" then
	 set_item("it-crack2", i-1,line)
      elseif c=="c" then
	 set_item("it-crack1", i-1,line)
      elseif c=="N" then
	 document(i-1,line,"Watch out, slippery!")
      elseif c=="F" then
	 set_item("it-flagblack", i-1,line)
        end
    end
end
--              01234567890123456789
renderLine(00, "                    ")
renderLine(01, "####################")
renderLine(02, "#L                 #")
renderLine(03, "# D  B B BB B B  D #")
renderLine(04, "#                  #")  
renderLine(05, "#      O    O   C  #")
renderLine(06, "# cN B B BB B B    #")
renderLine(07, "#      O    O      #")
renderLine(08, "#                  #")
renderLine(09, "# D  B B BB B B cD #")
renderLine(10, "#F                 #")
renderLine(11, "####################")
renderLine(12, "                    ")
--              01234567890123456789
oxyd_shuffle()











