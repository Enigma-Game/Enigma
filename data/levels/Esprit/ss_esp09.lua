-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #009

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.

set_actor("ac-blackball", 10.5,6.5, {player=0})

fill_floor("fl-normal", 0,0, 15,level_height)
fill_floor("fl-gray", 16,0, level_width,level_height)
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-actorimpulse",i-1,line)
      elseif c=="S" then
         set_stone("st-grate2",i-1,line)
      elseif c=="G" then
	 set_floor("fl-gradient",  i-1,  line, {type=3})
      elseif c=="W" then
         set_stone("st-metal",i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end
end
--              01234567890123456789
renderLine(00, "WWWWWWWWWWWWWWWWWWWW")
renderLine(01, "W              G   W")
renderLine(02, "W     WWWWWWW  G   W")
renderLine(03, "W     W     W  G O W")
renderLine(04, "W     W     W  G   W")  
renderLine(05, "W     S     W  G   W")
renderLine(06, "W     S  O  W  G O W")
renderLine(07, "W     S     W  G   W")
renderLine(08, "W     W     W  G   W")
renderLine(09, "W     W     W  G O W")
renderLine(10, "W     WWWWWWW  G   W")
renderLine(11, "W              G   W")
renderLine(12, "WWWWWWWWWWWWWWWWWWWW")
--              01234567890123456789
oxyd_shuffle()
