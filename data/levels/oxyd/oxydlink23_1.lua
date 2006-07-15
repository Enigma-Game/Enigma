--
-- A level for Enigma
--
-- Copyright (c) 2004 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 39
levelh = 25

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
draw_border("st-blue-sand")
fill_floor("fl-acblack", 0, 0,20,13)
fill_floor("fl-acblack",19,13,20,12)
fill_floor("fl-acwhite",20, 0,19,13)
fill_floor("fl-acwhite", 0,13,19,12)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c == "r" then
         set_stone("st-blue-sand",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "B" then
         b1("black", i-1, line)
      elseif c == "W" then
         w1("white", i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
      elseif c == "b" then
         set_actor("ac-whiteball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
        end
    end
end

function b1( color, x, y)
        stone = format( "st-%s1", color)
        set_stone( stone, x, y)
        set_floor("fl-acblack", x, y)
end

function w1( color, x, y)
        stone = format( "st-%s1", color)
        set_stone( stone, x, y)
        set_floor("fl-acwhite", x, y)
end

--                        1         2         3
--              012345678901234567890123456789012345678
renderLine(00, "                                       ")
renderLine(01, "                   r                   ")
renderLine(02, "                   r          orrrr    ")
renderLine(03, "   r  rrrro        r  rrrr        r    ")
renderLine(04, "   r      r   r    W              r    ")
renderLine(05, "   r      r   rrr  r              r    ")
renderLine(06, "   r      r   r    r     r        r    ")
renderLine(07, "   r r r  r   r    r     r        r    ")
renderLine(08, "   r r r  r   r    r     r        r    ")
renderLine(09, "   r r r      r    r     rrrrr    r    ")
renderLine(10, "   r   r  ab       B              r    ")
renderLine(11, "   r               r              r    ")
renderLine(12, " rBrrrrrrrrrrrrrrWrrrrrrrWrrrrrrrrrrBr ")
renderLine(13, "      r            r   r   r           ")
renderLine(14, "      r  r         r   r   r           ")
renderLine(15, "   rrrr  r   rrr   r   r               ")
renderLine(16, "      r  r     r   W   r  orrrr        ")
renderLine(17, "      r  r     r   r   r  r       r    ")
renderLine(18, "         r     r   r   r  r       r    ")
renderLine(19, "         r  o  r   r   r  r       r    ")
renderLine(20, "   rrrr  r  r  rrrrr   r  r       r    ")
renderLine(21, "         r  r  r   r      r       r    ")
renderLine(22, "            r      B              r    ")
renderLine(23, "            r      r              r    ")
renderLine(24, "                                       ")
--              012345678901234567890123456789012345678
--                        1         2         3

oxyd_shuffle()












