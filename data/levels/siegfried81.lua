--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-greenbrown")
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "a"
fill_floor("fl-abyss")

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
      elseif c == "x" then
         set_stone("st-turnstile-w",i-1,line)
      elseif c == "b" then
         set_stone("st-greenbrown",i-1,line)
      elseif c == "w" then
         set_floor("fl-wood",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
        end
    end
end
--              01234567890123456789
renderLine(00, " o                  ")
renderLine(01, " wwnwwwwwnwwwwww  o ")
renderLine(02, "   t     t   o  t w ")
renderLine(03, " ww wwwww ww   wsww ")
renderLine(04, "  w wwwww ww   ww w ")
renderLine(05, "  wbwwwwwnwwwwwww w ")
renderLine(06, "  bw     t       t  ")
renderLine(07, "  nwwwwww wwww wwsw ")
renderLine(08, "  te     t  ww ww w ")
renderLine(09, "  wwwwwwwsw ww ww w ")
renderLine(10, "  w       w  xt   w ")
renderLine(11, " ow       wwww ww o ")
renderLine(12, "               o    ")
--              01234567890123456789

oxyd_shuffle()

set_actor("ac-blackball", 5.5, 4.5)
