--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
--draw_border("st-marble")
--enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-abyss")
fill_floor("fl-himalaya", 2, 2, 16, 9)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="g" then
         set_stone("st-glass1",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "m" then
         mirror3(i-1,line,0,0,2)
      elseif c == "n" then
         mirrorp(i-1,line,0,0,3)
      elseif c == "p" then
         mirrorp(i-1,line,0,1,3)
      elseif c == "e" then
         set_attrib(laser(i-1,line, TRUE, EAST), "name", "laser1")
      elseif c == "w" then
         set_attrib(laser(i-1,line, TRUE, WEST), "name", "laser2")
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
      elseif c == "b" then
         set_actor("ac-whiteball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
        end
    end
end
--              01234567890123456789
renderLine(00, "        o  o        ")
renderLine(01, "                    ")
renderLine(02, "o  n    mg m  n    o")
renderLine(03, "         g          ")
renderLine(04, "         g          ")
renderLine(05, "     mm  gb  mm     ")
renderLine(06, "e  p mm mggm mm p  w")
renderLine(07, "     mm  ag  mm     ")
renderLine(08, "          g         ")
renderLine(09, "          g         ")
renderLine(10, "o  n    m gm  n    o")
renderLine(11, "                    ")
renderLine(12, "     o        o     ")
--              01234567890123456789

oxyd_shuffle()










