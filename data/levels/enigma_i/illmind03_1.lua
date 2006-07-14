-- Hanging In The Balance, a level for Enigma
-- Copyright (C) 2004 illmind
-- Licensed under GPL v2.0 or above
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
cells["!"]=cell{floor="fl-normal"}
stones["!"]=cell{stone="st-break_bolder"}
cells["#"]=cell{floor="fl-water"}
stones["#"]=cell{parent={{file_oxyd,"a"}}}
stones["$"]=cell{stone="st-death"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==6) and (y==11) then
  n="ac6x11"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
cells["$"]=cell{floor="fl-stwood"}
items["!"]=cell{item="it-document"}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!################!!",
       "!!#!!!!!!!!!!!!!!#!!",
       "!!#!######!#####!#!!",
       "!!#!#!!!!!$!!!!#!#!!",
       "!!#!#!########!#!#!!",
       "!!#!#!!!!!$!!!!#!#!!",
       "!!#!############!#!!",
       "!!#!!!!!!!!!!!!!!#!!",
       "!!########!#######!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!"}
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
          "      !             ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                  ! ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!                  !",
        "! #              # !",
        "!                  !",
        "!                  !",
        "!     $            !",
        "!   #    # #   #   !",
        "!                  !",
        "!               $  !",
        "!                  !",
        "! #              # !",
        "!        $         !",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(18,8),"text","... Hanging In The Balance ...")
oxyd_shuffle()



























