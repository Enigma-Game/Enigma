-- Fatal attraction I, a level for Enigma
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

cells["!"]=cell{floor="fl-normal"}

if difficult then
 stones["!"]=cell{stone="st-fart"}
else
 stones["!"]=cell{stone="st-likeoxydb"}
end

stones["#"]=cell{parent={{file_oxyd,"b"}}}

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
        "!                  !",
        "!                  !",
        "!                  !",
        "!       #  #       !",
        "!      #    #      !",
        "!     #      #     !",
        "!    #        #    !",
        "!   #          #   !",
        "!  #            #  !",
        "! #              # !",
        "!#                #!",
        "!!!!!!!!!!!!!!!!!!!!"}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_actor("ac-blackball",10.5,1.5,{player=0,name="bb"})
set_actor("ac-rotor",4.5,11.5,{range=20,force=20,name="rot1",gohome=TRUE})
set_actor("ac-rotor",15.5,11.5,{range=20,force=20,name="rot2",gohome=TRUE})

AddRubberBand(enigma.GetNamedObject("bb"),enigma.GetNamedObject("rot1"),10,100)
AddRubberBand(enigma.GetNamedObject("bb"),enigma.GetNamedObject("rot2"),10,100)

set_item("it-document",10,1,{text="Dont get too attracted!"})
oxyd_shuffle()

