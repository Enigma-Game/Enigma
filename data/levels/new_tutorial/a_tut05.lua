-- Items, a tutorial-level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above 
-- Created with the help of BBE 1.05

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
stones["!"]=cell{stone="st-likeoxyda"}
cells["#"]=cell{floor="fl-leavesb"}
cells["$"]=cell{floor="fl-leavesc1"}
cells["%"]=cell{floor="fl-leavesc2"}
items["!"]=cell{item="it-document"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==3) and (y==5) then
  n="ac3x5"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
items["#"]=cell{item="it-key_a"}
items["$"]=cell{item="it-key_b"}
items["%"]=cell{item="it-key_c"}
cells["&"]=cell{floor="fl-leavesc3"}
cells["'"]=cell{floor="fl-leavesd1"}
cells["("]=cell{floor="fl-leavesc4"}
cells[")"]=cell{floor="fl-leavesd2"}
stones["#"]=cell{stone="st-key_b"}
stones["$"]=cell{stone="st-door_a"}
cells["*"]=cell{floor="fl-leavesd3"}
stones["%"]=cell{stone="st-key_a"}
cells["+"]=cell{floor="fl-leavesd4"}
stones["&"]=cell{stone="st-key_c"}
stones["'"]=cell{parent={{file_oxyd,"d"}}}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!#####!###!###!####!",
       "!#$!&#!###!###!#$&#!",
       "!#!!!)!###!###!+!!#!",
       "!#%!!!!!&#!#$!!!!!#!",
       "!##!'#!*!#!#!'!*!!#!",
       "!#$!&#!#!###!#!#!!#!",
       "!#!!!#!#!)!+!#!#!!#!",
       "!#!!!#!#%!!!(#!#!!#!",
       "!#!!!#!###!###!#!!#!",
       "!#%!(#!###!###!#%(#!",
       "!#####!#######!####!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "   !                ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "   !                ",
         "                    ",
         "                    ",
         "                !   ",
         "   #                ",
         "   $                ",
         "   %                ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!     !   !   !    !",
        "!     !   !   !    !",
        "!     !   !   !    !",
        "!     #   $   &  ' !",
        "!     !   !   !  ' !",
        "!     !   !   !    !",
        "!     !   !   !  ' !",
        "!     $   %   $  ' !",
        "!     !   !   !    !",
        "!     !   !   !    !",
        "!     !   !   !    !",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(3,3),"text","Use the right mouse-button to rotate your inventory,  it's the bar at the bottom of the screen.   And guess what these three items down there can be used do.")
SetAttrib(enigma.GetItem(16,6),"text","There are lots of different items to use in Enigma, as well as special floors and stones.   Let's explore some more of the actual Enigma levels!   And if you have problems with a level - take a look at the manual:   For some levels, there are hints.")
Signal ("st(6 4)","st(6 8)")
Signal ("st(10 8)","st(10 4)")
Signal ("st(14 4)","st(14 8)")
oxyd_shuffle()
