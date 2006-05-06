-- Acoustic Memory, a level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above
-- Generated with the help of BBE 1.05

-- Initialisation

Require("levels/lib/ant.lua")
levelh=15
levelw=20
enigma.FlatForce=30
enigma.SlopeForce=30
enigma.ElectricForce=30
oxyd_default_flavor = "b"

triggerstone = {}
for j = 0,17 do
  triggerstone[j] = -1
end

-- Set a trigger (parent)

counter = 0
function settrigger(x,y)
  local c
  local found = -1
  for c = 1,1000 do  -- try this at most 1000 times 
    if found == -1 then
      found = random(0,17)
      if triggerstone[found] == -1 then
        triggerstone[found] = counter
        counter = counter + 1
      else
        found = -1
      end
    end
  end
  if found == -1 then
    error("Unexpected probability: Assume to be extraordinarily happy to have found this!")
  else
    set_item("it-trigger", x, y, {action="trigger", target="mystone"..tostring(found),
        invisible=FALSE})
  end
end

-- Set stone with name "mystone"..counter2

counter2 = 0
function setmystone(x,y,st)
  if st == "st-oxyd" then
    oxyd(x,y)
  else
    set_stone(st, x, y)
  end
  SetAttrib(enigma.GetStone(x,y), "name", "mystone"..tostring(counter2))
  counter2 = counter2 + 1
end

-- The level itself

function ac_blackball(x,y)
  n=""
  p=0
  f=0
  if (x==9) and (y==5) then
    n="ac9x5"
    p=0
    mf=1
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
cells["!"]=cell{floor="fl-samba"}
stones["!"]=cell{stone="st-wood"}
stones["#"]=cell{parent={{setmystone,"st-oxyd"}}}
stones["$"]=cell{parent={{setmystone,"st-fart"}}}
items["!"]=cell{parent={{settrigger}}}
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
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
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
          "                    ",
          "                    ",
          "                    ",
          "                    "}
  itmap={"                    ",
         "                    ",
         "                    ",
         "  !  !  !  !  !  !  ",
         "                    ",
         "                    ",
         "  !  !  !  !  !  !  ",
         "                    ",
         "                    ",
         "  !  !  !  !  !  !  ",
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
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!                  !",
        "!!!!!!!!!!!!!!!!!!!!",
        "################$$!!",
        "                    "}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
oxyd_shuffle()


