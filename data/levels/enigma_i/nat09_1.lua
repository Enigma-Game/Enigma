-- Balancing Act
-- A level for Enigma
-- Copyright (c) 2002 Nat Pryce
-- License: GPL v2.0 or above


screen_width = 20
screen_height = 13

screens_x = 2
screens_y = 1

create_world( (screens_x-1)*(screen_width-1) + screen_width,
              (screens_y-1)*(screen_height-1) + screen_height )


fill_floor( "fl-abyss")
fill_floor( "fl-water", 3, 3, level_width-6, level_height-6 )

gradient( 5, 5, SLOPE_LARGE_SE )
gradient( 5, level_height-6, SLOPE_LARGE_NE )
gradient( level_width-6, 5, SLOPE_LARGE_SW )
gradient( level_width-6, level_height-6, SLOPE_LARGE_NW )
for x = 6,level_width-7 do
    gradient( x, 5, SLOPE_S )
    gradient( x, level_height-6, SLOPE_N )
end
for y = 6,level_height-7 do
    gradient( 5, y, SLOPE_E )
    gradient( level_width-6, y, SLOPE_W )
    for x = 6, level_width-7 do
        set_floor( "fl-normal", x, y )
    end
end


for x = 3,level_width-4 do
    gradient( x, 1, SLOPE_N )
    set_floor( "fl-normal", x, 2 )
    gradient( x, 3, SLOPE_S )
    gradient( x, level_height-4, SLOPE_N )
    set_floor( "fl-normal", x, level_height-3 )
    gradient( x, level_height-2, SLOPE_S )
end

for y = 3,level_height-4 do
    gradient( 1, y, SLOPE_W )
    set_floor( "fl-normal", 2, y )
    gradient( 3, y, SLOPE_E )
    gradient( level_width-4, y, SLOPE_W )
    set_floor( "fl-normal", level_width-3, y )
    gradient( level_width-2, y, SLOPE_E )
end

set_floor( "fl-normal", 2, 2 )
gradient( 3, 3, SLOPE_LARGE_SE )
gradient( 1, 1, SLOPE_SMALL_NW )
gradient( 2, 1, SLOPE_N )
gradient( 1, 2, SLOPE_W )

set_floor( "fl-normal", 2, level_height-3 )
gradient( 3, level_height-4, SLOPE_LARGE_NE )
gradient( 1, level_height-2, SLOPE_SMALL_SW )
gradient( 1, level_height-3, SLOPE_W )
gradient( 2, level_height-2, SLOPE_S )

set_floor( "fl-normal", level_width-3, 2 )
gradient( level_width-4, 3, SLOPE_LARGE_SW )
gradient( level_width-2, 1, SLOPE_SMALL_NE )
gradient( level_width-3, 1, SLOPE_N )
gradient( level_width-2, 2, SLOPE_E )

set_floor( "fl-normal", level_width-3, level_height-3 )
gradient( level_width-4, level_height-4, SLOPE_LARGE_NW )
gradient( level_width-2, level_height-2, SLOPE_SMALL_SE )
gradient( level_width-3, level_height-2, SLOPE_S )
gradient( level_width-2, level_height-3, SLOPE_E )

gradient( 7, 3, SLOPE_LARGE_SW )
gradient( 8, 3, SLOPE_LARGE_SE )
gradient( 7, 4, SLOPE_SMALL_SW )
gradient( 8, 4, SLOPE_SMALL_SE )

gradient( level_width-9, level_height-5, SLOPE_SMALL_NW )
gradient( level_width-8, level_height-5, SLOPE_SMALL_NE )
gradient( level_width-9, level_height-4, SLOPE_LARGE_NW )
gradient( level_width-8, level_height-4, SLOPE_LARGE_NE )


for sx = 1, screens_x-1 do
    x = sx*(screen_width-1)
    oxyd( x, 0 )
    oxyd( x, level_height-1 )
end
for sy = 0, screens_y-1 do
    y = sy*(screen_height-1) + screen_height/2
    oxyd( 0, y )
    oxyd( level_width-1, y )
end
oxyd( 1, 0 )
oxyd( 1, level_height-1 )
oxyd( level_width-2, 0 )
oxyd( level_width-2, level_height-1 )
oxyd_shuffle()

set_actor( "ac-blackball", 6.5, 6.5 )
















