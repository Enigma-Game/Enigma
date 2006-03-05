-- Bounce Out
-- Copyright (C) JAHR ihope
-- Licensed under GPL v2.0 or above ?
-- !!BBE105!! Do not delete this line

Require("levels/lib/ant.lua")

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
cells["!"]=cell{floor="fl-leaves"}
stones["!"]=cell{stone="st-greenbrown"}
items["!"]=cell{item="it-hollow"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==4) and (y==3) then
  n="ac4x3"
  p=0
  mf=1.66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
items["#"]=cell{item="it-magicwand"}
stones["#"]=cell{stone="st-oneway-e"}
cells["#"]=cell{floor="fl-space"}
function ac_whiteball_small(x,y)
n=""
p=0
f=0
 if (x==13) and (y==6) then
  n="ac13x6"
  p=0
  mf=1.66666666666667
 end
 if (x==14) and (y==6) then
  n="ac14x6"
  p=0
  mf=1.66666666666667
 end
 if (x==15) and (y==6) then
  n="ac15x6"
  p=0
  mf=1.66666666666667
 end
 if (x==16) and (y==6) then
  n="ac16x6"
  p=0
  mf=1.66666666666667
 end
 if (x==17) and (y==6) then
  n="ac17x6"
  p=0
  mf=1.66666666666667
 end
 if (x==18) and (y==6) then
  n="ac18x6"
  p=0
  mf=1.66666666666667
 end
set_actor("ac-whiteball-small",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["#"]=cell{parent={{ac_whiteball_small}}}
stones["$"]=cell{stone="st-actorimpulse"}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!#######",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "    !               ",
          "                    ",
          "                    ",
          "             ###### ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    "}
  itmap={"                    ",
         " !                ! ",
         "                    ",
         "                    ",
         "                  ! ",
         "                    ",
         "       #            ",
         "                    ",
         "                  ! ",
         "                    ",
         "                    ",
         " !                ! ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!           !!!!!!!!",
        "!           #      $",
        "!           !!!!!!!!",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
oxyd_shuffle()


























