-- Bye!, a tutorial-level for Enigma
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
cells["#"]=cell{floor="fl-leavesc4"}
cells["$"]=cell{floor="fl-leavesc3"}
cells["%"]=cell{floor="fl-leavesc2"}
cells["&"]=cell{floor="fl-leavesc1"}
items["!"]=cell{item="it-document"}
stones["#"]=cell{parent={{file_oxyd,"a"}}}
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
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!#%!!!!!",
       "!!!#%!!!!!!!!$&!!!!!",
       "!!!$&!!!!!!!!!!!!!!!",
       "!!!!!!!!#%!!!!!!!!!!",
       "!!!!!!!!$&!!!!#%!!!!",
       "!!!!!#%!!!!!!!$&!!!!",
       "!!!!!$&!!!#%!!!!!!!!",
       "!!#%!!!!!!$&!!!!#%!!",
       "!!$&!!!!!!!!!!!!$&!!",
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
         "                    ",
         "                    ",
         "                    ",
         "     ! !            ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  #",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!!!!!!#!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(5,6),"text","But before this, take a look at the final tutorial level Jacob devised for you.   See it as an exam, if you want to.   It's quite difficult, so don't despair if you don't get through it at the first time.   Bye!")
SetAttrib(enigma.GetItem(7,6),"text","Well, I hope you enjoyed this tutorial-pack.   You will see most of these levels a second time in the Enigma packs, together with many new and much more difficult ones.   Just don't give up on them!")
oxyd_shuffle()

