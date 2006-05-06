-- Lasers and Mirrors, a tutorial-level for Enigma
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
cells["!"]=cell{floor="fl-leavesb"}
stones["!"]=cell{stone="st-likeoxyda"}
stones["#"]=cell{stone="st-glass1"}
cells["#"]=cell{floor="fl-leaves"}
function par_laser(x,y)
  set_stone("st-laser-e",x,y,{on=TRUE})
end
stones["$"]=cell{parent={{par_laser}}}
cells["$"]=cell{floor="fl-leavesd4"}
cells["%"]=cell{floor="fl-leavesd3"}
stones["%"]=cell{parent={{file_oxyd,"d"}}}
stones["&"]=cell{stone="st-mirror-p/"}
cells["&"]=cell{floor="fl-leavesd2"}
cells["'"]=cell{floor="fl-leavesd1"}
stones["'"]=cell{stone="st-mirror-p\\m"}
items["!"]=cell{item="it-document"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==14) and (y==6) then
  n="ac14x6"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!#!!!!!!!!!!!!",
       "!!!!!!!#&!!!!!!!!!!!",
       "!!!!!!!############!",
       "!!!!!!$##'!!!!!!!!!!",
       "!!!!#####!!!!!!!!!!!",
       "!!!!!!%##&!!!!!!!!!!",
       "!!!!!!!############!",
       "!!!!!!!#'!!!!!!!!!!!",
       "!!!!!!!#!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "              !     ",
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
         "            !       ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!      %           !",
        "!    ##########    !",
        "!  ###        ###  !",
        "! ##            ##%!",
        "! #              # !",
        "! # $  &  '      # !",
        "! #              # !",
        "! ##            ##%!",
        "!  ###        ###  !",
        "!    ##########    !",
        "!      %           !",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
--SetAttrib(enigma.GetStone(4,6),"init",TRUE)
SetAttrib(enigma.GetItem(12,6),"text","The bright light beam is a laser beam.   Don't get to close to it!   But it has its usages...    You can use the mirrors to direct the laser to its targets.   From these, only the light-gray ones are movable, just try it!")
oxyd_shuffle()
