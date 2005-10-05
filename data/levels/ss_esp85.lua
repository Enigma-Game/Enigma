-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #085
-- This level should only be solvable if the marble activates
-- without anything (no inventory) the trigger. If somebody knows to realize this,
-- please change it. For now the doors open if the trigger is activated.


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
      elseif c=="O" then
	 oxyd(i-1,line) 
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#    ...          D#")
renderLine(02, "#                  #")
renderLine(03, "#                  #")
renderLine(04, "#              #####")  
renderLine(05, "#        OO    #1  #")
renderLine(06, "#       OMMO    tOO#")
renderLine(07, "#        OO    #2  #")
renderLine(08, "#              #####")
renderLine(09, "#                  #")
renderLine(10, "#                  #")
renderLine(11, "#                 D#")
renderLine(12, "####################")
--              01234567890123456789

oxyd_shuffle()

no_inventory = true

function s1()
    if no_inventory==true then
        SendMessage("door1", "open")
        SendMessage("door2", "open")
    end
end
