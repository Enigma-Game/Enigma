-- Hail Storm
-- A level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Licensed under the GPL version 2.

function wormhole(n)
    return "wormhole-"..n
end

function fill_floor(name, x0,y0, w,h, attrs)
    if x0 == nil then x0 = 0 end
    if y0 == nil then y0 = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    for y=y0,y0+h-1 do
	for x=x0,x0+w-1 
            do set_floor(name, x, y, attrs) 
        end
    end
end

function random_real( min, max, precision )
    precision = precision or 100
    return random(min*precision,max*precision-1)/precision
end

create_world( 20, 14 )
fill_floor( "fl-ice_001" )

draw_stones( "st-rock5", {0,0}, {0,1}, level_height )
draw_stones( "st-rock5", {level_width-1,0}, {0,1}, level_height )

fill_stones( "st-oneway-s", 1, 0, level_width-2, 2 )
fill_floor( "fl-abyss", 1,level_height-2, level_width-2, 2 )

if not difficult then -- easy
    MIN_FORCE = 15
    MAX_FORCE = 20
else -- difficult
    MIN_FORCE = 20
    MAX_FORCE = 35
end

for x=1,level_width-2 do
    draw_floor( "fl-gradient", {x,0}, {0,1}, 2,
                {type=FLAT_FORCE_S, 
                 force=random_real(MIN_FORCE,MAX_FORCE)} ) 
    set_actor( "ac-killerball", x+0.5, 0.5, {controllers=0} )
    set_item( "it-wormhole", x, level_height-1,
              {name=wormhole(x),
               targetx=x+0.5, targety=0.5, 
               range=0.5,force=1} )
end



for y=level_height-8,level_height-4,2 do
    oxyd( 0, y )
    oxyd( level_width-1, y )
end
oxyd_shuffle()

set_actor( "ac-blackball", level_width/2, 7.5 )

















