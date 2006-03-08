--
-- A level for Enigma
--
-- Copyright (c) 2003 Siegfried Fennig
-- Licensed under the GPL version 2.

levelw = 39
levelh = 13

create_world(levelw, levelh)
enigma.ConserveLevel = FALSE
enigma.SetCompatibility("oxyd1")
oxyd_default_flavor = "d"
fill_floor("fl-wood")
fill_floor("fl-abyss", 0, 0, 19, level_height)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="A" then
         set_item("it-vortex-open", i-1,line, {targetx = 37.5, targety =  1.5})
      elseif c == "B" then
         set_item("it-vortex-open", i-1,line, {targetx = 20.5, targety =  9.5})
      elseif c == "C" then
         set_item("it-vortex-open", i-1,line, {targetx = 20.5, targety = 10.5})
      elseif c == "w" then
         set_stone("st-wood",i-1,line)
      elseif c == "s" then
         set_stone("st-stoneimpulse",i-1,line)
      elseif c == "#" then
         set_floor("fl-abyss",i-1,line)
      elseif c == "x" then
         set_item("it-blackbomb",i-1,line)
      elseif c == "X" then
         set_stone("st-bombs",i-1,line)
      elseif c == "d" then
         document(i-1,line,"No problem! Piece of cake!")
      elseif c == "D" then
         document(i-1,line,"It was easy, wasn't it!")
      elseif c == "o" then
         oxyd( i-1, line)
      elseif c == "a" then
         set_actor("ac-blackball", i-.5,line+.5)
        end
    end
end
--                        1         2         3
--              012345678901234567890123456789012345678
renderLine(00, "                   ###############sssss")
renderLine(01, "                   #######      x    Cs")
renderLine(02, "                   #######sw ###x     s")
renderLine(03, "                   #sssssss#x###x s  ss")
renderLine(04, "                   #D     s x###x s# ##")
renderLine(05, "                   # #s # s x###xxs#x##")
renderLine(06, "                   # #s # s x###  s#X##")
renderLine(07, "                   ###s # s x###  s# ##")
renderLine(08, "                   #d#s # s x     s# ##")
renderLine(09, "                   #A#s # s###### s# ##")
renderLine(10, "                   oBos ######### so o#")
renderLine(11, "                   #o#s           s#o##")
renderLine(12, "                   ###sssssssssssss####")
--              012345678901234567890123456789012345678
--                        1         2         3
oxyd_shuffle()

set_actor("ac-blackball",20.5, 8.5)

draw_stones("st-actorimpulse", {24, 7}, {0,1}, 3)
draw_stones("st-actorimpulse", {27,10}, {1,0}, 6)
set_stones("st-block", {{27, 8},{37, 1}})
fill_floor("fl-stwood",20, 9, 1,1)
fill_floor("fl-stwood",20,10, 1,1)
fill_floor("fl-stwood",37, 1, 1,1)










