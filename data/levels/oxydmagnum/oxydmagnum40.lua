-- NAME, a level for Enigma
-- Copyright (C) YEAR AUTHOR
-- Licensed under GPL v2.0 or above 

Require("levels/lib/ant.lua")

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end

levelh=13
levelw=20

cells={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}

cells["r"]=cell{floor="fl-rock"}
cells["g"]=cell{floor="fl-gray"}
cells[" "]=cell{floor="fl-abyss"}

stones["s"]=cell{stone="st-stoneimpulse"}
stones["h"]=cell{stone="st-stoneimpulse-hollow"}

level={
"                    ",
"                    ",
" rrrrrrrrrrrrrrrrrr ",
" rrrrrrrrrrrrrrrrrr ",
" rrrrrrggggggrrrrrr ",
" rrrrrrg    grrrrrr ",
" rrrrrrg    grrrrrr ",
" rrrrrrg    grrrrrr ",
" rrrrrrggggggrrrrrr ",
" rrrrrrrrrrrrrrrrrr ",
" rrrrrrrrrrrrrrrrrr ",
"                    ",
"                    "
}

stmap={
"                    ",
"                    ",
" shh            hhs ",
"   s hshhhhhhsh s   ",
"   hhh        hhh   ",
"     h        h     ",
"  shhs        shhs  ",
"     h        h     ",
"   hhh        hhh   ",
"   s hshhhhhhsh s   ",
" shh            hhs ",
"                    ",
"                    "
}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_actor("ac-whiteball-small",7.5,4.5,{player=0})
set_actor("ac-whiteball-small",7.5,8.5,{player=0})
set_actor("ac-whiteball-small",12.5,4.5,{player=0})
set_actor("ac-whiteball-small",12.5,8.5,{player=0})

set_item("it-hollow",2,3)
set_item("it-hollow",2,9)
set_item("it-hollow",17,3)
set_item("it-hollow",17,9)
