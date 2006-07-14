-- Laser Path, a level for Enigma
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
cells["!"]=cell{floor="fl-normal"}
stones["!"]=cell{stone="st-rock2"}
stones["#"]=cell{stone="st-laserswitch"}
stones["$"]=cell{parent={{file_oxyd,"a"}}}
stones["%"]=cell{stone="st-door-h"}
stones["&"]=cell{stone="st-laser-e"}
stones["'"]=cell{stone="st-glass1"}
stones["("]=cell{stone="st-mirror-p|t"}
stones[")"]=cell{stone="st-mirror-p|"}
stones["*"]=cell{stone="st-switch"}
stones["+"]=cell{stone="st-door-v"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==4) and (y==2) then
  n="ac4x2"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
-- stones[","]=cell{stone="st-oneway-w"}
stones["<"]=cell{stone="st-oneway-w"}
stones[">"]=cell{stone="st-oneway-e"}
stones["-"]=cell{stone="st-mirror-p|m"}
stones["."]=cell{stone="st-mirror-p|tm"}
if difficult then
  stones["M"]=cell{stone="st-rock2"}
  stones["L"]=cell{stone="st-rock2"}
  stones["K"]=cell{stone="st-rock2"}
else
  stones["M"]=cell{stone="st-mirror-p|m"}
  stones["L"]=cell{stone="st-mirror-p|tm"}
  stones["K"]=cell{stone="st-mirror-p|"}
end
if difficult then
else
end
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
          "    !               ",
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
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!#!",
        "!!&              ( #",
        "!$'            >   !",
        "!!!*'!!(''')'-''K'%!",
        "# (              ) !",
        "!    <         <   !",
        "!%'!'!-''!''!-!!'!'!",
        "! )              ( #",
        "!    >         >   !",
        "!!!!'!M!LL!-''.!''%!",
        "!$'+'            ) !",
        "!!!*+<         <   !",
        "!!!!!!!!!!!!!!!!('(!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetStone(0,4),"init",FALSE)
SetAttrib(enigma.GetStone(2,1),"init",FALSE)
SetAttrib(enigma.GetStone(18,0),"init",FALSE)
SetAttrib(enigma.GetStone(19,1),"init",FALSE)
SetAttrib(enigma.GetStone(19,7),"init",FALSE)
Signal ("st(3 3)","st(2 1)")
Signal ("st(19 1)","st(18 3)")
Signal ("st(3 11)","st(3 10)")
Signal ("st(0 4)","st(1 6)")
Signal ("st(19 7)","st(18 9)")
Signal ("st(18 0)","st(4 11)")
if difficult then
  enigma.ConserveLevel = FALSE
  set_item("it-document", 5, 2, {text="Try the easy mode first..."})
end
oxyd_shuffle()





























