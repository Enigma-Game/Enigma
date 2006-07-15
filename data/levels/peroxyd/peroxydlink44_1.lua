--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-rock7")
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
      elseif c == "r" then
         set_stone("st-rock7",i-1,line)
      elseif c == "S" then
         set_stone("st-swap",i-1,line)
      elseif c == "x" then
         set_stone("st-block",i-1,line)
      elseif c == "d" then
         set_stone("st-death",i-1,line)
      elseif c == "g" then
         set_stone("st-grate1",i-1,line)
      elseif c == "h" then
         set_item("it-hollow",i-1,line)
      elseif c == "W" then
         yy1( "white",  i-1, line)
      elseif c == "B" then
         yy1( "black",  i-1, line)
      elseif c == "f" then
         set_stone("st-fakeoxyd",i-1,line)
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "#" then
         set_floor("fl-water",i-1,line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
      elseif c == "b" then
         set_actor("ac-whiteball", i-.5,line+.5)
         set_item("it-yinyang", i-1, line+.5)
        end
    end
end

function yy1( color, x, y)
        stone = format( "st-%s1", color)
        set_stone( stone, x, y)
        set_item("it-hollow", x, y)
end

--              01234567890123456789
renderLine(00, "               fdod ")
renderLine(01, "     r        gWhBhd")
renderLine(02, "     r    ### ghghBo")
renderLine(03, "     r    ### gBhghd")
renderLine(04, "     n   #### ghghWo")
renderLine(05, "    rt  ###   gWhghd")
renderLine(06, " a   s####    ghghBo")
renderLine(07, "   x r        gBhghd")
renderLine(08, "     W bSx    ghghWf")
renderLine(09, "     r        gWhghd")
renderLine(10, "     r        ghghBf")
renderLine(11, "     r        gBhWhd")
renderLine(12, "               odod ")
--              01234567890123456789

oxyd_shuffle()











