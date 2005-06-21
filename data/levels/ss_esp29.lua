-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #029

levelw = 39
levelh = 25
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 14.5,5.5)

fill_floor("fl-abyss",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_floor("fl-normal", i-1,line)
      elseif c=="I" then
         set_floor("fl-ice_001", i-1,line)
      elseif c=="L" then
         set_item("it-extralife",i-1,line)
         set_floor("fl-normal", i-1,line)      
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
      elseif c=="a" then
         set_floor("fl-gradient",  i-1,  line, {type=12})   
      elseif c=="b" then
         set_floor("fl-gradient",  i-1,  line, {type=10})         
      elseif c=="c" then
         set_floor("fl-gradient",  i-1,  line, {type=11})   
      elseif c=="d" then
         set_floor("fl-gradient",  i-1,  line, {type=9})   
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               012345678901234567890123456789012345678
renderLine(00 , "                                       ")
renderLine(01 , "                                       ")
renderLine(02 , "  O333333333336              53333336  ")
renderLine(03 , "  1###########2              1###O##2  ")
renderLine(04 , "  1#a4444444b#c6IIIII5336    1#a44b#2  ")  
renderLine(05 , "  1#2       1##2IIOII1##2    1#2  1#2  ")
renderLine(06 , "  1#2       7448IIIII7b#2    1#2  1#2  ")
renderLine(07 , "  1#2                 1#2    1#2  1#2  ")
renderLine(08 , "  1#2                 1#2    1#2  1#2  ")
renderLine(09 , "  1#2                 1#2    1#2  1#2  ")
renderLine(10 , "  1#c33333333333336   1#2    1#2  1#2  ")
renderLine(11 , "  1###############2   1#2    1#2  1#2  ")
renderLine(12 , "  1#a44444444444b#2   1#2    1#2  1#2  ") 

renderLine(13 , "  1#2           1#2   1#2    1#2  1#2  ")
renderLine(14 , "  1#2       5333d#2   1#2    1#2  1#2  ")
renderLine(15 , "  1#2       1#####2   1#2    1#2  1#2  ")
renderLine(16 , "  1#2       1#a4448   1#c3333d#2  1#2  ")
renderLine(17 , "  1#2       1#2       1L###O###2  1#2  ")  
renderLine(18 , "  1#2       1#2       1#a4444448  1#2  ")
renderLine(19 , "  1#2       1#2       1#2         1#2  ")
renderLine(20 , "  1#c3333333d#2       1#c333333333d#2  ")
renderLine(21 , "  1##########O2       1#############2  ")
renderLine(22 , "  7444444444448       74444444444444O  ")
renderLine(23 , "                                       ")
renderLine(24 , "                                       ")
--               012345678901234567890123456789012345678
set_item("it-crack0",2,7)
set_item("it-crack0",30,8)
set_item("it-crack0",29,22)
set_item("it-crack0",14,15)
set_item("it-crack0",3,18)
oxyd_shuffle()







