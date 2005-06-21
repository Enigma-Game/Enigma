-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #084

levelw = 20
levelh = 25
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 9.5,5.5, {player=0})
fill_floor("fl-space",0,0,levelw,levelh)
fill_floor("fl-metal",1,1,levelw-2,levelh-2)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-actorimpulse_invisible", i-1,line)
      elseif c=="I" then
         set_stone("st-invisible", i-1,line) 
       
      elseif c=="0" then
	 set_item("it-crack0", i-1,line)   
      elseif c=="1" then
	 set_item("it-crack1", i-1,line)   
      elseif c=="R" then
	 set_actor("ac-rotor", i-1+0.5,line+0.5, {mouseforce=1, range=50, force=20})
      elseif c=="O" then
	 oxyd(i-1,line)
        end
    end  
end
--               01234567890123456789
renderLine(00 , "#I#I#I#III#I#I#II###")
renderLine(01 , "I                  I")
renderLine(02 , "I  O    1    1  0  I")
renderLine(03 , "I  1               #")
renderLine(04 , "#     1            #")  
renderLine(05 , "#             O    I")
renderLine(06 , "I          1  0    #")
renderLine(07 , "#  O  0            I")
renderLine(08 , "I                  #")
renderLine(09 , "I       0 0   O    #")
renderLine(10 , "#      0 0      11 #")
renderLine(11 , "#                  I")
renderLine(12 , "#00  01     0      I")
renderLine(13 , "I                  #")
renderLine(14 , "I       0  1   1   #")
renderLine(15 , "I   O              I")
renderLine(16 , "#      0       O   I")
renderLine(17 , "I       1          #")
renderLine(18 , "I   O         00   I")
renderLine(19 , "I  0   0         0 #")
renderLine(20 , "#                  I")
renderLine(21 , "I   1         O    #")
renderLine(22 , "I   0   0 0     00 I")
renderLine(23 , "I                 RI")
renderLine(24 , "##III##III#I##IIII##")
--               01234567890123456789

oxyd_shuffle()