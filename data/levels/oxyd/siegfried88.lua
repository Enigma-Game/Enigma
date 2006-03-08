--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 39
levelh = 25

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-abyss")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c == "r" then
         set_stone("st-rock1",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "S" then
         springboard(i-1,line)
      elseif c == "g" then
         set_floor("fl-gray",i-1,line)
      elseif c == "n" then
         set_floor("fl-gradient",i-1,line,{type=22})
      elseif c == "e" then
         set_floor("fl-gradient",i-1,line,{type=23})
      elseif c == "s" then
         set_floor("fl-gradient",i-1,line,{type=21})
      elseif c == "w" then
         set_floor("fl-gradient",i-1,line,{type=24})
      elseif c == "b" then
         basis(i-1,line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
      elseif c == "d" then
         document(i-1,line,"")
        end
    end
end

function basis(x, y)
       fill_floor("fl-gray", x, y, 3, 3)
end

function springboard(x, y)
       set_floor("fl-springboard", x, y)
       set_item("it-springboard", x, y)
end

--                        1         2         3
--              012345678901234567890123456789012345678
renderLine(00, "  o                                    ")
renderLine(01, " b  rrrrrrrrrr   b  rrrrrrrrrrrr   b   ")
renderLine(02, "  a eeeeeeeSee      eeeeeeeeeSee      o")
renderLine(03, "    rrrrrrrrrr      rrrrrrrrrrrr       ")
renderLine(04, "                 rgr               rsr ")
renderLine(05, "     b     rrrrrrrgrb     rrrrrrrr rsr ")
renderLine(06, "           wSwwwwwgr      Swwwwwgr rsr ")
renderLine(07, " rnr       rrrrrrrrr      rrrrrrgr rsr ")
renderLine(08, " rnr rgr         rggggr        rgr rsr ")
renderLine(09, " rSr rgr         rsrrrr        rgr rsr ")
renderLine(10, " rnr rgr         rsr           rgr rsr ")
renderLine(11, " rnr rgr         rsr           rgr rsr ")
renderLine(12, " rnr rgr         rsr           rgr rsr ")
renderLine(13, " rnr rgr         rsr           rgr rsr ")
renderLine(14, " rnr rgr         rsr           rgr rsr ")
renderLine(15, " rnr rgr         rsr           rgr rSr ")
renderLine(16, " rnr rgr         rSr           rgr rsr ")
renderLine(17, " rnr rgr         rsr           rgr rsr ")
renderLine(18, " rnr rgrrrrrrrrr     b  rrrrrrrrgr     ")
renderLine(19, " rnr ogeeeeeeeeS        gggggggggo     ")
renderLine(20, " rnr rrrrrrrrrrr        rrrrrrrrrr     ")
renderLine(21, " b     rrrrrrrrrrb     rrrrrrrrrrrrb   ")
renderLine(22, "o      wwSwwwwwww      wwSwwwwwwwww    ")
renderLine(23, "       rrrrrrrrrr      rrrrrrrrrrrr    ")
renderLine(24, "                                    o  ")
--              012345678901234567890123456789012345678
--                        1         2         3

oxyd_shuffle()










