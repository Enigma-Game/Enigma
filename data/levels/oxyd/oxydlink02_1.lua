--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 39, 13)
oxyd_default_flavor = "d"

fill_floor( "fl-wood")

stone = "st-rock3"

draw_border( stone)

draw_stones( stone, {19, 1}, {0,1}, 11)

function oxbase1( x, y, a, b)
	oxyd( x, y)
	draw_stones( stone, {x-2,y-1}, {1,0}, 4)
	draw_stones( stone, {x-2,y+1}, {1,0}, 4)
	set_stone( stone, x+1, y)
	doorv(x-1,y, {name="door"..a})
	set_item("it-trigger", x-2,y, {invisible=1, action="open", target="door"..b})
end

function oxbase2( x, y, a, b)
	oxyd( x, y)
	draw_stones( stone, {x-1,y-1}, {1,0}, 4)
	draw_stones( stone, {x-1,y+1}, {1,0}, 4)
	set_stone( stone, x-1, y)
	doorv(x+1,y, {name="door"..a})
	set_item("it-trigger", x+2, y, {invisible=1, action="open", target="door"..b})
end


oxbase1( 7, 4, 1, 5)
oxbase1( 7, 8, 2, 6)
oxbase2(11, 4, 3, 7)
oxbase2(11, 8, 4, 8)
oxbase1(26, 4, 5, 1)
oxbase1(26, 8, 6, 2)
oxbase2(30, 4, 7, 3)
oxbase2(30, 8, 8, 4)

oxyd(17, 4)
oxyd(17, 8)
oxyd(36, 4)
oxyd(36, 8)

oxyd_shuffle()

set_item( "it-yinyang", 9, 6)
set_item( "it-yinyang",28, 6)
set_actor("ac-blackball", 9.5, 6.5)
set_actor("ac-whiteball",28.5, 6.5)











