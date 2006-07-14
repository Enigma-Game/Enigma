-- Floppy Floors, a level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above 
-- Created with the help of BBE 1.05

Require("levels/lib/ant.lua")

-- Global variables holding the floor-state

levelh=13
levelw=20
enigma.FlatForce=30
enigma.SlopeForce=15
enigma.ElectricForce=30
enigma.FrictionFactor=1.5

way = {}
way[1] = 0
way[2] = 0
way[3] = 0
way[4] = 0
way[5] = 0

nofloor = "fl-abyss"
myfloor = "fl-gray"

function drawstate()
  cells["'"]=cell{floor=nofloor}
  cells["("]=cell{floor=nofloor}
  cells[")"]=cell{floor=nofloor}
  cells["*"]=cell{floor=nofloor}
  cells["+"]=cell{floor=nofloor}
  if way[1] == 1 then  cells["'"]=cell{floor=myfloor}  end
  if way[2] == 1 then  cells["("]=cell{floor=myfloor}  end
  if way[3] == 1 then  cells[")"]=cell{floor=myfloor}  end
  if way[4] == 1 then  cells["*"]=cell{floor=myfloor}  end
  if way[5] == 1 then  cells["+"]=cell{floor=myfloor}  end
  draw_map(0,0,level,cells)
end

-- Initialise random connections

trigger = {}
local j, k
for j = 1,5 do  trigger[j] = 0  end
for j = 1,5 do
  repeat
    k = random(1,5)
    if trigger[k] == 0 then
      trigger[k] = j
    else
      k = 0
    end
  until k > 0
end

-- Switch functions

function switch1()  switch(trigger[1])  end
function switch2()  switch(trigger[2])  end
function switch3()  switch(trigger[3])  end
function switch4()  switch(trigger[4])  end
function switch5()  switch(trigger[5])  end

function switch(j)
  way[j] = 1 - way[j]
  set_floor("fl-leaves", 0, j)
  drawstate()
  oxyd_closeall()
  oxyd_shuffle()
end

-- BBE/ant.lua stuff

function file_oxyd(x,y,f)
  oxyd_default_flavor=f
  oxyd(x,y)
end
function ac_blackball(x,y)
  n=""
  p=0
  f=0
  if (x==1) and (y==2) then
    n="ac1x2"
    p=0
    mf=1,66666666666667
  end
  set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end

cells={}
items={}
actors={}
stones={}
actors[" "]=cell{}
stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}
cells["!"]=cell{floor=nofloor}
if difficult then
  cells["#"]=cell{floor="fl-gradient1"}
  cells["&"]=cell{floor="fl-gradient2"}
  cells["$"]=cell{floor="fl-gradient3"}
  cells["%"]=cell{floor="fl-gradient4"}
else
  cells["#"]=cell{floor=myfloor}
  cells["&"]=cell{floor=myfloor}
  cells["$"]=cell{floor=myfloor}
  cells["%"]=cell{floor=myfloor}
end
cells["'"]=cell{floor=nofloor}
cells["("]=cell{floor=nofloor}
cells[")"]=cell{floor=nofloor}
cells["*"]=cell{floor=nofloor}
cells["+"]=cell{floor=nofloor}
stones["!"]=cell{}
stones["#"]=cell{parent={{file_oxyd,"b"}}}
items["!"]=cell{item="it-floppy"}
actors["!"]=cell{parent={{ac_blackball}}}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!#%%'''''!****(((++!",
       "!#!&!!!!'!*!!*!!!!+!",
       "!#!&(((('((((*!!!!+!",
       "!#!&!!!!'!+!!'!!!!)!",
       "!#!&!!!!)!+!!'!!!!)!",
       "!#!&)))))!+!!'*****!",
       "!#!&!!!!!!+!!(!!!!'!",
       "!#!&!!!!!!+!!(!!!!'!",
       "!#!&***'''+++))))))!",
       "!#!&!!!!!!!!!(!!!!)!",
       "!$$&+++++((((('''')!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          " !                  ",
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
         "                !   ",
         "                    ",
         "                    ",
         "        !           ",
         "                    ",
         "   !                ",
         "                    ",
         "                    ",
         "                    ",
         "             !      ",
         "                    ",
         "                    "}
 stmap={"                    ",
        "                   #",
        "  !                 ",
        "                    ",
        "  !                 ",
        "                    ",
        "  !         #      #",
        "                    ",
        "  !                 ",
        "                    ",
        "  !                 ",
        "                   #",
        "                    "}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
oxyd_shuffle()

set_stone("st-floppy", 2, 2, {action="callback", target="switch1"})
set_stone("st-floppy", 2, 4, {action="callback", target="switch2"})
set_stone("st-floppy", 2, 6, {action="callback", target="switch3"})
set_stone("st-floppy", 2, 8, {action="callback", target="switch4"})
set_stone("st-floppy", 2, 10, {action="callback", target="switch5"})



