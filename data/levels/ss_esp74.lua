-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #074

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 10,4.5)
set_actor("ac-killerball", 3.5,8.5, {player=0, mouseforce=1, controllers=1})
set_actor("ac-killerball", 16.5,8.5, {player=0, mouseforce=1, controllers=1})
fill_floor("fl-space",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "I" then
         set_stone("st-invisible", i-1,line)
      elseif c=="#" then
         set_floor("fl-metal",  i-1,  line) 
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               01234567890123456789
renderLine(00 , "IIIIIIIIIIIIIIIIIIII")
renderLine(01 , "I                  I")
renderLine(02 , "I                  I")
renderLine(03 , "I                  I")
renderLine(04 , "I  #  O  ##  O  #  I")  
renderLine(05 , "I                  I")
renderLine(06 , "IO       OO       OI")
renderLine(07 , "I                  I")
renderLine(08 , "I  #  O  ##  O  #  I")
renderLine(09 , "I                  I")
renderLine(10 , "I                  I")
renderLine(11 , "I                  I")
renderLine(12 , "IIIIIIIIIIIIIIIIIIII") 
--               01234567890123456789

oxyd_shuffle()
