-- Spaceless, a level for Enigma
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
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==9) and (y==1) then
  n="ac9x1"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
function random_gradient(x,y)
  nr = random(11)
  set_floor("fl-gradient"..nr, x, y)
end
cells[" "]=cell{floor="fl-space"}
stones["!"]=cell{stone="st-death"}
stones["#"]=cell{parent={{file_oxyd,"b"}}}
actors["!"]=cell{parent={{ac_blackball}}}
if difficult then
  cells["v"]=cell{floor="fl-gradient2"}
  cells["^"]=cell{floor="fl-gradient1"}
  cells[">"]=cell{floor="fl-gradient4"}
  cells["<"]=cell{floor="fl-gradient3"}
  cells["B"]=cell{floor="fl-gradient12"}
  cells["A"]=cell{floor="fl-gradient10"}
  cells["C"]=cell{floor="fl-gradient11"}
  cells["D"]=cell{floor="fl-gradient9"}
  cells["b"]=cell{floor="fl-gradient8"}
  cells["c"]=cell{floor="fl-gradient6"}
  cells["a"]=cell{floor="fl-gradient7"}
  cells["d"]=cell{floor="fl-gradient5"}
else
  cells["v"]=cell{floor="fl-gradient1"}
  cells["^"]=cell{floor="fl-gradient2"}
  cells[">"]=cell{floor="fl-gradient3"}
  cells["<"]=cell{floor="fl-gradient4"}
  cells["B"]=cell{floor="fl-gradient5"}
  cells["A"]=cell{floor="fl-gradient6"}
  cells["C"]=cell{floor="fl-gradient7"}
  cells["D"]=cell{floor="fl-gradient8"}
  cells["b"]=cell{floor="fl-gradient9"}
  cells["c"]=cell{floor="fl-gradient10"}
  cells["a"]=cell{floor="fl-gradient11"}
  cells["d"]=cell{floor="fl-gradient12"}
end
cells["R"]=cell{parent={{random_gradient}}}

-- Gradienten
--   1   v
--   2   ^
--   3   >
--   4   <
--   5  > v  Tal
--   6  < v  Tal
--   7  > ^  Tal
--   8  < ^  Tal
--   9  > v  Spitze
--  10  > ^  Spitze
--  11  < v  Spitze
--  12  < ^  Spitze

   level={"                    ",
          " BvvvvvvvvvvvvvvvvA ",
          " >                < ",
          " > R R R dc R R R < ",
          " >       ab       < ",
          " > R R R    R R R < ",
          " >                < ",
          " > R R R    R R R < ",
          " >       dc       < ",
          " > R R R ab R R R < ",
          " >                < ",
          " C^^^^^^^^^^^^^^^^D ",
          "                    "}
   acmap={"                    ",
          "                    ",
          "                    ",
          "         !          ",
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
   stmap={"!!!!!!!!!!!!!!!!!!!!",
          "!                  !",
          "! #              # !",
          "!                  !",
          "!                  !",
          "!                  !",
          "!        ##        !",
          "!                  !",
          "!                  !",
          "!                  !",
          "! #              # !",
          "!                  !",
          "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
oxyd_shuffle()































