-- Dead Ball Walking, a level for Enigma
-- Copyright (C) 2004 illmind
-- Licensed under GPL v2.0 or above 
-- !!BBE105!! Do not delete this line
-- Rand-Bug behoben, Raoul

Require("levels/lib/ant.lua")

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end
levelh=13
levelw=19
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
cells["!"]=cell{floor="fl-normal"}
cells["i"]=cell{floor="fl-abyss"}
stones["!"]=cell{stone="st-death"}
stones["#"]=cell{parent={{file_oxyd,"d"}}}
items["!"]=cell{item="it-extralife"}
items["#"]=cell{item="it-document"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==17) and (y==11) then
  n="ac17x11"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
level={"!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i",
       "!!!!!!!!!!!!!!!!!!!i"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                 !  ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "          !         ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                #   ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!! ",
        "!                 ! ",
        "! !!! ! !!! ! !!! ! ",
        "! !#! ! !#  ! !#! ! ",
        "! ! ! ! !!!!! ! ! ! ",
        "! !   !#  !#    ! ! ",
        "! ! ! ! ! ! ! ! ! ! ",
        "! !   !#  !#!   ! ! ",
        "! ! ! ! !!! ! ! ! ! ",
        "! !#! !  #!   !#! ! ",
        "! !!! ! !!!!! !!! ! ",
        "!                 ! ",
        "!!!!!!!!!!!!!!!!!!! "}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(16,11),"text","... Dead Ball Walking ...")
oxyd_shuffle()


























