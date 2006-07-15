-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #075

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 3.5, 6.5)
fill_floor("fl-ice_001")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-door_a",i-1,line)
      elseif c=="D" then                                                                
         set_stone("st-death", i-1,  line)
      elseif c=="1" then                                                                
         set_stone("st-door_a", i-1,  line,{name="door1"})
      elseif c=="2" then                                                                
         set_stone("st-door_a", i-1,  line,{name="door2"})
      elseif c =="t" then
         set_item("it-trigger", i-1,line, {invisible=1,action="callback", target="s1"})
      elseif c=="M" then
         set_item("it-magnet",i-1,line,{on=1})
      elseif c=="." then                 
         set_item("it-extralife", i-1,line)
      elseif c=="k" then
	 set_actor("ac-killerball", i-1+0.5,line+0.5, {mouseforce=1, controllers=1}) 
      elseif c=="O" then
	 oxyd(i-1,line) 
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#                  #")
renderLine(02, "#                k #")
renderLine(03, "#                  #")
renderLine(04, "#                  #")  
renderLine(05, "#        OO        #")
renderLine(06, "#     c OMMO c     #")
renderLine(07, "#        OO        #")
renderLine(08, "#                  #")
renderLine(09, "#                  #")
renderLine(10, "#                k #")
renderLine(11, "#                  #")
renderLine(12, "####################")
--              01234567890123456789

oxyd_shuffle()











