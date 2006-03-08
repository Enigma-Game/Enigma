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
         fill_floor("fl-swamp", i-1,line, 1,1)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "r" then
         set_stone("st-marble",i-1,line)
      elseif c == "w" then
         set_stone("st-break_acwhite",i-1,line)
      elseif c == "s" then
         set_stone("st-switch",i-1,line, {action="onoff", target="laser"})
      elseif c == "S" then
         set_stone("st-switch",i-1,line, {action="onoff", target="laser"})
      elseif c == "l" then
         set_attrib(laser(i-1,line, FALSE, EAST), "name", "laser")
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
      elseif c == "d" then
         document(i-1,line,"Let there be light!")
        end
    end
end
--              01234567890123456789
renderLine(00, "                    ")
renderLine(01, " o         #### oro ")
renderLine(02, "           ####  r  ")
renderLine(03, "           ####  r  ")
renderLine(04, "           #### sr  ")
renderLine(05, "           rrrrrrr  ")
renderLine(06, "l         dwwwwwww  ")
renderLine(07, "           rrrrrrr  ")
renderLine(08, "           #### Sr  ")
renderLine(09, "           ####  r  ")
renderLine(10, "    a      ####  r  ")
renderLine(11, " o         #### oro ")
renderLine(12, "                    ")
--              01234567890123456789

oxyd_shuffle()










