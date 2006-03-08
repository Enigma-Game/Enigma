-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ERSPRIT #073

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "a"
fill_floor( "fl-floor_001",0,0,levelw,levelh)
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_floor("fl-abyss",i-1,line)
      elseif c=="d" then
         set_actor("ac-blackball", i-.5,line+.5)
         document(i-1,line,"if you think this is to easy, then play -Bavarian Madness-")
      elseif c =="W" then                                                               
         set_stone("st-marble",i-1,line)        
      elseif c =="." then                                                               
         set_item("it-burnable_ash",i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line) 
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "WWWWWWWWWWWWWWWWWWWW")
renderLine(02, "W###    .     . ###W")
renderLine(03, "W#O  .      .d   O#W")  
renderLine(04, "W#          .  .  #W")
renderLine(05, "W  .               W")
renderLine(06, "W     .            W")
renderLine(07, "W .       . . .  . W")
renderLine(08, "W#   .  .         #W")
renderLine(09, "W#O  .     .  .  O#W")
renderLine(10, "W###    .       ###W")
renderLine(11, "WWWWWWWWWWWWWWWWWWWW")
renderLine(12, "####################")
--              01234567890123456789
                              
oxyd_shuffle()










