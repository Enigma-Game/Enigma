-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #080

levelw = 20
levelh = 13
create_world(levelw, levelh)

fill_floor("fl-bluegray", 0,0, level_width,level_height)

enigma.AddConstantForce(0,20)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-rock6",i-1,line)
      elseif c=="H" then
         hollow(i-1,line)
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "#                  #")
renderLine(02, "#                  #")
renderLine(03, "#                  #")
renderLine(04, "#        #H#       #")  
renderLine(05, "#        #H#       #")
renderLine(06, "#        #H#       #")
renderLine(07, "#        #H#       #")
renderLine(08, "#                  #")
renderLine(09, "#                  #")
renderLine(10, "#                  #")
renderLine(11, "#                  #")
renderLine(12, "####################")
--              01234567890123456789

set_actor("ac-whiteball-small", 2.5, 11.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",7.5, 11.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 13.5, 11.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",17.5, 11.5, {player=0, mouseforce=1})
