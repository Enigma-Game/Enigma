--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)
fill_floor("fl-himalaya")

oxyd_default_flavor = "d"

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         set_stone("st-door_a",i-1,line,{type="v"})
      elseif c == "O" then
         oxyd( i-1, line)
      elseif c == "+" then
         set_stone("st-shogun-s",i-1,line)
      end
    end
end

set_stone("st-door_a", 1,10, {type="v", name="door1"})
set_stone("st-door_a", 6, 9, {type="v", name="door2"})
set_stone("st-door_a", 6, 5, {type="v", name="door3"})
set_stone("st-door_a", 9,10, {type="v", name="door4"})
set_stone("st-door_a",17, 8, {type="v", name="door5"})
set_stone("st-door_a", 6, 2, {type="v", name="door6"})

shogundot1(17,1, {target="door1", action="openclose"})
shogundot1(16,2, {target="door2", action="openclose"})
shogundot1(17,2, {target="door3", action="openclose"})
shogundot1(18,2, {target="door4", action="openclose"})
shogundot1(17,3, {target="door5", action="openclose"})
shogundot1(18,3, {target="door6", action="openclose"})

renderLine( 0, "####################")
renderLine( 1, "#OOOOO#     #   #  #")
renderLine( 2, "#       ## ##  +   #")
renderLine( 3, "#OOOOO###  #  + #+ #")
renderLine( 4, "#######   ## ##+# ##")
renderLine( 5, "#         #   +  + #")
renderLine( 6, "# #  ######   #    #")
renderLine( 7, "#  ####   #######  #")
renderLine( 8, "#  #  # #       # ##")
renderLine( 9, "# ##    #  ######  #")
renderLine(10, "# #  #### ##    ## #")
renderLine(11, "#    #        #    #")
renderLine(12, "####################")

oxyd_shuffle()

set_actor("ac-blackball", 17.5, 7.5)
