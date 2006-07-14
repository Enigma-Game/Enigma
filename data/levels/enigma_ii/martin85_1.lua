--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 20
levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "d"

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c =="#" then
         set_floor("fl-samba",i-1,line)
         set_stone("st-door_a",i-1,line,{type="v"})
      elseif c == "O" then
         set_floor("fl-samba",i-1,line)
         oxyd( i-1, line)
      elseif c == "." then
         set_floor("fl-water",i-1,line)
      elseif c == "+" then
         set_floor("fl-samba",i-1,line)
         set_stone("st-shogun-s",i-1,line)
      elseif c == "x" then
         set_floor("fl-samba",i-1,line)
         set_stone("st-grate1",i-1,line)
      elseif c == "-" then
         set_floor("fl-samba",i-1,line)
      elseif c == " " then
         set_floor("fl-water",i-1,line)
      end
    end
end

set_stone("st-door_a", 8,6, {type="v", name="door1"})
set_stone("st-door_a", 9,6, {type="v", name="door2"})
set_stone("st-door_a",10,6, {type="v", name="door3"})
set_stone("st-door_a",11,6, {type="v", name="door4"})

set_floor("fl-bridge", 8,6, {type="x", name="bridge1"})
set_floor("fl-bridge", 9,6, {type="x", name="bridge2"})
set_floor("fl-bridge",10,6, {name="bridge3", type="x"})
set_floor("fl-bridge",11,6, {name="bridge4", type="x"})

shogundot1( 4,4, {target="bridge1", action="openclose"})
shogundot1( 3,5, {target="bridge2", action="openclose"})
shogundot1( 5,5, {target="door2", action="openclose"})
shogundot1( 3,6, {target="door1", action="openclose"})
shogundot1( 5,6, {target="door4", action="openclose"})
shogundot1( 3,7, {target="door3", action="openclose"})
shogundot1( 5,7, {target="bridge3", action="openclose"})
shogundot1( 4,8, {target="bridge4", action="openclose"})

renderLine( 0, "                    ")
renderLine( 1, "                    ")
renderLine( 2, "  #####      #####  ")
renderLine( 3, " ##---##    ##---## ")
renderLine( 4, "##--+--##  ##--O--##")
renderLine( 5, "#--+++--#  #--O-O--#")
renderLine( 6, "#-#---#-%%%%-xO-Ox-#")
renderLine( 7, "#--+++--#  #--O-O--#")
renderLine( 8, "##--+--##  ##--O--##")
renderLine( 9, " ##---##    ##---## ")
renderLine(10, "  #####      #####  ")
renderLine(11, "                    ")
renderLine(12, "                    ")

oxyd_shuffle()


set_actor("ac-blackball", 7.5, 6.5)
















