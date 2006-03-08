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
fill_floor("fl-gray")

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
         set_stone("st-rock6",i-1,line)
      elseif c == "X" then
         set_stone("st-oneway_black-e",i-1,line)
      elseif c == "x" then
         set_stone("st-oneway_white-e",i-1,line)
      elseif c == "g" then
         set_stone("st-glass1",i-1,line)
      elseif c == "Y" then
         set_stone("st-switch_black",i-1,line, {action="callback", target="callback1"})
      elseif c == "y" then
         set_stone("st-switch_white",i-1,line, {action="callback", target="callback2"})
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
         set_item("it-yinyang", i-1, line+.5)
      elseif c == "b" then
         set_actor("ac-whiteball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
      elseif c == "d" then
         document(i-1,line,"Nothing can stop us!")
        end
    end
end
--              01234567890123456789
renderLine(00, "rrrrrrrrrrrrrrrrrrrr")
renderLine(01, "rggggggggggggggrggrr")
renderLine(02, "og  n  n  n  n g  mr")
renderLine(03, "rg  tewtewtewt ro  r")
renderLine(04, "rg       n   sbX   r")
renderLine(05, "rgwtewt wt wte rY  r")
renderLine(06, "og s  sn  n  n gd mr")
renderLine(07, "rg  tewtewte tery  r")
renderLine(08, "rg  s          x   r")
renderLine(09, "rgwtewt wt wtear   r")
renderLine(10, "og s  s  s  s  g  mr")
renderLine(11, "rggggggggggggggLggMr")
renderLine(12, "rrrrrrrrrrrrrrrrrrrr")
--              01234567890123456789

switch1 = 0
switch2 = 0

function switchaction()
    lasera=enigma.GetNamedObject("laser")
    if switch1 == 1 and switch2 == 1 then
        SendMessage(lasera, "on")
    end
    if switch1 == 0 and switch2 == 1 then
        SendMessage(lasera, "off")
    end
    if switch1 == 1 and switch2 == 0 then
        SendMessage(lasera, "off")
    end
    if switch1 == 0 and switch2 == 0 then
        SendMessage(lasera, "off")
    end
end

function callback1 (ison)
     switch1=ison
     switchaction()
end

function callback2 (ison)
     switch2=ison
     switchaction()
end










