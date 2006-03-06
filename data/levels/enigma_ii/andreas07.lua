-- Turn Around, a level for Enigma
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
cells["!"]=cell{floor="fl-normal"}
stones["!"]=cell{stone="st-rock2"}
stones["#"]=cell{stone="st-puzzle2-nw"}
stones["$"]=cell{stone="st-puzzle2-ew"}
stones["%"]=cell{stone="st-puzzle2-nsw"}
stones["&"]=cell{stone="st-puzzle2-sw"}
stones["'"]=cell{stone="st-puzzle2-ns"}
stones["o"]=cell{parent={{file_oxyd,"a"}}}
stones[")"]=cell{stone="st-puzzle2-new"}
stones["*"]=cell{stone="st-puzzle2-hollow"}
stones["+"]=cell{stone="st-puzzle2-esw"}
stones[","]=cell{stone="st-puzzle2-ne"}
stones["-"]=cell{stone="st-puzzle2-nes"}
stones["."]=cell{stone="st-puzzle2-es"}
stones["X"]=cell{stone="st-puzzle2-nesw"}
items["#"]=cell{item="it-document"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==16) and (y==9) then
  n="ac16x9"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
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
          "                !   ",
          "                    ",
          "                    ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "                #   ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
if difficult then
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!                  !",
        "!   #')')')',      !",
        "!   $o$o$o$o$      !",
        "!   %'*'X'*'-      !",
        "!   $o$o$o$o$      !",
        "!   %'X'!'X'-      !",
        "!   $o$o$o$o$      !",
        "!   %'*'X'*'-      !",
        "!   $o$o$o$o$      !",
        "!   &'+'+'+'.      !",
        "!                  !",
        "!!!!!!!!!!!!!!!!!!!!"}
else
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!                  !",
        "!   #')')')',      !",
        "!   $o$o$o$o$      !",
        "!   %'*'*'*'-      !",
        "!   $o$o$o$o$      !",
        "!   %'*'*'*'-      !",
        "!   $o$o$o$o$      !",
        "!   %'*'*'*'-      !",
        "!   $o$o$o$o$      !",
        "!   &'+'+'+'.      !",
        "!                  !",
        "!!!!!!!!!!!!!!!!!!!!"}
end
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
if difficult then
  SetAttrib(enigma.GetItem(16,3),"text","Just a hint: Try the easy-mode first...")
else
  SetAttrib(enigma.GetItem(16,3),"text","Don't think too long... it's easy!")
end
oxyd_shuffle()






















