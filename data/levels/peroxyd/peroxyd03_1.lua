--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 58
levelh = 13

create_world( levelw, levelh)
draw_border("st-greenbrown")
fill_floor("fl-sand", 0,0,levelw,levelh-3)
fill_floor("fl-water", 0,levelh-3,levelw,3)

function createOxyd( x, y)
	set_stone( "st-metal", x-1, y)
	set_stone( "st-metal", x+1, y)
	set_stone( "st-metal", x,   y+1)
	oxyd(x,y)
end

set_stone( "st-greenbrown", 1, 4)
set_stone( "st-greenbrown", 2, 4)
set_stone( "st-greenbrown", 3, 4)
set_stone( "st-greenbrown", 3, 3)

createOxyd( 3,8)
createOxyd( 10,3)
createOxyd( 12,8)
createOxyd( 17,5)
createOxyd( 24,7)
createOxyd( 32,2)
createOxyd( 40,8)
createOxyd( 51,4)

oxyd_shuffle()

enigma.AddConstantForce(0,30)

set_actor("ac-blackball", 2, 2)












