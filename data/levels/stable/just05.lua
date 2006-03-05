-- walking around, a level for Enigma
-- Copyright (C) JAHR JuSt
-- Licensed under GPL v2.0 or above
-- !!BBE105!! Do not delete this line

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
cells["!"]=cell{floor="fl-concrete"}
stones["!"]=cell{stone="st-rock7"}
cells["#"]=cell{floor="fl-normal"}
stones["#"]=cell{parent={{file_oxyd,"c"}}}
stones["$"]=cell{stone="st-glass"}
items["!"]=cell{item="it-blackbomb"}
items["#"]=cell{item="it-trigger"}
stones["%"]=cell{stone="st-stoneimpulse"}
stones["&"]=cell{stone="st-mirror-3<tm"}
stones["'"]=cell{stone="st-grate1"}
stones["("]=cell{stone="st-shogun-sml"}
stones[")"]=cell{stone="st-door-v"}
stones["*"]=cell{stone="st-mirror-3<"}
items["$"]=cell{item="it-shogun-l"}
stones["+"]=cell{stone="st-oneway-s"}
stones[","]=cell{stone="st-stoneimpulse-hollow"}
stones["-"]=cell{stone="st-laser-n"}
stones["."]=cell{stone="st-mirror-p-"}
stones["/"]=cell{stone="st-mirror-p/"}
stones["0"]=cell{stone="st-oneway-w"}
cells["$"]=cell{floor="fl-abyss"}
stones["1"]=cell{stone="st-wood"}
stones["2"]=cell{stone="st-oneway-n"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==10) and (y==6) then
  n="ac10x6"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
items["%"]=cell{item="it-seed"}
stones["3"]=cell{stone="st-turnstile-w"}
stones["4"]=cell{stone="st-oneway-e"}
stones["5"]=cell{stone="st-turnstile"}
stones["6"]=cell{stone="st-turnstile-s"}
stones["7"]=cell{stone="st-turnstile-e"}
stones["8"]=cell{stone="st-puzzle2-ns"}
stones["9"]=cell{stone="st-puzzle2-ne"}
stones[":"]=cell{stone="st-puzzle2-s"}
stones[";"]=cell{stone="st-puzzle2-n"}
stones["<"]=cell{stone="st-puzzle2-ew"}
stones["="]=cell{stone="st-turnstile-n"}
stones[">"]=cell{stone="st-puzzle2-sw"}
level={"!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!!!!!!!!!!!!!",
       "!!!!!!!!$!!!!!!!!!!!",
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
          "          !         ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    ",
          "                    "}
  itmap={"                    ",
         "  !!!!  !           ",
         " !!     !           ",
         " !!     !           ",
         " #!         !       ",
         "                    ",
         "                    ",
         "          %         ",
         "   $              ! ",
         "                    ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!#$   '.   !   8''#!",
        "!$$*   '   !   9'''!",
        "! & & '' !2! ! :8<<!",
        "!     '/ ! 4 !  ;  !",
        "!!!!!!-! !2!!!    >!",
        "!%%%%  0 0 4 4     !",
        "!%' %  !!!+! !!!!!!!",
        "!%( %      ! 57  = !",
        "!%%%%  !1!+! 6  357!",
        "!!!    !,!35  57 6 !",
        "!#)+!!!!,!    6   #!",
        "!!%,,,,,,!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(1,4),"invisible",FALSE)
SetAttrib(enigma.GetStone(6,5),"init",FALSE)
Signal ("it(1 4)","st(6 5)")
Signal ("it(3 8)","st(2 11)")
oxyd_shuffle()

display.SetFollowMode(display.FOLLOW_SCROLLING)

















