-- Fatal attraction II, a level for Enigma
-- Copyright (C) 2006 Spaceman
-- Licensed under GPL v2.0 or above 
-- Levelcode cleaned by raoul 3.2006
-- !!BBE105!! Do not delete this line

Require("levels/lib/ant.lua")

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end

levelh=13
levelw=20

cells={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}

cells["!"]=cell{floor="fl-bluegray"}

stones["!"]=cell{stone="st-blue-sand"}
stones["#"]=cell{stone="st-likeoxydc"}
stones["$"]=cell{parent={{file_oxyd,"c"}}}

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

 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!     #   #     #  !",
        "!  $         $     !",
        "!                  !",
        "! #    $   #  $   #!",
        "!    #             !",
        "! $                !",
        "!   #        $     !",
        "!#   $ #        $  !",
        "!         $        !",
        "!  $          #    !",
        "!      #           !",
        "!!!!!!!!!!!!!!!!!!!!"}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_stone("st-black4",9,6)

set_actor("ac-blackball",9.5,6.5,{player=0,name="bb"})
set_actor("ac-rotor",1.5,1.5,{range=11,force=20,name="rot1",gohome=TRUE})
set_actor("ac-rotor",1.5,11.5,{range=11,force=20,name="rot2",gohome=TRUE})
set_actor("ac-rotor",18.5,1.5,{range=11,force=20,name="rot3",gohome=TRUE})
set_actor("ac-rotor",18.5,11.5,{range=11,force=20,name="rot4",gohome=TRUE})

AddRubberBand(enigma.GetNamedObject("bb"),enigma.GetNamedObject("rot1"),10,10)
AddRubberBand(enigma.GetNamedObject("bb"),enigma.GetNamedObject("rot2"),10,10)
AddRubberBand(enigma.GetNamedObject("bb"),enigma.GetNamedObject("rot3"),10,10)
AddRubberBand(enigma.GetNamedObject("bb"),enigma.GetNamedObject("rot4"),10,10)

set_item("it-document",6,10,{text="Don't get attracted!"})

oxyd_shuffle()

