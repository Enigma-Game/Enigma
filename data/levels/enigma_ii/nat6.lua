-- Twelve Doors
-- A public domain Sokoban level adapted for Enigma
-- 
-- Copyright (c) 2002 Nat Pryce


trigger_count = 0
function trigger( x, y )
    door_id = "door"..trigger_count
    trigger_count = trigger_count + 1
    
    set_floor( "fl-rough", x, y )
    set_item( "it-trigger", x, y, {target=door_id, action="openclose"} )
end

door_count = 0
function door( x, y )
    id = "door"..door_count
    door_count = door_count + 1
    
    set_floor( "fl-rough", x, y )
    doorh( x, y, {name=id} )
end


level = {
"                     ################# ",
"                   ###o##::::::::::::# ",
"                   #....#++++++++++++# ",
"                   #.a..#::::::::::::# ",
"                   #.aa.#::::::::::::# ",
"                   ##.a.###b######=### ",
"                    #,###..a.#   #=#   ",
"                   ##...b..a.#   #=#   ",
"                   #..a.#..a.#   #=#   ",
"                ####.a..#....#   #=#   ",
"                #s.#..a.#....#   #=#   ",
"                #..@....#....#   #=#   ",
"                ##############   #=#   ",
"                                 #=#   ",
"                                 #=#   ",
"                                 #=#   ",
"                               #x#=#x# ",
"                               x:::::x ",
"                               #:::::# ",
"                               x::^::x ",
"                               #:::::# ",
"                               x:::::x ",
"                               #x###x# ",
"                                       ",
"                                       ",
}

cells = {}
cells[" "] = function( x, y )
    set_floor( "fl-leaves", x,y )
end
cells["#"] = function( x, y )
    set_floor( "fl-leaves", x, y )
    set_stone( "st-greenbrown", x, y )
end
cells["@"] = function( x, y )
    set_floor( "fl-gray", x, y )
    set_stone( "st-greenbrown_move", x, y )
end
cells["o"] = function( x, y )
    set_floor( "fl-gray", x, y )
    set_actor( "ac-blackball", x+0.5, y+0.5 )
end
cells["x"] = function( x, y )
    set_floor( "fl-gray", x, y )
    oxyd(x,y)
end
cells["."] = function( x, y )
    set_floor( "fl-gray", x, y )
end
cells["a"] = function( x, y )
    set_floor( "fl-gray", x, y )
    set_stone( "st-brownie", x, y )
end
cells[","] = function( x, y )
    set_floor( "fl-normal", x, y )
end
cells["b"] = function( x, y )
    set_floor( "fl-normal", x, y )
    set_stone( "st-brownie", x, y )
end
cells[":"] = function( x, y )
    set_floor( "fl-rough", x, y )
end
cells["+"] = trigger
cells["="] = door
cells["^"] = function( x, y )
    set_floor( "fl-rough", x, y )
    set_stone( "st-laser", x, y, {name="laser", dir=NORTH, on=1} )
end
cells["s"] = function( x, y )
    set_floor( "fl-hay", x, y )
    set_stone( "st-switch", x, y, {on=1,target="laser", action="onoff"} )
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















