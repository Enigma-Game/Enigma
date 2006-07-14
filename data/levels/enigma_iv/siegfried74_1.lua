--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-marble")
--enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-leaves")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         set_stone("st-marble",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "h" then
         set_stone("st-marble_hole",i-1,line)
      elseif c == "w" then
         set_stone("st-break_acblack",i-1,line)
      elseif c == "a" then
         set_actor("ac-blackball",i-.5,line+.5)
      elseif c == "x" then
         set_stone("st-wood",i-1,line)
      elseif c == "d" then
         dynamite(i-1,line)
        end
    end
end
--              01234567890123456789
renderLine(00, "                    ")
renderLine(01, "     x        x     ")
renderLine(02, "  ################  ")
renderLine(03, "  #o#o#o#  #o#o#o#  ")
renderLine(04, "  #w#w#w#  #w#w#w#  ")
renderLine(05, "  #w#w#w#dd#w#w#w#  ")
renderLine(06, "  hwhw#w#dd#w#whwh  ")
renderLine(07, "  #w#w#w#  #w#w#w#  ")
renderLine(08, "  #w#w#w#  #w#w#w#  ")
renderLine(09, "  #  x   a    x  #  ")
renderLine(10, "  ####w######w####  ")
renderLine(11, "    x          x    ")
renderLine(12, "                    ")
--              01234567890123456789

set_item("it-hammer", 5, 1)
















