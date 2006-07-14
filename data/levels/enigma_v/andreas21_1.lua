-- Ice-Bomb, a level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above
-- Generated with the help of BBE 1.05

Require("levels/lib/ant.lua")
Require("levels/lib/andreas_itemfreeze.lua")

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end
levelh=13
levelw=20
enigma.FlatForce=30
enigma.SlopeForce=30
enigma.ElectricForce=30
cells={}
items={}
actors={}
stones={}
actors[" "]=cell{}
stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}
cells["!"]=cell{floor="fl-mortar"}
stones["!"]=cell{stone="st-grate3"}
stones["#"]=cell{stone="st-wood"}
items["!"]=cell{item="it-extinguisher"}
items["#"]=cell{item="it-blackbomb"}
stones["$"]=cell{parent={{file_oxyd,"c"}}}
items["$"]=cell{item="it-document"}
stones["%"]=cell{stone="st-death"}
items["%"]=cell{item="it-ring"}
stones["&"]=cell{stone="st-stoneimpulse_movable"}
items["&"]=cell{item="it-key_a"}
stones["'"]=cell{stone="st-swap"}
items["'"]=cell{item="it-puller-e"}
stones["("]=cell{stone="st-grate1"}
items["("]=cell{item="it-magicwand"}
stones[")"]=cell{stone="st-shogun-sl"}
items[")"]=cell{item="it-spring2"}
stones["*"]=cell{parent={{parent_itemfreeze, ""}}}


--stones["!"]=cell{stone="st-swap"}
--stones["#"]=cell{stone="st-swap"}
--stones["$"]=cell{stone="st-swap"}
--stones["%"]=cell{stone="st-swap"}
--stones["&"]=cell{stone="st-swap"}

--stones["'"]=cell{stone="st-swap"}
--stones["("]=cell{stone="st-swap"}
--stones[")"]=cell{stone="st-swap"}
--stones["*"]=cell{stone="st-swap"}



function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==12) and (y==6) then
  n="ac12x6"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
stones["+"]=cell{stone="st-stone_break"}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "            !       ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "    !  %  (         ",
         "                    ",
         "                    ",
         "    #  &  )         ",
         "                    ",
         "                    ",
         "    $  '            ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!                  !",
        "!                  !",
        "!   #  %  (    !!!!!",
        "!              !   !",
        "!              !   !",
        "!      &  )    + $ !",
        "!              !   !",
        "!              !   !",
        "!   $  '  *    !!!!!",
        "!                  !",
        "!                  !",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(4,9),"text","Remember Seed Puzzle 2? There was a similar problem. Or was it Portable Laser...?")
oxyd_shuffle()

itemfreeze_init(0, 6, 0.01)

enigma.SetCompatibility("oxyd1")

