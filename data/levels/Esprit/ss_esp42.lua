-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #042
-- change rotors with spermbirds if implemented!

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "a"
set_actor("ac-blackball", 10,11.5, {player=0})
set_actor("ac-rotor", 10,6.5, {mouseforce=0, range=40, force=30})

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-stone1", i-1,line)
      elseif c=="a" then
         set_floor("fl-abyss", i-1,line)
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
--               01234567890123456789
renderLine(00 , "#####O########O#####")
renderLine(01 , "#533333333333333336#")
renderLine(02 , "#153333333333333362#")
renderLine(03 , "#115333333333333622#")
renderLine(04 , "O111533333333336222O")  
renderLine(05 , "#1111aaaaaaaaaa2222#")
renderLine(06 , "#1111aaaaaaaaaa2222#")
renderLine(07 , "#1111aaaaaaaaaa2222#")
renderLine(08 , "O111744444444448222O")
renderLine(09 , "#117444444444444822#")
renderLine(10 , "#174444444444444482#")
renderLine(11 , "#744444444444444448#")
renderLine(12 , "#####O########O#####") 
--               01234567890123456789

oxyd_shuffle()
