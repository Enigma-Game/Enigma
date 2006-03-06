-- Frozen Magnets, a level for Enigma
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
cells["!"]=cell{floor="fl-white"}
stones["!"]=cell{stone="st-rock1"}
items["!"]=cell{item="it-magnet-on"}
stones["#"]=cell{stone="st-shogun-l"}
items["#"]=cell{item="it-easykeepstone"}
stones["$"]=cell{parent={{file_oxyd,"c"}}}
function ac_whiteball(x,y)
n=""
p=0
f=0
 if (x==7) and (y==1) then
  n="ac7x1"
  p=0
  mf=1,66666666666667
 end
 if (x==10) and (y==6) then
  n="ac10x6"
  p=1
  mf=1,66666666666667
 end
set_actor("ac-whiteball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end

function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==7) and (y==1) then
  n="ac2x2"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end

actors["!"]=cell{parent={{ac_whiteball}}}
actors["b"]=cell{parent={{ac_blackball}}}
stones["%"]=cell{parent={{parent_itemfreeze, ""}}}
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
          "       b            ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "          !         ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "                    ",
         "    ### #### ###    ",
         "    #          #    ",
         "  !              !  ",
         "    #          #    ",
         "    ### #### ###    ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"  !!!!!!!!!!!!!!!!  ",
        " !                ! ",
        "!         %        !",
        "!   !!!!!!!!!!!!   !",
        "!  !###$####$###!  !",
        "!  !#          #!  !",
        "!  !$          $!  !",
        "!  !#          #!  !",
        "!  !###$####$###!  !",
        "!   !!!!!!!!!!!!   !",
        "!       %          !",
        " !                ! ",
        "  !!!!!!!!!!!!!!!!  "}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(2,6),"force",10)
SetAttrib(enigma.GetItem(17,6),"force",10)
oxyd_shuffle()

itemfreeze_init(0,0,0.01)
-- some performance-thingies
itemfreeze_test_for_explosion = 0
itemfreeze_test_for_fire = 0
itemfreeze_test_for_existence = 0





