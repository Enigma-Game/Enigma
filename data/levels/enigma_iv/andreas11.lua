-- Patterns of Impulse, a level for Enigma
-- Copyright (C) 2005 Andreas Lochmann
-- Licensed under GPL v2.0 or above 
-- Created with the help of BBE 1.05

Require("levels/lib/ant.lua")
function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end
levelh=37
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
cells["!"]=cell{floor="fl-wood"}
stones["!"]=cell{stone="st-stoneimpulse"}
stones["#"]=cell{parent={{file_oxyd,"c"}}}
stones["$"]=cell{stone="st-stoneimpulse_movable"}
stones["."]=cell{stone="st-grate1"}
stones["N"]=cell{stone="st-stone1"}
stones["M"]=cell{stone="st-wood"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==9) and (y==16) then
  n="ac9x5"
  p=0
  mf=1,66666666666667
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
          "                    ",
          "                    ",
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
         "                    ",
         "                    ",
         "                    "}
if difficult then
 stmap={"                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "NNN              NNN",
        "!!N              N!!",
        "!MN              NM!",
        "! NNNN        NNNN !",
        "!!!!!NNNNNNNNNN!!!!!",
        "!#. $          $ .#!",
        "!...$          $...!",
        "!$$$ $ $ $$ $ $ $$$!",
        "N   $   $..$   $   N",
        "N   $...$  $...$   N",
        "N    $ $    $ $    N",
        "N   $...$  $...$   N",
        "N   $   $..$   $   N",
        "!$$$ $ $ $$ $ $ $$$!",
        "!...$          $...!",
        "!#. $          $ .#!",
        "!!!!!NNNNNNNNNN!!!!!",
        "! NNNN        NNNN !",
        "!MN              NM!",
        "!!N              N!!",
        "NNN              NNN",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    "}
else
 stmap={"                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "NNN              NNN",
        "!!N              N!!",
        "!MN              NM!",
        "! NNNN        NNNN !",
        "!!!!!NNNNNNNNNN!!!!!",
        "!#. $          $ .#!",
        "!...$          $...!",
        "!$$$ $ $ $$ $ $ $$$!",
        "N   $   $..$   $   N",
        "N   $...$  $...$   N",
        "N....$ $    $ $....N",
        "N   $...$  $...$   N",
        "N   $   $..$   $   N",
        "!$$$ $ $ $$ $ $ $$$!",
        "!...$          $...!",
        "!#. $          $ .#!",
        "!!!!!NNNNNNNNNN!!!!!",
        "! NNNN        NNNN !",
        "!MN              NM!",
        "!!N              N!!",
        "NNN              NNN",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    ",
        "                    "}
end
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)

TIMERINT = 3.0
motion_a = 0
motion_b = 0
motion_c = 0
motion_d = 0

function trig_a()
  set_stone("st-stone1", 2, 13);
  set_stone("st-stone1", 1, 14);
  set_stone("st-stone1", 2, 14);
  motion_a = 1
end
function clock_a()
  if motion_a == 0 then
    set_stone("st-grate1", 2, 13);
    set_stone("st-grate1", 1, 14);
    set_stone("st-grate1", 2, 14);
  end
  motion_a = 0
end

function trig_b()
  set_stone("st-stone1", 17, 13);
  set_stone("st-stone1", 18, 14);
  set_stone("st-stone1", 17, 14);
  motion_b = 1
end
function clock_b()
  if motion_b == 0 then
    set_stone("st-grate1", 17, 13);
    set_stone("st-grate1", 18, 14);
    set_stone("st-grate1", 17, 14);
  end
  motion_b = 0
end

function trig_c()
  set_stone("st-stone1", 2, 23);
  set_stone("st-stone1", 1, 22);
  set_stone("st-stone1", 2, 22);
  motion_c = 1
end
function clock_c()
  if motion_c == 0 then
    set_stone("st-grate1", 2, 23);
    set_stone("st-grate1", 1, 22);
    set_stone("st-grate1", 2, 22);
  end
  motion_c = 0
end

function trig_d()
  set_stone("st-stone1", 17, 23);
  set_stone("st-stone1", 18, 22);
  set_stone("st-stone1", 17, 22);
  motion_d = 1
end
function clock_d()
  if motion_d == 0 then
    set_stone("st-grate1", 17, 23);
    set_stone("st-grate1", 18, 22);
    set_stone("st-grate1", 17, 22);
  end
  motion_d = 0
end


set_item("it-trigger", 1, 11, {action="callback", target="trig_a"})
set_item("it-trigger", 18, 11, {action="callback", target="trig_b"})
set_item("it-trigger", 1, 25, {action="callback", target="trig_c"})
set_item("it-trigger", 18, 25, {action="callback", target="trig_d"})
set_stone("st-timer", 2, 11, {action="callback", target="clock_a", interval=TIMERINT} )
set_stone("st-timer", 17, 11, {action="callback", target="clock_b", interval=TIMERINT} )
set_stone("st-timer", 2, 25, {action="callback", target="clock_c", interval=TIMERINT} )
set_stone("st-timer", 17, 25, {action="callback", target="clock_d", interval=TIMERINT} )

set_item("it-document", 10, 16, {text="As long as there is motion around the oxyd-stones, you won't reach them.   So: Don't lose control of the level! It's possible."})

oxyd_shuffle()






























