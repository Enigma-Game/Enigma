--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "c"
fill_floor("fl-leaves")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="r" then
         set_stone("st-rock7",i-1,line)
      elseif c == "w" then
         set_stone("st-wood",i-1,line)
      elseif c == "C" then
         set_item("it-trigger",i-1,line, {action="callback", target="bl1"})
      elseif c == "c" then
         set_stone("st-blocker",i-1,line, {name="blocker1"})
      elseif c == "D" then
         set_item("it-trigger",i-1,line, {action="callback", target="bl2"})
      elseif c == "d" then
         set_stone("st-blocker",i-1,line, {name="blocker2"})
      elseif c == "E" then
         set_item("it-trigger",i-1,line, {action="callback", target="bl3"})
      elseif c == "e" then
         set_stone("st-blocker",i-1,line, {name="blocker3"})
      elseif c == "F" then
         set_item("it-trigger",i-1,line, {action="callback", target="bl4"})
      elseif c == "f" then
         set_stone("st-blocker",i-1,line, {name="blocker4"})
      elseif c == "G" then
         set_item("it-trigger",i-1,line, {action="callback", target="bl5"})
      elseif c == "g" then
         set_stone("st-blocker",i-1,line, {name="blocker5"})
      elseif c == "H" then
         set_item("it-trigger",i-1,line, {action="callback", target="bl6"})
      elseif c == "h" then
         set_stone("st-blocker",i-1,line, {name="blocker6"})
      elseif c == "#" then
         set_floor("fl-water",i-1,line)
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
--              01234567890123456789
renderLine(00, "rrrrrrrrrrrrrrrrrrrr")
renderLine(01, "rrrrrrrrrrrrrrrrrrrr")
renderLine(02, "r   rrr  r     r   r")
renderLine(03, "r rw     d  rr  wrgr")
renderLine(04, "r c   rrrr D      Gr")
renderLine(05, "r w #### abe#### rrr")
renderLine(06, "r w #oo#r  E#oo#   r")
renderLine(07, "rrC #oo# Frr#oo#H  r")
renderLine(08, "r rr#### f r####hrrr")
renderLine(09, "r w     r w wr r w r")
renderLine(10, "r       r    r     r")
renderLine(11, "rrrrrrrrrrrrrrrrrrrr")
renderLine(12, "rrrrrrrrrrrrrrrrrrrr")
--              01234567890123456789

oxyd_shuffle()

function bl1()
        b1=enigma.GetNamedObject("blocker1")
        SendMessage(b1, "trigger")
end

function bl2()
        b1=enigma.GetNamedObject("blocker2")
        SendMessage(b1, "trigger")
end

function bl3()
        b1=enigma.GetNamedObject("blocker3")
        SendMessage(b1, "trigger")
end

function bl4()
        b1=enigma.GetNamedObject("blocker4")
        SendMessage(b1, "trigger")
end

function bl5()
        b1=enigma.GetNamedObject("blocker5")
        SendMessage(b1, "trigger")
end

function bl6()
        b1=enigma.GetNamedObject("blocker6")
        SendMessage(b1, "trigger")
end
