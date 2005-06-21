--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "b"
fill_floor("fl-rough")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="t" then
         set_stone("st-turnstile",i-1,line)
      elseif c == "n" then
         set_stone("st-turnstile-n",i-1,line)
      elseif c == "s" then
         set_stone("st-turnstile-s",i-1,line)
      elseif c == "e" then
         set_stone("st-turnstile-e",i-1,line)
      elseif c == "w" then
         set_stone("st-turnstile-w",i-1,line)
      elseif c == "r" then
         set_stone("st-rock1",i-1,line)
      elseif c == "E" then
         set_stone("st-oneway-e",i-1,line)
      elseif c == "g" then
         set_stone("st-glass1",i-1,line)
      elseif c == "T" then
         set_item("it-trigger",i-1,line, {action="onoff", target="laser"})
      elseif c == "L" then
         set_attrib(laser(i-1,line, FALSE, EAST), "name", "laser")
      elseif c == "m" then
         mirrorp(i-1,line,0,1,2)
      elseif c == "M" then
         mirrorp(i-1,line,0,0,4)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
        end
    end
end
--              01234567890123456789
renderLine(00, "rrrrrrrrrrrrrrrrrrrr")
renderLine(01, "rrrrrrrrrrrrrrrrrrrr")
renderLine(02, "rrrrrrrrrrrrrr E rrr")
renderLine(03, "rggggggggggggg g rrr")
renderLine(04, "og  n  n  n  n g  mr")
renderLine(05, "rg  tewtewtewt g   r")
renderLine(06, "rg       n   sag   r")
renderLine(07, "rgwtewt wt wte g   r")
renderLine(08, "og s  s        g  mr")
renderLine(09, "rgggggggggggggggTggr")
renderLine(10, "rrrrrrrrrrrrrrrrrrgr")
renderLine(11, "rrrrrrrrrrrrrrrLggMr")
renderLine(12, "rrrrrrrrrrrrrrrrrrrr")
--              01234567890123456789
