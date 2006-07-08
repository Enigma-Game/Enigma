-- The Chamber, a level for enigma
-- Copyright (C) 2006 Lukas Schueller
-- Licensed under GPL v2.0 or above
-- Created with BlackBallEd

Require("levels/ant.lua")

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
stones={}

stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}

cells["!"]=cell{floor="fl-stwood"}
stones["!"]=cell{stone="st-blue-sand"}
stones["#"]=cell{parent={{file_oxyd,"c"}}}
stones["$"]=cell{stone="st-door-v"}
items["!"]=cell{item="it-trigger"}
stones["%"]=cell{stone="st-wood"}
stones["&"]=cell{stone="st-door-h"}

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

itmap={"                    ",
       "                    ",
       "                    ",
       "                    ",
       "                    ",
       "       ! !          ",
       "        ! !         ",
       "       ! !          ",
       "        ! !         ",
       "                    ",
       "                    ",
       "                    ",
       "                    "}

stmap={"!!!!!!!!!!!!!!!!!!!!",
       "#                  #",
       "!    !!!!!!!!!!!   !",
       "!    $ $ $ $ $ !   !",
       "!    !!!!!!!!!&!   !",
       "!    !  % %  ! !   !",
       "!    ! % %   ! !   !",
       "!    !  % %  !&!   !",
       "!    ! % %   $ !   !",
       "!    !!!!!!!!!!!   !",
       "!                  !",
       "#                  #",
       "!!!!!!!!!!!!!!!!!!!!"}

create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)

SetAttrib(enigma.GetItem(7,5),"invisible",FALSE)
SetAttrib(enigma.GetItem(7,7),"invisible",FALSE)
SetAttrib(enigma.GetItem(8,6),"invisible",FALSE)
SetAttrib(enigma.GetItem(8,8),"invisible",FALSE)
SetAttrib(enigma.GetItem(9,5),"invisible",FALSE)
SetAttrib(enigma.GetItem(9,7),"invisible",FALSE)
SetAttrib(enigma.GetItem(10,6),"invisible",FALSE)
SetAttrib(enigma.GetItem(10,8),"invisible",FALSE)

Signal ("it(7 5)","st(13 8)")
Signal ("it(9 5)","st(14 7)")
Signal ("it(8 6)","st(14 4)")
Signal ("it(10 6)","st(13 3)")
Signal ("it(7 7)","st(11 3)")
Signal ("it(9 7)","st(9 3)")
Signal ("it(8 8)","st(7 3)")
Signal ("it(10 8)","st(5 3)")

oxyd_shuffle()

set_actor("ac-blackball",6.5,5.5)


