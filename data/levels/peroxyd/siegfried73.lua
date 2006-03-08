--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "a"
fill_floor("fl-samba")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         fill_floor("fl-abyss", i-1,line, 1,1)
      elseif c == "s" then
         set_stone("st-scissors",i-1,line)
      elseif c == "h" then
         set_item("it-hollow",i-1,line)
        end
    end
end
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "####################")
renderLine(02, "####################")
renderLine(03, "####################")
renderLine(04, "#  #####h########  #")
renderLine(05, "#  ###        ###  #")
renderLine(06, "       s    s       ")
renderLine(07, "#  ###        ###  #")
renderLine(08, "#  ########h#####  #")
renderLine(09, "####################")
renderLine(10, "####################")
renderLine(11, "####################")
renderLine(12, "####################")
--              01234567890123456789

local st1=set_stone("st-rubberband", 0, 6)
local st2=set_stone("st-rubberband",19, 6)

local ac1=set_actor("ac-whiteball-small",  2.5,6.5, {player=0, mouseforce=1})
local ac2=set_actor("ac-whiteball-small", 17.5,6.5, {player=0, mouseforce=1})

AddRubberBand( ac1, st1, 4, 1)
AddRubberBand( ac2, st2, 4, 1)










