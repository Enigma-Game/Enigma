-- evil, a level for Enigma
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

cells["!"]=cell{floor="fl-inverse"}
cells["#"]=cell{floor="fl-normal"}
cells["$"]=cell{floor="fl-acwhite"}

stones["#"]=cell{stone="st-likeoxydb"}
stones["$"]=cell{parent={{file_oxyd,"b"}}}
stones["!"]=cell{stone="st-marble"}

level={"!!!!!!!!!!!!!!!!!!!!",
       "!!#!#!#!#!#!#!#!#!#!",
       "!#!$!#!#!$!#!#!#!#!!",
       "!!#!#!#!#!#!#!#!#!#!",
       "!#!#!#!#!#!#!$!#!#!!",
       "!!$!#!#!#!#!#!#!#!#!",
       "!#!#!#!#!#!#!#!#!#!!",
       "!!#!#!#!#!#!#!#!$!#!",
       "!#!#!#!$!#!#!#!#!#!!",
       "!!#!#!#!#!#!#!#!#!#!",
       "!#!#!#!$!#!$!#!#!#!!",
       "!!#!#!#!#!#!#!#!#!#!",
       "!!!!!!!!!!!!!!!!!!!!"}

 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!                  !",
        "!  $     #    #    !",
        "!                  !",
        "!       $    $  #  !",
        "! #  $             !",
        "!                  !",
        "!            $  #  !",
        "!      #  $       $!",
        "!  $               !",
        "!     ##   #    $  !",
        "!            $     !",
        "!!!!!!!!!!!!!!!!!!!!"}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_actor("ac-blackball",18.5,1.5,{player=0,name="bb"})

oxyd_shuffle()


