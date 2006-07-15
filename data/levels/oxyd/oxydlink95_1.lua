--
-- A level for Enigma
--
-- Copyright (c) 2004 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-samba")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="B" then
         set_stone("st-block", i-1,line)
      elseif c == "w" then
         set_stone("st-wood",i-1,line)
      elseif c == "y" then
         set_stone("st-yinyang1",i-1,line)
      elseif c == "s" then
         set_stone("st-rock1",i-1,line)
      elseif c == "W" then
         set_stone("st-oneway_white-w",i-1,line)
      elseif c == "#" then
         set_floor("fl-abyss",i-1,line)
      elseif c == "x" then
         set_item("it-blackbomb",i-1,line)
      elseif c == "X" then
         set_stone("st-bombs",i-1,line)
      elseif c == "S" then
         set_stone("st-switch",i-1,line, {action="callback", target="sw1"})
      elseif c == "1" then
         set_stone("st-door",i-1,line, {name="door1", type="v"})
      elseif c == "2" then
         set_stone("st-door",i-1,line, {name="door2", type="h"})
      elseif c == "c" then
         set_item("it-crack3",i-1,line)
      elseif c == "Y" then
         f1( i-1, line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
      elseif c == "b" then
         set_actor("ac-whiteball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
        end
    end
end

function f1( x, y)
        set_stone("st-grate1", x, y)
        set_floor("fl-abyss", x, y)
end

function sw1()
    SendMessage("door1", "openclose")
    SendMessage("door2", "openclose")
end

--                        1
--              01234567890123456789
renderLine(00, "####################")
renderLine(01, "o#Y################S")
renderLine(02, "# 1x#########o# c  #")
renderLine(03, "Y2Yx#  yw   ### YY #")
renderLine(04, "# #x  ## ##    yYY #")
renderLine(05, "#xxxB W  ## ###   y#")
renderLine(06, "Yx###### ## ###### #")
renderLine(07, "YyxxxB  accb   y   #")
renderLine(08, "Yxxsssss ###### ## #")
renderLine(09, " X sssss ###### ## #")
renderLine(10, "oXxxx          y   #")
renderLine(11, "###################o")
renderLine(12, "####################")
--              01234567890123456789
--                        1
oxyd_shuffle()











