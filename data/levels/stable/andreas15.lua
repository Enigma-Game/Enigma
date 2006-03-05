-- Spaceleast, a level for Enigma
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
stones["!"]=cell{stone="st-rock6"}
stones["#"]=cell{stone="st-death"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==2) and (y==2) then
  n="ac2x2"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
stones["$"]=cell{parent={{file_oxyd,"b"}}}
stones["%"]=cell{stone="st-easymode"}
items["!"]=cell{item="it-magnet-on"}
cells["!"]=cell{floor="fl-space"}
cells["&"]=cell{floor="fl-gradient1"}
cells["'"]=cell{floor="fl-gradient2"}
cells["("]=cell{floor="fl-gradient11"}
cells[")"]=cell{floor="fl-gradient12"}
cells["#"]=cell{floor="fl-normal"}
cells["$"]=cell{floor="fl-gradient9"}
cells["%"]=cell{floor="fl-gradient10"}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!$!!!!!!!&!!!!!!!!(!",
       "!!$!!!!!!!&!!!!!!(!!",
       "!!!$!!!!!&!!!!!!(!!!",
       "!!!!$!!!!!&!!!!(!!!!",
       "!!!!!$!!!&!!!!(!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!%!!!!'!!!)!!!!!",
       "!!!!%!!!!'!!!!!)!!!!",
       "!!!%!!!!!!'!!!!!)!!!",
       "!!%!!!!!!'!!!!!!!)!!",
       "!%!!!!!!!!'!!!!!!!)!",
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
         "                    ",
         "                    ",
         "                    ",
         "        !  !        ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!#!!#!!#!##!#!!#!!#!",
        "#                  #",
        "!                  !",
        "#                  #",
        "!                  !",
        "#                  #",
        "!     $$%$$%$$     !",
        "#                  #",
        "!                  !",
        "#                  #",
        "!                  !",
        "#                  #",
        "!#!!#!!#!##!#!!#!!#!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(8,6),"force",10)
SetAttrib(enigma.GetItem(11,6),"force",10)
oxyd_shuffle()


















