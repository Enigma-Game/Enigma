-- Sisyphus
-- A level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Bug fix by Sven Siggelkow
-- Licensed under the GPL version 2.

function fill_gradient( x0, y0, width, height, type )
    for y=y0,y0+height-1 do
	for x=x0,x0+width-1 do 
            gradient( x, y, type )
        end
    end
end

-----------------------------------------------------------------------------
-- Scenery
-----------------------------------------------------------------------------

create_world( 39, 13 )
fill_floor( "fl-water")

FLAT_FLOOR = "fl-gray"

-- West end barrier
fill_gradient( 1,2, 1, level_height-4, SLOPE_E )
gradient( 1, 1, SLOPE_LARGE_SE )
gradient( 1, level_height-2, SLOPE_LARGE_NE )

-- West flat area
fill_floor( FLAT_FLOOR, 2,2, 4, level_height-4 )
fill_gradient( 2,1, 4, 1, SLOPE_S )
fill_gradient( 2,level_height-2, 4, 1, SLOPE_N )

-- West slope
fill_gradient( 6,2, 10, level_height-4, SLOPE_W )
fill_gradient( 6,1, 9, 1, SLOPE_S_FORCE_W )
fill_gradient( 6,level_height-2, 9, 1, SLOPE_N_FORCE_W )
gradient( 14, 1, SLOPE_LARGE_SW )
gradient( 14, level_height-2, SLOPE_LARGE_NW )

-- West top barrier
fill_gradient( 15, 2, 1, level_height-4, SLOPE_E )
gradient( 15, 1, SLOPE_LARGE_SE )
gradient( 15, level_height-2, SLOPE_LARGE_NE )

-- Top flat area

fill_floor( FLAT_FLOOR, 16, 2, 7, level_height-4 )
fill_gradient( 16, 1, 7, 1, SLOPE_S )
fill_gradient( 16, level_height-2, 7, 1, SLOPE_N )

-- East top barrier
fill_gradient( 23, 2, 1, level_height-4, SLOPE_W )
gradient( 23, 1, SLOPE_LARGE_SW )
gradient( 23, level_height-2, SLOPE_LARGE_NW )

-- East slope
fill_gradient( 24,2, 10, level_height-4, SLOPE_E )
fill_gradient( 25,1, 9, 1, SLOPE_S_FORCE_E )
fill_gradient( 25,level_height-2, 9, 1, SLOPE_N_FORCE_E )
gradient( 24, 1, SLOPE_LARGE_SE )
gradient( 24, level_height-2, SLOPE_LARGE_NE )

-- East flat area
fill_floor( FLAT_FLOOR, 34, 2, 3, level_height-4 )
fill_gradient( 34,1, 3, 1, SLOPE_S )
fill_gradient( 34,level_height-2, 3, 1, SLOPE_N )

-- East end barrier
fill_gradient( 37, 2, 1, level_height-4, SLOPE_W )
gradient( 37, 1, SLOPE_LARGE_SW )
gradient( 37, level_height-2, SLOPE_LARGE_NW )


-----------------------------------------------------------------------------
-- Oxyds

OXYD_WALL = "st-white4"

function set_oxyd( x, y )
    oxyd( x, y )
    set_stones( OXYD_WALL, {{x-1,y},{x+1,y},{x,y-1},{x,y+1}} )
end

function set_oxyds( xs )
    for i,x in xs do
        set_oxyd( x, 1 )
        set_oxyd( x, level_height-2 )
    end
end

set_oxyds{ 1, level_width/2-3, level_width/2+3, level_width-2 }
oxyd_shuffle()

-----------------------------------------------------------------------------
-- Actors
-----------------------------------------------------------------------------

set_actor( "ac-blackball", 3.5, 5.5 )
set_actor( "ac-whiteball", 3.5, 7.5, {player=0,controllers=0} )















