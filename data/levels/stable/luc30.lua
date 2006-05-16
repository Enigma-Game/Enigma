-- The Carousel, a level for Enigma
-- Copyright (C) 2006 Lukas Schueller
-- Licensed under GPL v2.0 or above
-- !!BBE105!! Do not delete this line

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
cells["!"]=cell{floor="fl-sahara"}
stones["!"]=cell{stone="st-lasertimeswitch"}
stones["#"]=cell{stone="st-rotator-right"}
stones["$"]=cell{stone="st-swap"}
stones["%"]=cell{parent={{file_oxyd,"b"}}}
stones["&"]=cell{stone="st-rotator-left"}
stones["'"]=cell{stone="st-fart"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==8) and (y==8) then
  n="ac8x8"
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
          "        !           ",
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
        "#$%$%$%$%$'$%$%$%$%!",
        "!%&&&&&&&&&&&&&&&&$!",
        "!$%$'$%$%$%$%$%$%$'#",
        "!#!!            !!!!",
        "!!!!            !!!!",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetStone(0,0),"init",FALSE)
SetAttrib(enigma.GetStone(0,2),"init",FALSE)
SetAttrib(enigma.GetStone(0,3),"init",FALSE)
SetAttrib(enigma.GetStone(0,4),"init",FALSE)
SetAttrib(enigma.GetStone(0,5),"init",FALSE)
SetAttrib(enigma.GetStone(0,6),"init",FALSE)
SetAttrib(enigma.GetStone(0,7),"init",FALSE)
SetAttrib(enigma.GetStone(0,8),"init",FALSE)
SetAttrib(enigma.GetStone(0,9),"init",FALSE)
SetAttrib(enigma.GetStone(0,10),"init",FALSE)
SetAttrib(enigma.GetStone(0,11),"init",FALSE)
SetAttrib(enigma.GetStone(0,12),"init",FALSE)
SetAttrib(enigma.GetStone(1,0),"init",FALSE)
SetAttrib(enigma.GetStone(1,5),"init",FALSE)
SetAttrib(enigma.GetStone(1,12),"init",FALSE)
SetAttrib(enigma.GetStone(2,0),"init",FALSE)
SetAttrib(enigma.GetStone(2,4),"init",FALSE)
SetAttrib(enigma.GetStone(2,5),"init",FALSE)
SetAttrib(enigma.GetStone(2,12),"init",FALSE)
SetAttrib(enigma.GetStone(3,0),"init",FALSE)
SetAttrib(enigma.GetStone(3,4),"init",FALSE)
SetAttrib(enigma.GetStone(3,5),"init",FALSE)
SetAttrib(enigma.GetStone(3,12),"init",FALSE)
SetAttrib(enigma.GetStone(4,0),"init",FALSE)
SetAttrib(enigma.GetStone(4,12),"init",FALSE)
SetAttrib(enigma.GetStone(5,0),"init",FALSE)
SetAttrib(enigma.GetStone(5,12),"init",FALSE)
SetAttrib(enigma.GetStone(6,0),"init",FALSE)
SetAttrib(enigma.GetStone(6,12),"init",FALSE)
SetAttrib(enigma.GetStone(7,0),"init",FALSE)
SetAttrib(enigma.GetStone(7,12),"init",FALSE)
SetAttrib(enigma.GetStone(8,0),"init",FALSE)
SetAttrib(enigma.GetStone(8,12),"init",FALSE)
SetAttrib(enigma.GetStone(9,0),"init",FALSE)
SetAttrib(enigma.GetStone(9,12),"init",FALSE)
SetAttrib(enigma.GetStone(10,0),"init",FALSE)
SetAttrib(enigma.GetStone(10,12),"init",FALSE)
SetAttrib(enigma.GetStone(11,0),"init",FALSE)
SetAttrib(enigma.GetStone(11,12),"init",FALSE)
SetAttrib(enigma.GetStone(12,0),"init",FALSE)
SetAttrib(enigma.GetStone(12,12),"init",FALSE)
SetAttrib(enigma.GetStone(13,0),"init",FALSE)
SetAttrib(enigma.GetStone(13,12),"init",FALSE)
SetAttrib(enigma.GetStone(14,0),"init",FALSE)
SetAttrib(enigma.GetStone(14,12),"init",FALSE)
SetAttrib(enigma.GetStone(15,0),"init",FALSE)
SetAttrib(enigma.GetStone(15,12),"init",FALSE)
SetAttrib(enigma.GetStone(16,0),"init",FALSE)
SetAttrib(enigma.GetStone(16,4),"init",FALSE)
SetAttrib(enigma.GetStone(16,5),"init",FALSE)
SetAttrib(enigma.GetStone(16,12),"init",FALSE)
SetAttrib(enigma.GetStone(17,0),"init",FALSE)
SetAttrib(enigma.GetStone(17,4),"init",FALSE)
SetAttrib(enigma.GetStone(17,5),"init",FALSE)
SetAttrib(enigma.GetStone(17,12),"init",FALSE)
SetAttrib(enigma.GetStone(18,4),"init",FALSE)
SetAttrib(enigma.GetStone(18,5),"init",FALSE)
SetAttrib(enigma.GetStone(18,12),"init",FALSE)
SetAttrib(enigma.GetStone(19,0),"init",FALSE)
SetAttrib(enigma.GetStone(19,1),"init",FALSE)
SetAttrib(enigma.GetStone(19,2),"init",FALSE)
SetAttrib(enigma.GetStone(19,4),"init",FALSE)
SetAttrib(enigma.GetStone(19,5),"init",FALSE)
SetAttrib(enigma.GetStone(19,6),"init",FALSE)
SetAttrib(enigma.GetStone(19,7),"init",FALSE)
SetAttrib(enigma.GetStone(19,8),"init",FALSE)
SetAttrib(enigma.GetStone(19,9),"init",FALSE)
SetAttrib(enigma.GetStone(19,10),"init",FALSE)
SetAttrib(enigma.GetStone(19,11),"init",FALSE)
SetAttrib(enigma.GetStone(19,12),"init",FALSE)
oxyd_shuffle()
