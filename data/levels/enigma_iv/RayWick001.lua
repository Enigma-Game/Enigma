-- Doors Galore
-- Created by Ray Wick
--

LAND_FLOOR = "fl-brick"
SOFT_FLOOR = "fl-hay"
WATER_FLOOR = "fl-water"
WALL = "st-brick"

level = {
    "#@###S#L##########@#",
    "#6#~~~~~@~~~~ > #B1#",
    "C ~~~ ~~~~~~#5# D  $",
    "#     ~~~~~~~ E H G#",
    "#b+++ ~~~~~~@ #4# 7@",
    "#e    ~~~!~~+ ! #8##",
    "#  a ~~~~~~~~ !   ##",
    "# ~~~~~~~~~~~ #c???#",
    "# ~~~@~~d~~ * #??+ #",
    "# ~~~~~~  + + ###  #",
    "# ~~~~~~  +++ 3 #9I#",
    "# >  + x     +# < 2@",
    "#@##########!!###A##"
}

cells = {}
cells["!"] = function( x, y )
--    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-stoneimpulse", x, y )
end

cells["#"] = function( x, y )
--    set_floor( LAND_FLOOR, x, y )
    set_stone( WALL, x, y )
end
cells["*"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_actor( "ac-blackball", x+0.5, y+0.5 )
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
cells["$"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-coinslot", x, y, {action="callback", target="funcc"})
end
cells["<"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-oneway-w", x, y)
end
cells["v"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-oneway-s", x, y)
end
cells[">"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-oneway-e", x, y)
end
cells["^"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-oneway-n", x, y)
end
cells["?"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_item( "it-coin1", x, y )
end
cells["x"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_item( "it-blackbomb", x, y )
end
cells[" "] = function( x, y )
    set_floor( LAND_FLOOR, x,y )
end
cells["1"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorh(x, y, {name="door1"} )
end
cells["2"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorv(x, y, {name="door2"} )
end
cells["3"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorv(x, y, {name="door3"} )
end
cells["4"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorh(x, y, {name="door4"} )
end
cells["5"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorh(x, y, {name="door5"} )
end
cells["6"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorh(x, y, {name="door6"} )
end
cells["7"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorv(x, y, {name="door7"} )
end
cells["8"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorh(x, y, {name="door8"} )
end
cells["9"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    doorh(x, y, {name="door9"} )
end
cells["A"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-key_a", x, y, {action="openclose", target="door1"})
end
cells["B"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-key_b", x, y, {action="openclose", target="door2"})
end
cells["C"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-key_c", x, y, {action="openclose", target="door3"})
end
cells["D"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-floppy", x, y, {action="openclose", target="door4"})
end
cells["E"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-floppy", x, y, {action="openclose", target="door5"})
end
cells["F"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-floppy", x, y, {action="openclose", target="door6"})
end
cells["G"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-floppy", x, y, {action="openclose", target="door7"})
end
cells["H"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-floppy", x, y, {action="openclose", target="door8"})
end
cells["I"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-floppy", x, y, {action="openclose", target="door9"})
end
cells["L"] = function( x, y )
    set_floor( SOFT_FLOOR, x, y )
    set_attrib(laser(x, y, FALSE, SOUTH), "name", "laser")
end
cells["S"] = function( x, y )
    set_floor( SOFT_FLOOR, x, y )
    set_stone( "st-switch", x, y, {target="laser", action="onoff", on=1 })
end
cells["a"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
--    set_stone( "st-wood", x, y )
    set_item( "it-key_a", x, y )
end
cells["b"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
--    set_stone( "st-wood", x, y )
    set_item( "it-key_b", x, y )
end
cells["c"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-wood", x, y )
    set_item( "it-key_c", x, y )
end
cells["d"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
--    set_stone( "st-wood", x, y )
    set_item( "it-floppy", x, y )
end
cells["e"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
    set_stone( "st-wood", x, y )
    set_item( "it-floppy", x, y )
end
cells["f"] = function( x, y )
    set_floor( LAND_FLOOR, x, y )
--    set_stone( "st-wood", x, y )
    set_item( "it-floppy", x, y )
end

create_world( strlen(level[1]), getn(level) )
fill_floor (LAND_FLOOR)

for y,line in level do
    for x = 1,strlen(line) do
        cell = strchar(strbyte(line,x))
        cells[cell]( x-1, y-1 )
    end
end
oxyd_shuffle()
through=0
function funcc()
    if through==1 then
        through=0
    end
    if through==0 then
        SendMessage("door3", "openclose")
        SendMessage("door6", "openclose")
    end
    through=through+1
end















