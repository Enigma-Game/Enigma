Require("levels/lib/ant.lua")
-- !!BBE105!! Do not delete this line
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
cells["!"]=cell{floor="fl-normal"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==3) and (y==2) then
  n="ac3x2"
  p=0
  mf=1
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
stones["!"]=cell{stone="st-rotator-right"}
stones["#"]=cell{stone="st-mirror-p/tm"}
stones["$"]=cell{stone="st-laser-w"}
stones["%"]=cell{stone="st-switch"}
stones["&"]=cell{parent={{file_oxyd,"d"}}}
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
          "   !                ",
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
         "                    "}
 stmap={"                    ",
        "                  & ",
        "                  & ",
        "                    ",
        "     !#  $%         ",
        "      #             ",
        "                    ",
        "                    ",
        "         ! #  $%    ",
        "                    ",
        "                    ",
        "                    ",
        "                    "}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetStone(9,4),"init",FALSE)
SetAttrib(enigma.GetStone(14,8),"init",FALSE)
Signal ("st(15 8)","st(14 8)")
Signal ("st(10 4)","st(9 4)")
oxyd_shuffle()


















