-- Sentry Duty
-- A level for Enigma
--
-- Copyright (c) 2005 Nat Pryce
-- Licensed under the GPL version 2.


set_oxyd = oxyd


function oxyd( type, color )
    return function( x, y, tiles )
        return set_oxyd( x, y, type or oxyd_default_flavor, color )
    end
end

function nothing(x, y, tiles)
    -- create nothing
end

function horizontal_bolder(x,y,tiles)
    if random() >= 0.5 then
        return set_stone( "st-bolder-w", x, y )
    else
    	return set_stone( "st-bolder-e", x, y )
    end
end

function vertical_bolder(x,y,tiles)
    if random() >= 0.5 then
        return set_stone( "st-bolder-n", x, y )
    else
    	return set_stone( "st-bolder-s", x, y )
    end
end

function checkerboard_floor( type1, type2, attribs1, attribs2 )
    return function( x, y, tiles )
        if mod(x,2) == mod(y,2) then
            return set_floor( type1, x, y, attribs1 or {} )
        else
            return set_floor( type2, x, y, attribs2 or {} )
        end
    end
end

function floor( floor_type, attribs )
    return function( x, y, tiles )
        return set_floor( floor_type, x, y, attribs or {} )
    end
end

function stone( stone_type, attribs )
    return function( x, y, tiles )
        return set_stone( stone_type, x, y, attribs or {} )
    end
end

function item( item_type, attribs )
    return function( x, y, tiles )
        return set_item( item_type, x, y, attribs or {} )
    end
end

function actor( actor_type, attribs )
    return function( x, y, tiles )
        return set_actor( actor_type, x+0.5, y+0.5, attribs or {} )
    end
end

function gradient( gradient_type )
    return function( x, y, tiles )
        return set_floor( "fl-gradient", x, y, {type=gradient_type} )
    end
end

function inherit(tile_type)
    return function( x, y, tiles )
        return create_tile( tiles, x, y, tile_type )
    end
end

function group( array, constructor )
    return function( x, y, tiles )
        object = constructor( x, y, tiles )
    	tinsert( array, object )
    	return object
    end
end

function difficulty( d, constructor )
    if options.Difficulty == d then
    	return constructor
    else
    	return nothing
    end
end

function create_world_from_map( tiles, map )
    create_world( strlen(map[1]), getn(map) )
    
    for y = 1,getn(map) do
        local line = map[y]
        for x = 1,strlen(line) do
            local tile = strchar(strbyte(line,x))
            create_tile( tiles, x-1, y-1, tile )
        end
    end
end

function create_tile( tiles, x, y, tile_type )
    local constructors = tiles[tile_type]

    if constructors then
	for i = 1,getn(constructors) do
	    constructors[i](x,y,tiles)
	end
    else
	error("invalid tile identifier " .. tile_type )
    end
end


tiles = {}
tiles["."] = {floor("fl-leaves")}
tiles["#"] = {inherit("."), stone("st-stone1")}
tiles["x"] = {inherit("."), oxyd("d")}
tiles[" "] = {floor("fl-water")}
tiles["-"] = {difficulty(1,floor("fl-water")), difficulty(2,floor("fl-swamp"))}
tiles["+"] = {difficulty(1,inherit("#")), 
              difficulty(2,inherit("-"))}
tiles["@"] = {inherit("."), difficulty(1,actor("ac-top", {range=4, force=10})),
                            difficulty(2,actor("ac-top", {range=4, force=15}))}



create_world_from_map( tiles, {
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                     --- ---  --- ---                     ",
"                     +x+ +x+  +x+ +x+                     ",
"                     +.+ +.+  +.+ +.+                     ",
"                     +.+ +.+  +.+ +.+                     ",
"                   ###@###@####@###@###                   ",
"                   #..................#                   ",
"               -+++#..................#+++-               ",
"               -x..@..................@..x-               ",
"               -+++#..................#+++-               ",
"                   #..................#                   ",
"                   #..................#                   ",
"                   #..................#                   ",
"               -+++#..................#+++-               ",
"               -x..@..................@..x-               ",
"               -+++#..................#+++-               ",
"                   #..................#                   ",
"                   ###@###@####@###@###                   ",
"                     +.+ +.+  +.+ +.+                     ",
"                     +.+ +.+  +.+ +.+                     ",
"                     +x+ +x+  +x+ +x+                     ",
"                     --- ---  --- ---                     ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          ",
"                                                          "
})					     

set_actor("ac-blackball", level_width/2, level_height/2, {player=0})

oxyd_shuffle()





























