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
fill_floor("fl-samba")

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
         set_stone("st-rock5",i-1,line)
      elseif c == "T" then
         set_item("it-trigger",i-1,line, {action="onoff", target="laser"})
      elseif c == "L" then
         set_attrib(laser(i-1,line, FALSE, EAST), "name", "laser")
      elseif c == "S" then
         set_stone("st-stoneimpulse",i-1,line)
      elseif c == "H" then
         set_stone("st-stoneimpulse-hollow",i-1,line)
      elseif c == "#" then
         set_floor("fl-abyss",i-1,line)
      elseif c == "m" then
         mirrorp(i-1,line,0,0,4)
      elseif c == "M" then
         mirrorp(i-1,line,0,0,3)
      elseif c == "x" then
         mirrorp(i-1,line,0,1,4)
      elseif c == "3" then
         mirror3(i-1,line,0,0,3)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
        end
    end
end
--              01234567890123456789
renderLine(00, "rrrrrrrrrrrrrrrrrrrr")
renderLine(01, "r     rrrrrrrr     r")
renderLine(02, "r     rm mrror     r")
renderLine(03, "r     rSn  n r   trr")
renderLine(04, "r     r#t wte    sHo")
renderLine(05, "L 3   x sxn      #SS")
renderLine(06, "r     te  te     ##S")
renderLine(07, "r     s   s       #S")
renderLine(08, "r     nwtwt a     #S")
renderLine(09, "r    wt s s      T r")
renderLine(10, "r M   s  M         r")
renderLine(11, "r r   rrrrrrrr     r")
renderLine(12, "rrrrrrrrrrrrrrrrrrrr")
--              01234567890123456789











