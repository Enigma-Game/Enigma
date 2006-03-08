Require("levels/lib/ant.lua")
-- !!BBE105!! Do not delete this line
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
actors={}
stones={}
actors[" "]=cell{}
stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}
cells["!"]=cell{floor="fl-abyss"}
cells["#"]=cell{floor="fl-gravel"}
stones["!"]=cell{stone="st-woven"}
cells["$"]=cell{floor="fl-normal"}
items["!"]=cell{item="it-coin2"}
stones["#"]=cell{stone="st-coinslot"}
items["#"]=cell{item="it-puller-e"}
stones["$"]=cell{stone="st-door-h"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==4) and (y==6) then
  n="ac4x6"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
items["$"]=cell{item="it-pipe-es"}
items["%"]=cell{item="it-pipe-ne"}
items["&"]=cell{item="it-puller-s"}
items["'"]=cell{item="it-pipe-h"}
items["("]=cell{item="it-pipe-v"}
items[")"]=cell{item="it-puller-w"}
items["*"]=cell{item="it-flagblack"}
items["+"]=cell{item="it-puller-n"}
stones["%"]=cell{parent={{file_oxyd,"a"}}}
items[","]=cell{item="it-spade"}
items["-"]=cell{item="it-cherry"}
items["."]=cell{item="it-pipe-wn"}
items["/"]=cell{item="it-coffee"}
items["0"]=cell{item={"it-trigger", {invisible="1"}}}
stones["&"]=cell{stone="st-mail-s"}
stones["'"]=cell{stone="st-turnstile-n"}
stones["("]=cell{stone="st-turnstile"}
stones[")"]=cell{stone="st-turnstile-s"}
stones["*"]=cell{stone="st-turnstile-w"}
stones["+"]=cell{stone="st-turnstile-e"}
level={"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "#######################################",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"}
   acmap={"                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "    !                                  ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       "}
  itmap={"                                       ",
         "                                       ",
         "    # & ) + ' , ( $                    ",
         " !          '   . (                    ",
         "                                       ",
         "      !        !                       ",
         "  !     ! !  !                         ",
         "                  0                    ",
         "  !                                    ",
         "    $ '     '     )                    ",
         "    % ( *   ' - / )                    ",
         "                                       ",
         "                                       "}
 stmap={"                                       ",
        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
        "!  ! ! ! ! ! ! ! ! !        !!!! ' !!%!",
        "!  ! ! ! ! ! ! ! ! !        ! '  (+!! !",
        "!  #$#$#$#$#$#$#$#$!        '*(*() !! !",
        "!                  !        ( '   ' ! !",
        "!                         & )*(*(+(+  !",
        "!                  !        ! '!  ' ! !",
        "!  #$#$#$#$#$#$#$#$!        !*(   (+! !",
        "!  ! ! ! ! ! ! ! ! !        !  ( (+ ! !",
        "!  ! ! ! !%! ! ! ! !        !  ) ) !! !",
        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
        "                                       "}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetStone(3,4),"init",FALSE)
SetAttrib(enigma.GetStone(3,8),"init",FALSE)
SetAttrib(enigma.GetStone(5,4),"init",FALSE)
SetAttrib(enigma.GetStone(5,8),"init",FALSE)
SetAttrib(enigma.GetStone(7,4),"init",FALSE)
SetAttrib(enigma.GetStone(7,8),"init",FALSE)
SetAttrib(enigma.GetStone(9,4),"init",FALSE)
SetAttrib(enigma.GetStone(9,8),"init",FALSE)
SetAttrib(enigma.GetStone(11,4),"init",FALSE)
SetAttrib(enigma.GetStone(11,8),"init",FALSE)
SetAttrib(enigma.GetStone(13,4),"init",FALSE)
SetAttrib(enigma.GetStone(13,8),"init",FALSE)
SetAttrib(enigma.GetStone(15,4),"init",FALSE)
SetAttrib(enigma.GetStone(15,8),"init",FALSE)
SetAttrib(enigma.GetStone(17,4),"init",FALSE)
SetAttrib(enigma.GetStone(17,8),"init",FALSE)
SetAttrib(enigma.GetItem(18,7),"invisible",TRUE)
Signal ("st(3 4)","st(4 4)")
Signal ("st(5 4)","st(6 4)")
Signal ("st(7 4)","st(8 4)")
Signal ("st(9 4)","st(10 4)")
Signal ("st(11 4)","st(12 4)")
Signal ("st(13 4)","st(14 4)")
Signal ("st(15 4)","st(16 4)")
Signal ("st(17 4)","st(18 4)")
Signal ("st(3 8)","st(4 8)")
Signal ("st(5 8)","st(6 8)")
Signal ("st(7 8)","st(8 8)")
Signal ("st(9 8)","st(10 8)")
Signal ("st(11 8)","st(12 8)")
Signal ("st(13 8)","st(14 8)")
Signal ("st(15 8)","st(16 8)")
Signal ("st(17 8)","st(18 8)")
Signal ("it(18 7)","st(18 8)")
oxyd_shuffle()










