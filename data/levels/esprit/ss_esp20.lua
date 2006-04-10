-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #020
-- force against the actors by switching to hill could be stronger!

levelw = 20
levelh = 13
create_world(levelw, levelh)

fill_floor("fl-wood")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-greenbrown",i-1,line)
      elseif c=="H" then
         set_floor("fl-gray",i-1,line)
         hollow(i-1,line)
      elseif c=="1" then
	 set_stone("st-switch",i-1,line, {action="callback", target="s1"})
      elseif c=="2" then
	 set_stone("st-switch",i-1,line, {action="callback", target="s2"})
      elseif c=="3" then
	 set_stone("st-switch",i-1,line, {action="callback", target="s3"})
      elseif c=="4" then
	 set_stone("st-switch",i-1,line, {action="callback", target="s4"})	
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#                  #")
renderLine(02, "#                  #")
renderLine(03, "#                  #")
renderLine(04, "#                  #")  
renderLine(05, "#                  #")
renderLine(06, "#    H1 H2 H3 H4   #")
renderLine(07, "#                  #")
renderLine(08, "#                  #")
renderLine(09, "#                  #")
renderLine(10, "#                  #")
renderLine(11, "#                  #")
renderLine(12, "####################")
--              01234567890123456789

local s={0, 0, 0, 0}
function s1 () sx(1) end 
function s2 () sx(2) end 
function s3 () sx(3) end 
function s4 () sx(4) end 

function sx(num)
   s[num] = 1-s[num]
   if (s[num] == 1) then 
      set_item("it-hill",3*num+2,6)
   else 
      set_item("it-hollow",3*num+2,6)
   end
end

set_actor("ac-whiteball-small", 9.5, 10.5, {player=0})
set_actor("ac-whiteball-small",10.5, 10.5, {player=0})
set_actor("ac-whiteball-small", 11.5, 10.5, {player=0})
set_actor("ac-whiteball-small",8.5, 10.5, {player=0})










