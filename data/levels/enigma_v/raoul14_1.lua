-- Oxydmoron, a level for Enigma
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
--cells["#"]=cell{floor="fl-sand"}

stones["O"]=cell{parent={{file_oxyd,"b"}}}
stones["S"]=cell{stone="st-black4"}
stones["W"]=cell{stone="st-white4"}
stones["Y"]=cell{stone="st-yinyang1"}

level={"                    ",
       " ####          #### ",
       " #  ##       ###### ",
       " #   #####   # #### ",
       " ##      #   #  #   ",
       "  ##    #### #  #   ",
       "   #  ########  #   ",
       "   #  # ####    ##  ",
       "   #  #   #      ## ",
       " #### #   #####   # ",
       " ######       ##  # ",
       " ####          #### ",
       "                    "}
if not difficult then
stmap={"                    ",
       " WWWW       O       ",
       " W  WW    O  WW Y   ",
       " W O WWWWW   W      ",
       " WW O    W   W  S   ",
       "  WW O       W  S   ",
       "   W  SS    WW  S   ",
       "   W  S       O SS  ",
       "   W  S   S    O SS ",
       "      S   SSSSS O S ",
       "   Y SS  O    SS  S ",
       "       O       SSSS ",
       "                    "}
else
stmap={"                    ",
       " WWWW O     O       ",
       " W  WW    O  WW Y   ",
       " W O WWWWW   W      ",
       " WW O    W   W  S   ",
       "  WW O       W  S   ",
       " O W  SS    WW  S O ",
       "   W  S       O SS  ",
       "   W  S   S    O SS ",
       "      S   SSSSS O S ",
       "   Y SS  O    SS  S ",
       "       O     O SSSS ",
       "                    "}
end

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_item("it-yinyang",3,10)
set_item("it-yinyang",16,2)

--Random first Actor--does not ork !?
--x1=random(2)-1
--set_item("it-document",2,10,{text=x1})

--if x1==0 then
-- set_actor("ac-blackball",2.5,10.5,{player=0,name="bb"})
-- set_actor("ac-whiteball",17.5,2.5,{player=1,name="wb"})
--elseif x1==1 then
-- set_actor("ac-blackball",2.5,10.5,{player=1,name="bb"})
-- set_actor("ac-whiteball",17.5,2.5,{player=0,name="wb"})
--end

set_actor("ac-blackball",2.5,10.5,{player=0,name="bb"})
set_actor("ac-whiteball",17.5,2.5,{player=1,name="wb"})

oxyd_shuffle()

