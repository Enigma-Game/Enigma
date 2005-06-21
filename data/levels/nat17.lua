-- Electrickery
-- A level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Bug fix by Sven Siggelkow
-- Licensed under the GPL version 2.

WALL = "st-wood_001"

function draw_checkerboard_floor( name1, name2, x, y, w, h, attrs)
    for i=1,h do
        for j=1,w do
	    if mod(i,2) == mod(j,2) then
	        set_floor( name1, x+j-1, y+i-1, attrs )
	    else
	        set_floor( name2, x+j-1, y+i-1, attrs )
            end
	end
    end
end

create_world( 20, 13 )
draw_checkerboard_floor( "fl-bluegray", "fl-red",  0, 0, 4,  13 )
draw_checkerboard_floor( "fl-sahara", "fl-tigris", 4, 0, 12, 13 )
draw_checkerboard_floor( "fl-bluegray", "fl-red",  16, 0, 4,  13 )
fill_floor( "fl-water", 8, 4, 4, 5 )

draw_border( WALL )
draw_stones( "st-oneway_white-w", {4,1}, {0,1}, 5 )
draw_stones( "st-oneway_white-e", {4,7}, {0,1}, 5 )
draw_stones( "st-oneway_white-w", {15,1}, {0,1}, 5 )
draw_stones( "st-oneway_white-e", {15,7}, {0,1}, 5 )
set_stone( WALL, 3, 6 )
set_stone( WALL, 16, 6 )
set_stone( "st-chargeplus", 4, 6 )
set_stone( "st-chargeminus", 15, 6 )

oxyd( 0, 3 )
oxyd( 0, 6 )
oxyd( 0, 9 )
oxyd( 19, 3 )
oxyd( 19, 6 )
oxyd( 19, 9 )
oxyd_shuffle()

set_actor( "ac-blackball", 5.5, 6.5 )
set_actor( "ac-whiteball", 14.5, 6.5, {player=0,controllers=0} )
