-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #039

levelw = 20
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 11,8, {player=0})
fill_floor("fl-red",0,0,levelw,levelh)

enigma.AddConstantForce(0,30)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-actorimpulse", i-1,line)
      elseif c=="D" then
         set_stone("st-death", i-1,line)
      elseif c=="^" then
         set_stone("st-oneway_black-n", i-1,line)
      elseif c=="d" then
         document(i-1,line,"This space intentionally left blank:                                                            ")
      elseif c=="f" then
         set_item("it-flagblack",i-1,line)
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               01234567890123456789
renderLine(00 , "DDDDDDDDDDDDDDDDDDDD")
renderLine(01 , "#DDDDDDDDDDDDDDDDDDD")
renderLine(02 , "#O                d#")
renderLine(03 , "#                  #")
renderLine(04 , "####  O^^^^#^^^^O  #")  
renderLine(05 , "#                  #")
renderLine(06 , "# O         O     O#")
renderLine(07 , "#    ####         f#")
renderLine(08 , "#              #####")
renderLine(09 , "#                  #")
renderLine(10 , "#                  #")
renderLine(11 , "####################")
renderLine(12 , "####################") 
--               01234567890123456789

oxyd_shuffle()





