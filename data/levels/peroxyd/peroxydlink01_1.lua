--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

create_world( 20, 13)

fill_floor( "fl-wood", 0, 0, 20, 13)

draw_border( "st-greenbrown")

function oxyd_base( color, x, y)
	stone = format( "st-%s4", color)
	set_stone( stone, x-1, y-1)
	set_stone( stone, x,   y-1)
	set_stone( stone, x+1, y-1)
	set_stone( stone, x-1, y)
	set_stone( stone, x+1, y)
	set_stone( stone, x-1, y+1)
	set_stone( stone, x,   y+1)
	set_stone( stone, x+1, y+1)
	oxyd( x, y)
end

oxyd_default_flavor = "d"

oxyd_base( "white", 2, 2)
oxyd_base( "white", 8, 2)
oxyd_base( "black",11, 2)
oxyd_base( "black",17, 2)
oxyd_base( "black", 5, 6)
oxyd_base( "white",14, 6)
oxyd_base( "black", 2,10)
oxyd_base( "black", 8,10)
oxyd_base( "white",11,10)
oxyd_base( "white",17,10)

oxyd_shuffle()

set_item("it-yinyang", 8,6)
set_item("it-yinyang", 11,6)

set_actor("ac-blackball", 11.5, 6.5)
set_actor("ac-whiteball",  8.5, 6.5)











