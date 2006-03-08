--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-stone1")
enigma.ConserveLevel = FALSE
oxyd_default_flavor = "d"
fill_floor("fl-concrete")
fill_floor("fl-abyss", 1, 13, 18, 11)
fill_floor("fl-concrete", 9, 14, 2, 2)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c == "g" then
         set_floor("fl-concrete",i-1,line)
      elseif c == "r" then
         set_stone("st-rock3",i-1,line)
      elseif c == "s" then
         set_stone("st-shogun",i-1,line)
      elseif c == "1" then
         set_stone("st-switch",i-1,line, {action="openclose", target="door1"})
      elseif c == "F" then
         doorh( i-1,line, {name="door1"})
      elseif c == "2" then
         set_stone("st-switch",i-1,line, {action="openclose", target="door2"})
      elseif c == "E" then
         doorh( i-1,line, {name="door2"})
      elseif c == "3" then
         set_stone("st-switch",i-1,line, {action="openclose", target="door3"})
      elseif c == "G" then
         doorh( i-1,line, {name="door3"})
      elseif c == "4" then
         set_stone("st-switch",i-1,line, {action="openclose", target="door4"})
      elseif c == "H" then
         doorh( i-1,line, {name="door4"})
      elseif c == "5" then
         set_stone("st-switch",i-1,line, {action="openclose", target="door5"})
      elseif c == "A" then
         doorh( i-1,line, {name="door5"})
      elseif c == "6" then
         set_stone("st-switch",i-1,line, {action="openclose", target="door6"})
      elseif c == "D" then
         doorh( i-1,line, {name="door6"})
      elseif c == "7" then
         set_stone("st-switch",i-1,line, {action="openclose", target="door7"})
      elseif c == "B" then
         doorh( i-1,line, {name="door7"})
      elseif c == "S" then
         shogundot1( i-1,line, {action="openclose", target="door8"})
      elseif c == "C" then
         doorh( i-1,line, {name="door8"})
      elseif c == "X" then
         shogundot1( i-1,line)
      elseif c == "u" then
         set_item("it-umbrella",i-1,line)
      elseif c == "k" then
         set_item("it-coffee",i-1,line)
      elseif c == "h" then
         set_item("it-cherry",i-1,line)
      elseif c == "d" then
         document(i-1,line,"Umbrellas prevent free fall!")
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
        end
    end
end
--              01234567890123456789
renderLine(00, "                    ")
renderLine(01, " or              ro ")
renderLine(02, "5ur              ru ")
renderLine(03, " Er              rF ")
renderLine(04, "         SX         ")
renderLine(05, "                    ")
renderLine(06, "                    ")
renderLine(07, "                    ")
renderLine(08, "                    ")
renderLine(09, " Gr              rH ")
renderLine(10, "6ur      a       ru7")
renderLine(11, " or              ro ")
renderLine(12, " ssssssssssssssssss ")
renderLine(13, " rrr    gggg    rrr ")
renderLine(14, " r1r     uh     r2r ")
renderLine(15, " rAr     kd     rBr ")
renderLine(16, "                    ")
renderLine(17, "                    ")
renderLine(18, "                    ")
renderLine(19, "                    ")
renderLine(20, "                    ")
renderLine(21, " rCr            rDr ")
renderLine(22, " r3r            r4r ")
renderLine(23, " rrr            rrr ")
renderLine(24, "                    ")
--              01234567890123456789

oxyd_shuffle()

set_item("it-umbrella", 4, 12)










