-- Timers, a tutorial-level for Enigma
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
 if (x==2) and (y==10) then
  n="ac2x10"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
--stones["#"]=cell{stone="st-timer"}
stones["#"]=cell{}
cells["&"]=cell{floor="fl-leavesc3"}
cells["'"]=cell{floor="fl-leavesc4"}
stones["$"]=cell{stone="st-oneway-w"}
stones["%"]=cell{stone="st-oneway-e"}
stones["&"]=cell{stone="st-oneway-n"}
stones["'"]=cell{stone="st-oneway-s"}
cells["("]=cell{floor="fl-leavesd2"}
cells[")"]=cell{floor="fl-leavesd1"}
cells["*"]=cell{floor="fl-leavesd3"}
cells["+"]=cell{floor="fl-leavesd4"}
stones["("]=cell{parent={{file_oxyd,"d"}}}
level={"!!!!!!!!!!!!!!!#!!!!",
       "!###############%!!!",
       "!################*!!",
       "!#################!!",
       "!#################!!",
       "!#$!&########$!!(+!!",
       "!#!!!########!!!!!!!",
       "!#%!'########%!!)*!!",
       "!#################!!",
       "!#################!!",
       "!################+!!",
       "!###############$!!!",
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
          "  !                 ",
          "                    ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "  ! !               ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!     $ $ % $ $    !",
        "!     !&!'!&!'!    !",
        "!     $ $ $ $ $    (",
        "!     !'!&!'!&!!   !",
        "!     % $ $ $  !   !",
        "!  #  !'!&!&!  %   !",
        "!     $ % $ $  !   !",
        "!     !&!'!'!'!!   !",
        "!     $ $ $ % $    (",
        "!     !'!&!&!'!    !",
        "!     $ $ $ $ $    !",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(2,9),"text","This oscillating stone is a timer-stone. You cannot interact with it, but nevertheless be aware when you see one: After some time-period anything might happen...")
--SetAttrib(enigma.GetStone(3,6),"interval",1000)
set_stone("st-timer", 3, 6, {interval = 15})
SetAttrib(enigma.GetStone(3,6),"on",TRUE)
SetAttrib(enigma.GetStone(3,6),"loop",TRUE)
SetAttrib(enigma.GetItem(4,9),"text","The stones with arrows are one-way-stones. As you might have guessed, you can only pass them in one direction.                Mazes are cherished elements of Enigma - you will see lots of them.")
Signal ("st(3 6)","st(9 2)")
Signal ("st(3 6)","st(11 2)")
Signal ("st(3 6)","st(13 8)")
Signal ("st(3 6)","st(11 4)")
Signal ("st(3 6)","st(11 6)")

set_item("it-document", 16, 6, {text = "Well done!     When you have finished this level, let's play some example levels from the actual Enigma packs!"})

oxyd_shuffle()

