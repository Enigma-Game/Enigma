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

cells["f"]=cell{floor="fl-gray"}
cells[" "]=cell{floor="fl-abyss"}

stones["x"]=cell{parent={{file_oxyd,"a"}}}
stones["w"]=cell{stone="st-oneway_white-e"}
stones["W"]=cell{stone="st-oneway_white-n"}
stones["s"]=cell{stone="st-oneway_black-e"}
stones["S"]=cell{stone="st-oneway_black-s"}
stones["r"]=cell{stone="st-oneway_black-w"}

level={
"                    ",
"                    ",
" ffffffffffffffffff ",
" f f f  f  f  f f f ",
" f fff  ffff  fff f ",
" f                f ",
" ffffffffffffffffff ",
" f                f ",
" f fff  ffff  fff f ",
" f f f  f  f  f f f ",
" ffffffffffffffffff ",
"                    ",
"                    "
}

stmap={
"                    ",
"x        xx        x",
"    s    s     s    ",
"   W    W     W     ",
"                    ",
"                    ",
"  r              w  ",
"                    ",
"                    ",
"   S    S     S     ",
"    w    w     w    ",
"x        xx        x",
"                    "
}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_actor("ac-blackball", 16.5,6.5, {player=0})  
set_actor("ac-whiteball", 3.5,6.5, {player=1})  
set_item("it-yinyang",3,6)
set_item("it-yinyang",16,6)

oxyd_shuffle()

