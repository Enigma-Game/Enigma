-- Sok it to 'em
-- A public domain Sokoban level adapted for Enigma
-- 
-- Copyright (c) 2002 Nat Pryce

INNER_FLOOR = "fl-plank"
OUTER_FLOOR = "fl-water"
WALL = "st-rock2"


function trigger( id )
    return function( x, y )
        set_floor( INNER_FLOOR, x, y )
        set_item( "it-trigger", x, y, {target=id, action="openclose"} )
    end
end

function door( id, type )
   return function( x, y )
      set_floor( INNER_FLOOR, x, y )
      set_stone( "st-door_a", x, y, {name=id,type=type} )
   end
end


level = {
    "   x x x            ",
    "  #A#H#F#           ",
    "  #.....#########   ",
    " xG.....#....#..##  ",
    "  #.+...#...+.+..#  ",
    " xD..+..#..+.##..Cx ",
    "  #.++..##+.+....#  ",
    "  ##.#..##.#######  ",
    "  ##.#..##....a~e#  ",
    " xB..#+.......b~f#  ",
    "  #...++......c~g#  ",
    " xE.....##o#..d~h#  ",
    "  ################  ",
}

cells = {}
for i,id in pairs({"a","b","c","d","e","f","g","h"}) do
    cells[id] = trigger(strupper(id))
end
for i,id in pairs({"A","H","F"}) do
    cells[id] = door( id, "h" )
end
for i,id in pairs({"G","D","B","E","C"}) do
    cells[id] = door( id, "v" )
end
cells["."] = function( x, y )
    set_floor( INNER_FLOOR, x, y )
end
cells["#"] = function( x, y )
    set_floor( INNER_FLOOR, x, y )
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
oxyd_default_flavor = "a"
for y,line in pairs(level) do
    for x = 1,strlen(line) do
        cell = strchar(strbyte(line,x))
        cells[cell]( x-1, y-1 )
    end
end
oxyd_shuffle()















