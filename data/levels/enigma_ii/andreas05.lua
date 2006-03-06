-- Friend or Foe?, a level for Enigma
-- Copyright (C) 2005 Andreas Lochmann
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
cells["!"]=cell{floor="fl-black"}
stones["!"]=cell{stone="st-rock5"}
stones["#"]=cell{stone="st-rock6"}
stones["$"]=cell{parent={{file_oxyd,"a"}}}
cells["#"]=cell{floor="fl-normal"}
cells["$"]=cell{floor="fl-light"}
cells["%"]=cell{floor="fl-bluegreen"}
cells["&"]=cell{floor="fl-red"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==6) and (y==6) then
  n="ac6x6"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
stones["%"]=cell{stone="st-rock7"}
function ac_top(x,y)
n=""
r=0
f=0
 init=false
 if (x==13) and (y==6) then
  n="ac13x6"
  r=10
  f=10
  init=FALSE
 end
set_actor("ac-top",x+0.5,y+0.5,{range=r,force=f,name=n,gohome=init})
end
actors["#"]=cell{parent={{ac_top}}}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!$%$%$%$%$%$%$%$%$%$",
       "!%&%&%&%&%&%&%&%&%&!",
       "!$%$%$%$%$%$%$%$%$%$",
       "!%&%&%&%&%&%&%&%&%&!",
       "!$%$%$%$%$%$%$%$%$%$",
       "!%&%&%&%&%&%&%&%&%&!",
       "!$%$%$%$%$%$%$%$%$%$",
       "!%&%&%&%&%&%&%&%&%&!",
       "!$%$%$%$%$%$%$%$%$%$",
       "!%&%&%&%&%&%&%&%&%&!",
       "!$%$%$%$%$%$%$%$%$%$",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "      !      #      ",
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
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!#$#!#$#!#!#!#!#!#!#",
        "#         #%#%#%#%#%",
        "$           !#!#!#!#",
        "#          %#%#%#%#%",
        "!         !#$   $#!#",
        "#                 #%",
        "$                  $",
        "#                 #%",
        "!         !#$   $#!#",
        "#          %#%#%#%#%",
        "$           !#!#!#!#",
        "#         #%#%#%#%#%",
        "!#$#!#$#!#!#!#!#!#!#"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
if difficult then
  AddRubberBand(enigma.GetNamedObject("ac13x6"),enigma.GetNamedObject("ac6x6"),15,0)
  AddRubberBand(enigma.GetNamedObject("ac6x6"),enigma.GetStone(0,6),8,0)
  AddRubberBand(enigma.GetNamedObject("ac13x6"),enigma.GetStone(19,6),15,0)
else
  AddRubberBand(enigma.GetNamedObject("ac13x6"),enigma.GetNamedObject("ac6x6"),8,0)
  AddRubberBand(enigma.GetNamedObject("ac6x6"),enigma.GetStone(0,6),8,0)
  AddRubberBand(enigma.GetNamedObject("ac13x6"),enigma.GetStone(19,6),8,0)
end 
oxyd_shuffle()































