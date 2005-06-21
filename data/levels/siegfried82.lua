--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock6")
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-leaves")

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
      elseif c == "g" then
         set_stone("st-grate1",i-1,line)
      elseif c == "r" then
         set_stone("st-rock6",i-1,line)
      elseif c == "x" then
         set_stone("st-shogun",i-1,line)
      elseif c == "#" then
         set_stone("st-oneway-s",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
        end
    end
end
--              01234567890123456789
renderLine(00, "                    ")
renderLine(01, "                    ")
renderLine(02, "   gggggggggggggg   ")
renderLine(03, "   gxn x x n  nxg   ")
renderLine(04, "   g t wte te t g   ")
renderLine(05, "   g s  n  s  s g   ")
renderLine(06, "   gwtewt wtewteg   ")
renderLine(07, "   gx   s x x  xg   ")
renderLine(08, "   gggggggggggggg   ")
renderLine(09, "          a         ")
renderLine(10, " rrrrrrrr##rrrrrrrr ")
renderLine(11, "o                  o")
renderLine(12, "                    ")
--              01234567890123456789

doorv( 2,11, {name="door1"})
shogundot1( 4, 5, {action="openclose", target="door1"})

doorv( 4,11, {name="door2"})
shogundot1( 6, 5, {action="openclose", target="door2"})

doorv( 6,11, {name="door3"})
shogundot1( 7, 5, {action="openclose", target="door3"})

doorv( 8,11, {name="door4"})
shogundot1( 9, 5, {action="openclose", target="door4"})

doorv(11,11, {name="door5"})
shogundot1(10, 5, {action="openclose", target="door5"})

doorv(13,11, {name="door6"})
shogundot1(12, 5, {action="openclose", target="door6"})

doorv(15,11, {name="door7"})
shogundot1(13, 5, {action="openclose", target="door7"})

doorv(17,11, {name="door8"})
shogundot1(15, 5, {action="openclose", target="door8"})
