-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #012

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"       -- Default flavor for oxyd stones.

set_actor("ac-blackball", 11.5,4.5)
draw_checkerboard_floor( "fl-woven", "fl-bluegreenx", 0, 0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-death", i-1,line)
      elseif c=="B" then
         set_stone("st-actorimpulse_invisible",i-1,line)
      elseif c=="b" then
         set_stone("st-actorimpulse_invisible",i-1,line)         
         set_item("it-crack1", i-1,  line)
      elseif c=="C" then                                                                
         set_item("it-crack1", i-1,  line, {fixed=1})
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "####################")
renderLine(01 , "#                  #")
renderLine(02 , "#   B   O   B      #")
renderLine(03 , "#  O C        bO   #")
renderLine(04 , "#                  #")  
renderLine(05 , "#           B      #")
renderLine(06 , "#    B    B        #")
renderLine(07 , "#   C           O  #")
renderLine(08 , "#  B O             #")
renderLine(09 , "#           BC    C#")
renderLine(10 , "#        B   O     #")
renderLine(11 , "#                  #")
renderLine(12 , "####################")
--               01234567890123456789
oxyd_shuffle()
