-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #070

levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.SlopeForce = 10
fill_floor("fl-abyss", 0,0, level_width,level_height)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-greenbrown",i-1,line)
      elseif c=="H" then
         set_floor("fl-gray",i-1,line)
         hollow(i-1,line)
      elseif c=="1" then
	 set_floor("fl-gradient",  i-1,  line, {type=2})
      elseif c=="2" then
	 set_floor("fl-gradient",  i-1,  line, {type=1})
      elseif c=="3" then
	 set_floor("fl-gradient",  i-1,  line, {type=3})
      elseif c=="4" then
	 set_floor("fl-gradient",  i-1,  line, {type=4})	
      elseif c=="a" then
         set_stone("st-actorimpulse_invisible",i-1,line)
	 set_floor("fl-gradient",  i-1,  line, {type=2})
      elseif c=="b" then
         set_stone("st-actorimpulse_invisible",i-1,line)      
	 set_floor("fl-gradient",  i-1,  line, {type=1})
      elseif c=="c" then
         set_stone("st-actorimpulse_invisible",i-1,line)      
	 set_floor("fl-gradient",  i-1,  line, {type=3})
      elseif c=="d" then
         set_stone("st-actorimpulse_invisible",i-1,line)      
	 set_floor("fl-gradient",  i-1,  line, {type=4})	  	 	
      elseif c=="M" then
         set_stone("st-greenbrown",i-1,line)
        end
    end
end
--              01234567890123456789
renderLine(00, "###  ### ###########")
renderLine(01, "#a#  #H# #H3333333c#")
renderLine(02, "#1#  #2# ###########")
renderLine(03, "#1#  #2#            ")
renderLine(04, "#1#  #2#            ")  
renderLine(05, "#1#  #2#            ")
renderLine(06, "#1#  #2#            ")
renderLine(07, "#1#  #2#            ")
renderLine(08, "#1#  #2#            ")
renderLine(09, "#1#  #2#            ")
renderLine(10, "#1#  #2# ###########")
renderLine(11, "#H#  #b# #d4444444H#")
renderLine(12, "###  ### ###########")
--              01234567890123456789

set_actor("ac-whiteball-small", 1.5, 5.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",6.5, 5.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 14.5, 1.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",14.5, 11.5, {player=0, mouseforce=1})
