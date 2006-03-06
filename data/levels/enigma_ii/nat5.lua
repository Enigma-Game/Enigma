-- Push Them In
-- A GSokoban level adapted for Enigma
--
-- GSokoban Copyright (c) 1999, 2000 Andreas Persenius
-- This conversion Copyright (c) 2002 Nat Pryce

INNER_FLOOR = "fl-normal"
OUTER_FLOOR = "fl-leaves"
WALL = "st-rock2"

level = {
    "                    ",
    "          ########  ",
    "          #.....o#  ",
    "          #.+#+.##  ",
    "   ########.+..+#   ",
    "   #x~~...##+.+.#   ",
    " #######..#.+.#.### ",
    " #x~~~~..##.+..+..# ",
    " ##x~~~....+..+...# ",
    " #x~~~~..########## ",
    " #########          ",
    "                    ",
    "                    "
}

cells = {}
cells["."] = function( x, y )
    set_floor( INNER_FLOOR, x, y )
end
cells["#"] = function( x, y )
    set_stone( WALL, x, y )
end
cells["o"] = function( x, y )
    set_floor( INNER_FLOOR, x, y )
    set_actor( "ac-blackball", x+0.5, y+0.5 )
end
cells["x"] = function( x, y )
    set_floor( INNER_FLOOR, x, y )
    oxyd(x,y)
end
cells["+"] = function( x, y )
    set_floor( INNER_FLOOR, x, y )
    set_stone( "st-wood", x, y )
end
cells["~"] = function( x, y )
    set_floor( "fl-water", x, y )
end
cells[" "] = function( x, y )
    set_floor( OUTER_FLOOR, x,y )
end


create_world( strlen(level[1]), getn(level) )
for y,line in level do
    for x = 1,strlen(line) do
        cell = strchar(strbyte(line,x))
        cells[cell]( x-1, y-1 )
    end
end
oxyd_shuffle()















