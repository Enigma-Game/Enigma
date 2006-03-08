-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #061
-- change rotors with spermbirds if implemented!

levelw = 58
levelh = 13
create_world(levelw, levelh)
oxyd_default_flavor = "b"
set_actor("ac-blackball", 7,4)
set_actor("ac-rotor", 11.5,10.5, {player=0, mouseforce=0, range=0, force=60})
fill_floor("fl-abyss",0,0,levelw,levelh)
draw_floor("fl-gradient", {23,3}, {1,0}, 12, {type=3})
draw_floor("fl-gradient", {23,4}, {1,0}, 12, {type=4})

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-invisible", i-1,line)
      elseif c=="B" then
	 set_stone("st-actorimpulse_invisible", i-1,line)
      elseif c=="f" then                                 
         set_floor("fl-abyss_fake",  i-1,  line)
      elseif c=="a" then
         set_floor("fl-gradient",  i-1,  line, {type=12})   
      elseif c=="b" then
         set_floor("fl-gradient",  i-1,  line, {type=10})         
      elseif c=="c" then
         set_floor("fl-gradient",  i-1,  line, {type=11})   
      elseif c=="d" then
         set_floor("fl-gradient",  i-1,  line, {type=9})   		 	 	 
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
      elseif c=="I" then
	 set_stone("st-invisible", i-1,line)
	 set_floor("fl-gradient",  i-1,  line, {type=1})   
      elseif c=="i" then
	 set_stone("st-invisible", i-1,line)
	 set_floor("fl-gradient",  i-1,  line, {type=2})         
      elseif c=="O" then
	 oxyd(i-1,line)         
        end
    end  
end
--               0123456789012345678901234567890123456789012345678901234567
renderLine(00 , "##########################################################")
renderLine(01 , "#                                                        #")
renderLine(02 , "#                      BBBBBBBBBBBB                      #")
renderLine(03 , "# 533333333333333333333ffffffffffff333333333333333333336 #")
renderLine(04 , "# 1a4444444444444444444ffffffffffff4444444444444444444b2 #")  
renderLine(05 , "# 12  O             O  BBBBBBBBBBBB  O              O 12 #")
renderLine(06 , "# 12                                                  12 #")
renderLine(07 , "# 12    O         O                                   12 #")
renderLine(08 , "# 1c3333333333333333333IIIIIIIIIIII3333333333333333333d2 #")
renderLine(09 , "# 744444444444444444444iiiiiiiiiiii444444444444444444448 #")
renderLine(10 , "#                                      O           O     #")
renderLine(11 , "#                                                        #")
renderLine(12 , "##########################################################") 
--               0123456789012345678901234567890123456789012345678901234567

oxyd_shuffle()










