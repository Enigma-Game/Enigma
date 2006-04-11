-- Pac Marble, a level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above
-- Generated with the help of BBE 1.05

-- Remark: This level demonstrates the ghosts-library.

Require("levels/lib/ant.lua")
Require("levels/lib/andreas_ghosts.lua")
function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end
levelh=13
levelw=20
enigma.FlatForce=30
--enigma.SlopeForce=30
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
stones["!"]=cell{stone="st-brick"}
stones["#"]=cell{parent={{file_oxyd,"d"}}}

cells["!"]=cell{parent={{ghosts_set_rail,1,"fl-gray"}}}
actors["!"]=cell{parent={{ghosts_set_ghost,"ac-top",1,
                          ghosts_direction_intelligent,{range=0,force=5,gohome=FALSE}}}}

function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==12) and (y==8) then
  n="ac12x8"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["#"]=cell{parent={{ac_blackball}}}
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
          "        !!!!        ",
          "                    ",
          "            #       ",
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
 stmap={"!!!!!!!!!##!!!!!!!!!",
        "!#  !          !  #!",
        "!   ! !!!!!!!! !   !",
        "!!! !          ! !!!",
        "!   !! !!  !! !!   !",
        "! !    !    !    ! !",
        "# !!!! !    ! !!!! #",
        "! !    !!!!!!    ! !",
        "!   !          !   !",
        "!!! ! !!!!!!!! ! !!!",
        "!   ! !      ! !   !",
        "!#  !   !  !   !  #!",
        "!!!!!!!!!##!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
oxyd_shuffle()

ghosts_init(0,0)

