-- Venice I, a level for Enigma
-- Copyright (C) 2006 Raoul
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

cells[" "]=cell{floor="fl-water"}
cells["#"]=cell{floor="fl-concrete"}

stones["O"]=cell{parent={{file_oxyd,"b"}}}
stones["G"]=cell{stone="st-glass"}

-- Random Bridge Setup:
r=random(2)
if r == 1 then
 stones["S"]=cell{stone="st-black4"}
 stones["W"]=cell{stone="st-white4"}
else
 stones["W"]=cell{stone="st-black4"}
 stones["S"]=cell{stone="st-white4"}
end

level={"                    ",
       "  ### ###  ### ###  ",
       "  ################  ",
       "  ### ###  ### ###  ",
       "   #   #    #   #   ",
       "  ### ###  ### ###  ",
       "  ################  ",
       "  ### ###  ### ###  ",
       "   #   #    #   #   ",
       "  ### ###  ### ###  ",
       "  ################  ",
       "  ### ###  ### ###  ",
       "                    "}

stmap={"                    ",
       "  GOG GOG  GOG GGG  ",
       "  O WWW SSSS WWW G  ",
       "  GSG GWG  GSG GWG  ",
       "   S   W    S   W   ",
       "  GSG GWG  GSG GWG  ",
       "  O SSS WWWW SSS O  ",
       "  GWG GSG  GWG GSG  ",
       "   W   S    W   S   ",
       "  GWG GSG  GWG GSG  ",
       "  G WWW SSSS WWW O  ",
       "  GGG GOG  GOG GOG  ",
       "                    "}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_item("it-yinyang",2,10)
set_item("it-yinyang",17,2)

set_item("it-ring",3,11)
set_item("it-ring",16,1)

set_item("it-cherry",3,10)
set_item("it-cherry",16,2)

set_actor("ac-whiteball",3.5,10.5,{player=1,name="wb"})
set_actor("ac-blackball",16.5,2.5,{player=0,name="bb"})

oxyd_shuffle()
