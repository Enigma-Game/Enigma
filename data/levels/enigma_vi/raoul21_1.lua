-- Cold way, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above

Require("levels/lib/ant.lua")
Require("levels/lib/andreas_itemfreeze.lua")

enigma.ConserveLevel = TRUE

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end

levelh=13
levelw=20

cells={}
items={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}

cells["!"]=cell{floor="fl-black"}
cells["-"]=cell{floor="fl-light"}
cells[" "]=cell{floor="fl-water"}

stones["!"]=cell{stone="st-glass"}
stones["C"]=cell{stone="st-coffee"}
stones["L"]=cell{stone="st-likeoxydc"}
stones["#"]=cell{parent={{file_oxyd,"c"}}}
stones["$"]=cell{parent={{parent_itemfreeze, ""}}}

items["p"]=cell{item="it-pin"}
items["e"]=cell{item="it-extralife"}
items["f"]=cell{item="it-flagblack"}

level={"!!!      !!      !!!",
       "!!!      !!      !!!",
       "!!!              !!!",
       "                    ",
       "-- -     !!     - --",
       "--  -   !!!!   -  --",
       "-   -- !!!!!! --   -",
       "--  -   !!!!   -  --",
       "-- -     !!     - --",
       "                    ",
       "!!!              !!!",
       "!!!      !!      !!!",
       "!!!      !!      !!!"}

itmap={"                    ",
       " pe              ee ",
       " e                e ",
       "                    ",
       "                    ",
       "                    ",
       "         ee         ",
       "                    ",
       "                    ",
       "                    ",
       " ee               e ",
       " f               ee ",
       "                    "}

stmap={"!!!!!!!!!!!!!!!!!!!!",
       "!        !!        !",
       "! $     !  !     $ !",
       "!      !    !      !",
       "!     !#C  C#!     !",
       "!   C  C    C !    !",
       "! #   L      L   # !",
       "!   C  C    C  C   !",
       "!     !#C  C#!     !",
       "!      !    !      !",
       "!       !  !     $ !",
       "! $      !!        !",
       "!!!!!!!!!!!!!!!!!!!!"}
        
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)

oxyd_shuffle()

itemfreeze_init(0,0,0.01)

set_actor("ac-blackball",1.5,1.5,{player=0})

--Doors
set_item("it-trigger",5,5,{action="callback", target="openway1", invisible=true})
function openway1()
    enigma.KillStone(3,12)
    set_item("it-trigger",3,12,{action="callback", target="openway3", invisible=true})
end

set_item("it-trigger",5,7,{action="callback", target="openway2", invisible=true})
function openway2()
    enigma.KillStone(9,11)
end

function openway3()
    enigma.KillStone(10,11)
end

set_item("it-trigger",14,7,{action="callback", target="openway4", invisible=true})
function openway4()
    enigma.KillStone(16,0)
    set_item("it-trigger",16,0,{action="callback", target="openway5", invisible=true})
end

function openway5()
    enigma.KillStone(11,2)
    set_item("it-trigger",11,2,{action="callback", target="openway6", invisible=true})
end

function openway6()
    set_floor("fl-black",10,3)
end

--traps:
set_item("it-trigger",5,6,{action="callback", target="trap1", invisible=true})
function trap1()
    set_stone("st-glass",4,6)
    --enigma.KillItem(5,6)
    --set_item("it-spoon",5,6)
end

set_item("it-trigger",14,6,{action="callback", target="trap2", invisible=true})
function trap2()
    set_stone("st-glass",15,6)
    --enigma.KillItem(14,6)
    --set_item("it-spoon",14,6)
end
