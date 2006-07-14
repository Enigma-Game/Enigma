-- Formentera, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
--------------------------------------------------------------------
-- Function to fill an area with random floor tiles
function fill_randomfloor(names, x0,y0, w,h)
    if x0 == nil then x0 = 0 end
    if y0 == nil then y0 = 0 end
    if w  == nil then w  = level_width end
    if h  == nil then h  = level_height end
    for y=y0,y0+h-1 do
        for x=x0,x0+w-1 do
            set_floor(names[random(getn(names))], x, y) 
        end
    end
end
---------------------------------------------------------------------
-- Environment and actor
CreateWorld(20,25)
draw_border("st-glass")
fill_randomfloor({"fl-normal","fl-inverse","fl-space"})
set_floor("fl-normal",1,1)
oxyd_default_flavor="c"
oxyd(0,1)
oxyd(19,1)
oxyd(0,12)
oxyd(19,12)
oxyd(0,23)
oxyd(19,23)
oxyd_shuffle()
set_actor("ac-blackball",1.5,1.5,{player="0"})



























