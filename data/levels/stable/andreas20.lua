-- Dynamice, a level for Enigma
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
cells["!"]=cell{floor="fl-light"}
stones["!"]=cell{stone="st-likeoxydc"}
stones["#"]=cell{parent={{file_oxyd,"c"}}}
items["!"]=cell{item="it-dynamite"}
stones["$"]=cell{parent={{parent_itemfreeze, ""}}}
items["#"]=cell{item="it-springboard"}
cells["#"]=cell{floor="fl-abyss"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==9) and (y==6) then
  n="ac9x6"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!#########!!!!!",
       "!!!!!!!!#####!!!!!!!",
       "!!!!!!!!!###!!!!!!!!",
       "!!!!!!!!!###!!!!!!!!",
       "!!!!!!!!!!#!!!!!!!!!",
       "!!!!!!!!!!#!!!!!!!!!",
       "!!!!!!!!!!#!!!!!!!!!",
       "!!!!!!!!!###!!!!!!!!",
       "!!!!!!!!!###!!!!!!!!",
       "!!!!!!!!#####!!!!!!!",
       "!!!!!!#########!!!!!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "         !          ",
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
         "     #              ",
         "                    ",
         " !   !              ",
         "                    ",
         "     #              ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!#!!!!!!!!!!!!#!!!",
        "!                  !",
        "!                  !",
        "#                  #",
        "!                  !",
        "!                  !",
        "#  $   $           #",
        "!                  !",
        "!                  !",
        "#                  #",
        "!                  !",
        "!                  !",
        "!!!#!!!!!!!!!!!!#!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(5,4),"force",10)
SetAttrib(enigma.GetItem(5,4),"range",10)
SetAttrib(enigma.GetItem(5,8),"force",10)
SetAttrib(enigma.GetItem(5,8),"range",10)
--AddRubberBand(enigma.GetNamedObject("ac9x6"),enigma.GetStone(7,6),10,10)
--AddRubberBand(enigma.GetNamedObject("ac9x6"),enigma.GetStone(3,6),10,10)
--Signal ("it(5 8)","it(5 4)")
--Signal ("it(5 4)","it(5 8)")
oxyd_shuffle()

itemfreeze_init(0,0,0.01)

enigma.ConserveLevel = FALSE



