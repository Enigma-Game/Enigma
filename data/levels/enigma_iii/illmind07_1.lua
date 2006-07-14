-- White Contrast, a level for Enigma
-- Copyright (C) JAHR illmind
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
cells["!"]=cell{floor="fl-acblack"}
stones["!"]=cell{stone="st-rock6"}
stones["#"]=cell{parent={{file_oxyd,"c"}}}
stones["$"]=cell{stone="st-grate1"}
stones["%"]=cell{stone="st-stone_break"}
stones["&"]=cell{stone="st-swap"}
stones["'"]=cell{stone="st-wood"}
items["!"]=cell{item="it-hammer"}
items["#"]=cell{item="it-document"}
function ac_whiteball(x,y)
n=""
p=0
f=0
 if (x==18) and (y==7) then
  n="ac18x7"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-whiteball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_whiteball}}}
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
          "                  ! ",
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
         "              ! ##  ",
         "                    ",
         "                 !  ",
         "                    ",
         "                    ",
         "                    "}
 stmap={"!!!!!!!!!!!!!!!!!!!!",
        "!#$!$$!$!$$$$%%%%#!!",
        "!!!!$$!#!$$&$%%%%!#!",
        "!$$$$$!!!$&!$%%%!#%!",
        "!$$$$$$$$$$&$%%!%%%!",
        "!$$$$$$$$$$$$%!%%%%!",
        "!%%%%%!!!!!!!!%%%%%!",
        "!%#%%!  $$$  '  '  !",
        "!%%%!$&$$$$$$$$$!!!!",
        "!%%!%$!&$$$&!$$$!$$!",
        "!#!%%$&$$$$$$$$$!!!!",
        "!!#%%$$$$$$$$$$$$$$!",
        "!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)
SetAttrib(enigma.GetItem(16,7),"text","one  more  and  you  won't  get  this  hammer  until  you  don't  need  it  anymore  !")
SetAttrib(enigma.GetItem(17,7),"text","illmind's  white  contrast  ...")
oxyd_shuffle()



























