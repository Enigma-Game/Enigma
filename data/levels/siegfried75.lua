--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-wood")
enigma.ConserveLevel = FALSE
enigma.SetCompatibility("oxyd1")
oxyd_default_flavor = "d"
fill_floor("fl-wood")
function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         set_floor("fl-abyss",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "b" then
         set_stone("st-bombs",i-1,line)
      elseif c == "x" then
         set_item("it-blackbomb",i-1,line)
      elseif c == "a" then
         set_actor("ac-blackball",i-.5,line+.5)
      elseif c == "e" then
         set_item("it-extralife",i-1,line)
      elseif c == "c" then
         set_item("it-crack3",i-1,line)
      elseif c == "w" then
         set_stone("st-wood",i-1,line)
      elseif c == "d" then
         dynamite(i-1,line)
        end
    end
end
--              01234567890123456789
renderLine(00, "                    ")
renderLine(01, " o   b    #####o### ")
renderLine(02, "   ##x### ##### ### ")
renderLine(03, "  # #x### ##### ### ")
renderLine(04, " d# #x### ##### ### ")
renderLine(05, "  #  xx  a d  #b  o ")
renderLine(06, "  #######cc##  d# o ")
renderLine(07, "    #bxxxxxe##### # ")
renderLine(08, " o### ########### # ")
renderLine(09, " #### ########### # ")
renderLine(10, " #### ########### # ")
renderLine(11, " ###o             # ")
renderLine(12, "                    ")
--              01234567890123456789

oxyd_shuffle()
set_stones("st-wood", {{ 3, 6}})
