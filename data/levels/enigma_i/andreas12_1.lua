-- Lissajous' Revenge, a level for Enigma
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
stones["!"]=cell{stone="st-rock6"}
cells["#"]=cell{floor="fl-water"}
cells["$"]=cell{floor="fl-swamp"}
if difficult then
  cells["-"]=cell{floor="fl-water"}
else
  cells["-"]=cell{floor="fl-swamp"}
end
stones["#"]=cell{parent={{file_oxyd,"b"}}}
cells["%"]=cell{floor="fl-bluegreenx"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==10) and (y==6) then
  n="ac10x6"
  p=0
  mf=1
 end
set_actor("ac-blackball",x,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!##$$$$$$$$$$$$$$##!",
       "!#$$$$###$$###$$$$#!",
       "!$$$$$#$-$$-$#$$$$$!",
       "!$$##--$$$$$$--##$$!",
       "!$$#$$$$$$$$$$$$#$$!",
       "!$$---$$$%%$$$---$$!",
       "!$$#$$$$$$$$$$$$#$$!",
       "!$$##--$$$$$$--##$$!",
       "!$$$$$#$-$$-$#$$$$$!",
       "!#$$$$###$$###$$$$#!",
       "!##$$$$$$$$$$$$$$##!",
       "!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "          !         ",
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
        "!                  !",
        "!      #    #      !",
        "!                  !",
        "!   #          #   !",
        "!                  !",
        "!   #          #   !",
        "!                  !",
        "!      #    #      !",
        "!                  !",
        "!                  !",
        "!!!!!!!!!!!!!!!!!!!!"}

-- stolen from Nat Pryce ("When Gravity Fails")

TIMER_INTERVAL1 = 0.1
TIMER_INTERVAL2 = 0.1231
LOOP_DURATION = 2.0
TICK_ANGLE1_DELTA = 360.0 * TIMER_INTERVAL1/LOOP_DURATION
TICK_ANGLE2_DELTA = 360.0 * TIMER_INTERVAL2/LOOP_DURATION
GRAVITY = 20.0

angle1 = 90.0
angle2 = 90.0
gravity_x = 0
gravity_y = 0

function spin_gravity()
    angle1 = mod( angle1 + TICK_ANGLE1_DELTA, 360.0 )
    angle2 = mod( angle2 + TICK_ANGLE2_DELTA, 360.0 )
    set_gravity()
end

function set_gravity()
    gravity_x = cos(angle1) * GRAVITY
    gravity_y = sin(angle2) * GRAVITY
    enigma.AddConstantForce( gravity_x, gravity_y )
end

create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
set_stone( "st-timer", 0, 0,
           {target="spin_gravity", action="callback",
            name="timer", interval=0.1} )
enigma.ConserveLevel = FALSE
oxyd_shuffle()



























