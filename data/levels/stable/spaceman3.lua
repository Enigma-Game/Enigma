-- Fatal Attraction III, a level for Enigma
-- Copyright (C) 2006 Space man
-- Licensed under GPL v2.0 or above 
-- !!BBE105!! Do not delete this line
-- modified by raoul

Require("levels/lib/ant.lua")

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end

levelh=13
levelw=39

enigma.FlatForce=30
enigma.SlopeForce=30
enigma.ElectricForce=30

cells={}
items={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}

cells["!"]=cell{floor="fl-normal"}
stones["!"]=cell{stone="st-bluegray"}
stones["#"]=cell{parent={{file_oxyd,"c"}}}

items["!"]=cell{item="it-floppy"}
items["#"]=cell{item="it-yinyang"}

stones["%"]=cell{stone="st-floppy"}
stones["h"]=cell{stone="st-door-h"}
stones["v"]=cell{stone="st-door-v"}

level={"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"}

  itmap={"                                       ",
         "          !                 !          ",
         "          #                 #          ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       "}
 stmap={"!!!!!!!!!!%!!!!!!!!!!!!!!!!!%!!!!!!!!!!",
        "!        v v       !       v v        !",
        "!  #     !h!     # ! #     !h!     #  !",
        "!                  !                  !",
        "!                  !                  !",
        "!                  !                  !",
        "!                  !                  !",
        "!                  !                  !",
        "!                  !                  !",
        "!                  !                  !",
        "!  #             # ! #             #  !",
        "!                  !                  !",
        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"}

create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)

--SIGNALS
Signal ("st(10 0)","st(9 1)")
Signal ("st(10 0)","st(10 2)")
Signal ("st(10 0)","st(11 1)")

Signal ("st(28 0)","st(27 1)")
Signal ("st(28 0)","st(28 2)")
Signal ("st(28 0)","st(29 1)")

-- ACTORS
set_actor("ac-blackball",10.5,1.5,{player=0,name="black"})
set_actor("ac-whiteball",28.5,1.5,{player=1,name="white"})

set_actor("ac-rotor",10.5,11.5,{range=15,force=10,gohome=FALSE,name="rot1"})
set_actor("ac-rotor",28.5,11.5,{range=15,force=10,gohome=FALSE,name="rot2"})

--RUBBERBANDS
AddRubberBand(enigma.GetNamedObject("black"),enigma.GetNamedObject("rot1"),10,100)
AddRubberBand(enigma.GetNamedObject("white"),enigma.GetNamedObject("rot2"),10,100)

oxyd_shuffle()
