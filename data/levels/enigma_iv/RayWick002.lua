-- Switch - Extra

-- Created by Ray Wick

--




LAND_FLOOR = "fl-wood"

SOFT_FLOOR = "fl-hay"

WATER_FLOOR = "fl-water"

WALL = "st-rock4"


function color_block( color, x, y)

       stone = format( "st-%s4", color)

       set_stone( stone, x, y)

end

level = {

    "#@#A######X###L###@#",

    "@W   V  @@  W   a V@",

    "@W   > ~~~~ <     V@",

    "@W   #~~~~~~#     V@",

    "@W   ~~~~~~~~     V@",

    "@W   ~~~~~~~~     V@",

    "#*   ~~~~M~~~   x .#",

    "#V+  ~~~~~~~~  #s+W#",

    "#V+  ~~~~~~~~  #xsW#",

    "#V+  #~~~~~~#  #MsW#",

    "#V+  < ~~~~ >  #xsW#",

    "#V+  V  +   W  #x+W#",

    "####################"

}


cells = {}

cells["#"] = function( x, y )

    set_stone( WALL, x, y )

end

cells["*"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_actor( "ac-blackball", x+0.5, y+0.5)

--    set_item( "it-yinyang", x, y+0.5 )

end

cells["."] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_actor( "ac-whiteball", x+0.5, y+0.5 )

--    set_item( "it-yinyang", x, y+0.5 )

end

cells["@"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    oxyd(x,y)

end

cells["+"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_stone( "st-wood", x, y )

end

cells["~"] = function( x, y )

    set_floor( WATER_FLOOR, x, y )

end

cells["<"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_stone( "st-oneway-w", x, y)

end

cells[">"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_stone( "st-oneway-e", x, y)

end

cells["s"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_item( "it-blackbomb", x, y )

    set_stone( "st-wood", x, y )

end

cells["x"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_item( "it-blackbomb", x, y )

end

cells[" "] = function( x, y )

    set_floor( LAND_FLOOR, x,y )

end

cells["A"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_stone( "st-key_a", x, y, {target="laser", action="onoff" })

end

cells["L"] = function( x, y )

    set_floor( SOFT_FLOOR, x, y )

    set_attrib(laser(x, y, FALSE, SOUTH), "name", "laser")

end

cells["M"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    set_stone( "st-pmirror", x, y, {movable=1, transparent=0, orientation=2 })

end

cells["V"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    color_block( "black", x, y)

end

cells["W"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

    color_block( "white", x, y)

end

cells["X"] = function( x, y )

    set_stone( "st-bombs", x, y )

end

cells["a"] = function( x, y )

    set_floor( LAND_FLOOR, x, y )

--    set_stone( "st-wood", x, y )

    set_item( "it-key_a", x, y )

end



create_world( strlen(level[1]), getn(level) )
fill_floor (LAND_FLOOR)

enigma.TwoPlayerGame = TRUE

for y,line in pairs(level) do

    for x = 1,strlen(line) do

        cell = strchar(strbyte(line,x))

        cells[cell]( x-1, y-1 )

    end

end

oxyd_shuffle()

oxyd_shuffle()


















