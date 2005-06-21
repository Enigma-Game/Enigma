--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 4*19+1, 2*12+1)
oxyd_default_flavor = "a"

SetDefaultAttribs("it-wormhole", {range=1.0, strength=10})

fill_floor( "fl-himalaya")

function draw_level_screen( x0, y0, tx, ty)
	draw_border( "st-rock5", x0*19, y0*12, 20, 13)
	
	set_item("it-wormhole", x0*19+3,  y0*12+9, {targetx=tx*19+15,targety=ty*12+5})

	oxyd( x0*19+17, y0*12+3)

	draw_stones( "st-wood", {x0*19+8, y0*12+1}, {0,1}, 11)
end

draw_level_screen( 0,0, 1,0)
draw_level_screen( 1,0, 2,0)
draw_level_screen( 2,0, 3,0)
draw_level_screen( 3,0, 3,1)
draw_level_screen( 0,1, 0,0)
draw_level_screen( 1,1, 0,1)
draw_level_screen( 2,1, 1,1)
draw_level_screen( 3,1, 2,1)

oxyd_shuffle()

set_actor("ac-blackball", 10, 5)
