--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
fill_floor("fl-rough")

oxyd_default_flavor = "b"

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         set_stone("st-door_b",i-1,line,{type="v"})
      elseif c == "~" then
         set_stone("st-door_b",i-1,line,{type="v"})
      elseif c == "O" then
         oxyd( i-1, line)
      elseif c == "+" then
         set_stone("st-shogun-s",i-1,line)
      elseif c == " " then
         set_floor("fl-sand", i-1, line)
      end
    end
end

set_stone("st-door_b", 1,7, {type="v", name="door1"})
set_stone("st-door_b", 6,9, {type="v", name="door2"})
set_stone("st-door_b",14,7, {type="v", name="door3"})
set_stone("st-door_b",11,4, {type="v", name="door4"})

set_stone("st-door_b", 8,2, {type="v", name="floor8"})
set_stone("st-door_b", 8,3, {type="v", name="floor11"})
set_stone("st-door_b", 8,4, {type="v", name="floor15"})
set_stone("st-door_b", 8,5, {type="v", name="floor5"})
set_stone("st-door_b", 8,6, {type="v", name="floor10"})
set_stone("st-door_b", 8,7, {type="v", name="floor14"})
set_stone("st-door_b", 9,7, {type="v", name="floor3"})
set_stone("st-door_b",10,7, {type="v", name="floor9"})
set_stone("st-door_b",10,6, {type="v", name="floor13"})
set_stone("st-door_b",10,5, {type="v", name="floor16"})
set_stone("st-door_b",10,4, {type="v", name="floor7"})
set_stone("st-door_b",10,3, {type="v", name="floor12"})
set_stone("st-door_b", 9,10, {type="v", name="floor1"})
set_stone("st-door_b", 8,10, {type="v", name="floor2"})
set_stone("st-door_b", 7,10, {type="v", name="floor4"})
set_stone("st-door_b", 7, 9, {type="v", name="floor6"})

shogundot1( 2, 2, {target="door1", action="openclose"})
shogundot1( 2, 1, {target="floor1", action="openclose"})
shogundot1( 4, 1, {target="floor2", action="openclose"})
shogundot1( 5, 1, {target="floor3", action="openclose"})
shogundot1( 5, 2, {target="floor4", action="openclose"})

shogundot1( 1,11, {target="floor5", action="openclose"})
shogundot1( 2,11, {target="floor6", action="openclose"})
shogundot1( 3,11, {target="door2", action="openclose"})
shogundot1( 4,11, {target="floor7", action="openclose"})

shogundot1(16,10, {target="floor8", action="openclose"})
shogundot1(16,11, {target="floor9", action="openclose"})
shogundot1(17,11, {target="floor10", action="openclose"})
shogundot1(18,11, {target="door3", action="openclose"})
shogundot1(18,10, {target="floor11", action="openclose"})

shogundot1(14,3, {target="floor12", action="openclose"})
shogundot1(14,4, {target="floor13", action="openclose"})
shogundot1(15,3, {target="floor14", action="openclose"})
shogundot1(15,4, {target="floor15", action="openclose"})
shogundot1(16,3, {target="door4", action="openclose"})
shogundot1(16,4, {target="floor16", action="openclose"})

renderLine( 0, "~######~~~~~~~#####~")
renderLine( 1, "~#----#~O~~####---##")
renderLine( 2, "~#--+-#O O~#-+-+---#")
renderLine( 3, "###--+## ~~#-#-+-#-#")
renderLine( 4, "#-+--+-# ~  -#-+-#-#")
renderLine( 5, "#-#+##-# ~ #---+-+-#")
renderLine( 6, "#------# O ##---####")
renderLine( 7, "# ######   ### #####")
renderLine( 8, "#-----#~~~##-------#")
renderLine( 9, "##+-+-  ~~#---#+#+-#")
renderLine(10, "#--++##    -++---+-#")
renderLine(11, "#----#~~~~#--###---#")
renderLine(12, "##############~#####")

oxyd_shuffle()

set_actor("ac-blackball", 4.5, 6.5)
