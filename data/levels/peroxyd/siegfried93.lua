--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-rough")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="r" then
         set_stone("st-rock5",i-1,line)
      elseif c == "N" then
         set_stone("st-oneway-n",i-1,line)
      elseif c == "W" then
         set_stone("st-oneway",i-1,line)
      elseif c == "w" then
         set_stone("st-wood",i-1,line)
      elseif c == "1" then
         set_item("it-trigger",i-1,line, {action="openclose", target="door1"})
      elseif c == "(" then
         doorv( i-1,line, {name="door1"})
      elseif c == "2" then
         set_item("it-trigger",i-1,line, {action="openclose", target="door2"})
      elseif c == ")" then
         doorv( i-1,line, {name="door2"})
      elseif c == "3" then
         set_item("it-trigger",i-1,line, {action="openclose", target="door3"})
      elseif c == "!" then
         doorv( i-1,line, {name="door3"})
      elseif c == "4" then
         set_item("it-trigger",i-1,line, {action="openclose", target="door4"})
      elseif c == "?" then
         doorv( i-1,line, {name="door4"})
      elseif c == "B" then
         set_stone("st-blocker",i-1,line)
      elseif c == "x" then
         set_stone("st-bolder-n", i-1,line)
      elseif c == "y" then
         set_stone("st-bolder-s", i-1,line)
      elseif c == "z" then
         set_stone("st-bolder-w", i-1,line)
      elseif c == "S" then
         set_stone("st-break_acblack",i-1,line)
      elseif c == "#" then
         set_floor("fl-abyss",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
        end
    end
end
--              01234567890123456789
renderLine(00, "rororrrrrrorrrrrrror")
renderLine(01, "r   r  yyr   #()!? r")
renderLine(02, "rrB B  Szr  o#rrrr o")
renderLine(03, "r rrr r      ##### r")
renderLine(04, "r   r r wrrNw    W r")
renderLine(05, "r r r r       ow r r")
renderLine(06, "r r r r rrro rr  r r")
renderLine(07, "r r   r r     ar r r")
renderLine(08, "r  rrrr rrrr rr  r r")
renderLine(09, "rr B   B    Szrrrr o")
renderLine(10, "rrr rrr rrwwwwr   Sr")
renderLine(11, "r     r   1234   rxr")
renderLine(12, "rorrrorrrrrrrrrrrror")
--              01234567890123456789

oxyd_shuffle()

set_item("it-dynamite", 8, 4)
set_item("it-dynamite",10, 9)
set_item("it-hammer",17, 1)










