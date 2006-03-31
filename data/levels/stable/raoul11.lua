-- Ghost Islands, a level for Enigma
-- Copyright (C) 2006 Raoul
-- Licensed under GPL v2.0 or above
 
Require("levels/lib/ant.lua")
Require("levels/lib/libterrain.lua")

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end

display.SetFollowMode(display.FOLLOW_SCROLLING)

levelh=13
levelw=20

cells={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}

cells[" "]=cell{floor="fl-water"}
cells["#"]=cell{floor="fl-gray"}

cells["a"]=cell{floor="fl-gradient1"}
cells["b"]=cell{floor="fl-gradient2"}
cells["c"]=cell{floor="fl-gradient3"}
cells["d"]=cell{floor="fl-gradient4"}

cells["e"]=cell{floor="fl-gradient5"}
cells["f"]=cell{floor="fl-gradient6"}
cells["g"]=cell{floor="fl-gradient7"}
cells["h"]=cell{floor="fl-gradient8"}

cells["i"]=cell{floor="fl-gradient9"}
cells["j"]=cell{floor="fl-gradient10"}
cells["k"]=cell{floor="fl-gradient11"}
cells["l"]=cell{floor="fl-gradient12"}

stones["O"]=cell{parent={{file_oxyd,"b"}}}
stones["#"]=cell{stone="st-black4"}
stones["X"]=cell{stone="st-death_invisible"}
--stones["X"]=cell{stone="st-death"}

level={"####ef   eaaaaaf    ",
       "#   gh   c#####d    ",
       "#        c#####d    ",
       "#        c#####d    ",
       "eaaaaaaaai#####d    ",
       "c##############d    ",
       "c#######lbbbbbjd    ",
       "c#######d     ckaaf ",
       "c#######d     c###d ",
       "c#######d     c###d ",
       "c#######d     c###d ",
       "c#######d     gbbbh ",
       "gbbbbbbbh           "}
if not difficult then
stmap={"                    ",
       "          O   O     ",
       "                    ",
       "            X       ",
       "                    ",
       " O     O  O   O     ",
       "                    ",
       "                    ",
       "    X          O O  ",
       "                X   ",
       "               O O  ",
       " O     O            ",
       "                    "}
else
stmap={"                    ",
       "          O   O     ",
       "           X X      ",
       "            X       ",
       "           X X      ",
       " O     O  O   O     ",
       "  X   X             ",
       "   O O              ",
       "    X          O O  ",
       "   O O          X   ",
       "  X   X        O O  ",
       " O     O            ",
       "                    "}
end

create_world_by_map(level)
draw_map(0,0,stmap,stones)

-- Draw hills with libterrain.lua:
draw_full(1,5,7,7,1)
draw_full(10,1,5,5,1)
draw_full(15,8,3,3,1)

set_actor("ac-blackball",5,1,{player=0,name="bb"})

oxyd_shuffle()
