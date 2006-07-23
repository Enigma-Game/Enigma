-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #008

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.

set_actor("ac-blackball", 10,6.5)

fill_floor("fl-metal")
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-actorimpulse",i-1,line)
      elseif c=="S" then
         set_item("it-umbrella", i-1,line)
      elseif c=="D" then
	 set_stone("st-death",i-1,line)
      elseif c=="W" then
         set_stone("st-rock2",i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)
      elseif c=="1" then
         set_stone("st-oneway_black-w",i-1,line)
      elseif c=="2" then
         set_stone("st-oneway_black-e",i-1,line)
      elseif c=="N" then
	 document(i-1,line,"Umbrellas protect!")
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#D       SS       D#")
renderLine(02, "#                  #")
renderLine(03, "#  OWWWOWWWWOWWWO  #")
renderLine(04, "#  W            W  #") 
renderLine(05, "#  W            W  #")  
renderLine(06, "#S 1N          N2 S#")
renderLine(07, "#  W            W  #")
renderLine(08, "#  W            W  #") 
renderLine(09, "#  OWWWOWWWWOWWWO  #")
renderLine(10, "#                  #")
renderLine(11, "#D       SS       D#")
renderLine(12, "####################")
--              01234567890123456789
oxyd_shuffle()












