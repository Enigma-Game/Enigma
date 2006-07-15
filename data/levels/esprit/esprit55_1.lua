-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #055

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-rotor", 1.5, 1.5, {mouseforce=0, range=30, force=30})
set_actor("ac-rotor", 18.5, 11.5, {mouseforce=0, range=30, force=30})
set_actor("ac-blackball", 10.5, 6.5)
fill_floor("fl-leaves")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock2",i-1,line)
      elseif c=="1" then
	 set_floor("fl-gradient",  i-1,  line, {type=3})         
      elseif c=="2" then
	 set_floor("fl-gradient",  i-1,  line, {type=4})   
      elseif c=="3" then
	 set_floor("fl-gradient",  i-1,  line, {type=1})   
      elseif c=="4" then
	 set_floor("fl-gradient",  i-1,  line, {type=2})         
      elseif c=="5" then
         set_floor("fl-gradient",  i-1,  line, {type=5})  
      elseif c=="6" then
         set_floor("fl-gradient",  i-1,  line, {type=6})   	
      elseif c=="7" then
         set_floor("fl-gradient",  i-1,  line, {type=7})         
      elseif c=="8" then
         set_floor("fl-gradient",  i-1,  line, {type=8})   
      elseif c=="O" then
	 oxyd(i-1,line) 
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#533333333333333336#")
renderLine(02, "#1                2#")
renderLine(03, "#1                2#")
renderLine(04, "#1 O   O    O   O 2#")  
renderLine(05, "#1                2#")
renderLine(06, "#1                2#")
renderLine(07, "#1                2#")
renderLine(08, "#1 O   O    O   O 2#")
renderLine(09, "#1                2#")
renderLine(10, "#1                2#")
renderLine(11, "#744444444444444448#")
renderLine(12, "####################")
--              01234567890123456789

oxyd_shuffle()










