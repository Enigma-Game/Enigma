-- Time is Money
-- A level for Enigma
-- 
-- Copyright (c) 2003 Nat Pryce
-- Licensed under the GPL, v2.0 or greater


function floor( floor_type, attribs )
    return function( x, y )
        set_floor( %floor_type, x, y, %attribs or {} )
    end
end

function stone( stone_type, attribs )
    return function( x, y )
        set_stone( %stone_type, x, y, %attribs or {} )
    end
end

function item( item_type, attribs )
    return function( x, y )
        set_item( %item_type, x, y, %attribs or {} )
    end
end

function actor( actor_type, attribs )
    return function( x, y )
        set_actor( %actor_type, x+0.5, y+0.5, %attribs
or {} )
    end
end

function gradient( gradient_type )
    return function( x, y )
        set_floor( "fl-gradient", x, y, {type=%gradient_type} )
    end
end

function create_world_from_map( tiles, map )
    create_world( strlen(map[1]), getn(map) )
    
    for y,line in map do
        for x = 1,strlen(line) do
            local tile = strchar(strbyte(line,x))
            local constructors = tiles[tile]
            
            if constructors then
                for i,constructor in constructors do
                    constructor(x-1,y-1)
                end
            else
                error("invalid tile identifier " ..
tile )
            end
        end
    end
end

default_floor = floor("fl-normal")


oxyd_default_flavor = "b"

tiles = {
    [" "] = { abyss },
    ["S"] = { floor("fl-normal") },
    ["W"] = { gradient(SLOPE_S) },
    ["X"] = { gradient(SLOPE_N) },
    ["A"] = { gradient(SLOPE_E) },
    ["D"] = { gradient(SLOPE_W) },
    ["Q"] = { gradient(SLOPE_LARGE_SE) },
    ["Z"] = { gradient(SLOPE_LARGE_NE) },
    ["E"] = { gradient(SLOPE_LARGE_SW) },
    ["C"] = { gradient(SLOPE_LARGE_NW) },
    ["q"] = { gradient(SLOPE_SMALL_SE) },
    ["z"] = { gradient(SLOPE_SMALL_NE) },
    ["e"] = { gradient(SLOPE_SMALL_SW) },
    ["c"] = { gradient(SLOPE_SMALL_NW) },
    ["o"] = { default_floor, 
              actor("ac-blackball",{player=0}) },
    ["#"] = { default_floor, 
              stone("st-rock1") },
    ["="] = { default_floor, 
              stone("st-coinslot",
              {target="laser",action="onoff"})},
    ["$"] = { default_floor,
              item("it-coin2") },
    ["x"] = { default_floor, oxyd },
    [">"] = { default_floor, 
              stone("st-laser",
                    {name="laser",on=TRUE,dir=EAST})},
}

create_world_from_map( tiles, {
"                                       ",
" ##################################### ",
" #QWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWE# ",
" =ASoS$SSSSSSSSSSSSSSSSSSSSSSSSSSSSSD# ",
" #ZXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXzSD# ",
" #################################ASD# ",
"                                 #ASD# ",
" #x#x# ###########################ASD# ",
" #QWE# #QWWWWWWWWWWWWWWWWWWWWWWWWWqSD# ",
" #ASD# #ASSSSSSSSSSSSSSSSSSSSSSSSSSSD# ",
" #ASD# #AScXXXXXXXXXXXXXXXXXXXXXXXXXC# ",
" >ASD# #ASD########################### ",
" #ASD# #ASD#                           ",
" #ASD# #ASD########################### ",
" #ASD# #ASeWWWWWWWWWWWWWWWWWWWWWWWWWE# ",
" #ASD# #ASSSSSSSSSSSSSSSSSSSSSSSSSSSD# ",
" #ASD# #ZXXXXXXXXXXXXXXXXXXXXXXXXXzSD# ",
" #ASD# ###########################ASD# ",
" #ASD#                           #ASD# ",
" #ASD#############################ASD# ",
" #ASeWWWWWWWWWWWWWWWWWWWWWWWWWWWWWqSD# ",
" #ASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSD# ",
" #ZXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXC# ",
" ##################################### ",
"                                       ",
})

oxyd_shuffle()
