-- Blocks and Slopes, a tutorial-level for Enigma
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
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==2) and (y==2) then
  n="ac2x2"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
items["!"]=cell{item="it-document"}
stones["#"]=cell{stone="st-wood"}
cells["&"]=cell{floor="fl-leavesc3"}
cells["'"]=cell{floor="fl-leavesc4"}
cells["("]=cell{floor="fl-leavesd2"}
cells[")"]=cell{floor="fl-gradient6"}
cells["*"]=cell{floor="fl-gradient1"}
cells["+"]=cell{floor="fl-leavesd4"}
cells[","]=cell{floor="fl-gradient4"}
stones["$"]=cell{stone="st-door_a"}
stones["%"]=cell{parent={{file_oxyd,"d"}}}
cells["-"]=cell{floor="fl-gradient3"}
cells["."]=cell{floor="fl-gradient5"}
items["#"]=cell{item="it-trigger"}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!###!),,,,----.!###!",
       "!$!&!*),,,---.*!$!&!",
       "!%!'!**),,--.**!%!'!",
       "!#!#!***!!!!***!#!#!",
       "!$!&!***!!!!***!$!&!",
       "!!!!!***!!!!***!!!!!",
       "!%!'!***!!!!***!%!'!",
       "!#!#!***!!!!***!#!#!",
       "!#!(#+!#!!!!#!(#+!#!",
       "!#%!!!'#!!!!#%!!!'#!",
       "!#######!!!!#######!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "  !                 ",
          "                    ",
          "                    ",
          "                    ",
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
         "  !              !  ",
         "                    ",
         "                    ",
         "                 #  ",
         "                    ",
         "                    ",
         "      !             ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!   !          !   !",
        "!   !          !   !",
        "!   !          !   !",
        "!   !   !$$!   !   !",
        "!   !   !  !   !   !",
        "! # !   !  !   !   !",
        "!   !   !  !   !   !",
        "!   !   !  !   !   !",
        "!       !  !       !",
        "!       !  !       !",
        "!       !  !       !",
        "!!!!!!!!!%%!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(2,3),"text","This wooden block is movable and heavy enough to hold down the trigger on the right.   Try to push it there!")
SetAttrib(enigma.GetItem(6,9),"text","Remember?  There are different kinds of floors.  And these are steep...")
SetAttrib(enigma.GetItem(17,3),"text","Is there more one can do with wooden blocks?   Yes!  Just explore the next levels.")
SetAttrib(enigma.GetItem(17,6),"invisible",FALSE)
Signal ("it(17 6)","st(10 4)")
Signal ("it(17 6)","st(9 4)")
oxyd_shuffle()
