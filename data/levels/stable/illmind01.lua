-- bumpahead
-- Copyright (C) JAHR AUTOR 
-- Licensed under GPL v2.0 or above ?

Require("levels/lib/ant.lua")function file_oxyd(x,y,f)
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
cells["!"]=cell{floor="fl-inverse"}
stones["!"]=cell{stone="st-stoneimpulse"}
stones["#"]=cell{stone="st-stoneimpulse-hollow"}
stones["$"]=cell{parent={{file_oxyd,"c"}}}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==1) and (y==11) then
  n="ac1x11"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
items["!"]=cell{item="it-crack1"}
items["#"]=cell{item="it-document"}
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
          "                    ",
          "                    ",
          " !                  ",
          "                    "}
  itmap={"                    ",
         "  !!                ",
         "  !!                ",
         "    !!!        !    ",
         "   ! !!   ! ! !!    ",
         "  !!     !!  !      ",
         "            !!      ",
         "   ! !!! !! !!      ",
         "     ! ! ! !        ",
         "      ! !  ! ! !    ",
         "        ! #! !      ",
         "                !   ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!#  #!#####!##!####!",
        "!#  #####$##$###$##!",
        "!###   #!###### ###!",
        "!## #  ### ! #  #!#!",
        "!#  #!###  ## #####!",
        "!########!##  #!#!#!",
        "!## #   #  #  ##!##!",
        "!$### # # # ## !###!",
        "!#!### # ## # # ##$!",
        "!##$#### #  # ####$!",
        "! #####$#### # # ##!",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(10,10),"text","illmind's  bump  ahead  ...")
oxyd_shuffle()


























