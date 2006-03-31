-- Venice II, a level for Enigma
-- Copyright (C) 2006 Raoul
-- Licensed under GPL v2.0 or above 

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

cells[" "]=cell{floor="fl-water"}
cells["#"]=cell{floor="fl-concrete"}

stones["O"]=cell{parent={{file_oxyd,"b"}}}
stones["G"]=cell{stone="st-glass"}

-- Random Bridge Setup:
c=random(2)
r=random(2)

if c == 1 then
 if r == 1 then
  stones["a"]=cell{stone="st-oneway_white-n"}
  stones["d"]=cell{stone="st-oneway_white-w"}

  stones["f"]=cell{stone="st-oneway_black-e"}
  stones["g"]=cell{stone="st-oneway_black-s"}
 else
  stones["a"]=cell{stone="st-oneway_white-s"}
  stones["d"]=cell{stone="st-oneway_white-e"}

  stones["f"]=cell{stone="st-oneway_black-w"}
  stones["g"]=cell{stone="st-oneway_black-n"}
 end
else
 if r == 1 then
  stones["a"]=cell{stone="st-oneway_black-n"}
  stones["d"]=cell{stone="st-oneway_black-w"}
  stones["f"]=cell{stone="st-oneway_white-e"}
  stones["g"]=cell{stone="st-oneway_white-s"}
 else
  stones["a"]=cell{stone="st-oneway_black-s"}
  stones["d"]=cell{stone="st-oneway_black-e"}
  stones["f"]=cell{stone="st-oneway_white-w"}
  stones["g"]=cell{stone="st-oneway_white-n"}
 end
end

level={"                    ",
       "  ### ###  ### ###  ",
       "  ################  ",
       "  ### ###  ### ###  ",
       "   #   #    #   #   ",
       "  ### ###  ### ###  ",
       "  ################  ",
       "  ### ###  ### ###  ",
       "   #   #    #   #   ",
       "  ### ###  ### ###  ",
       "  ################  ",
       "  ### ###  ### ###  ",
       "                    "}

stmap={"                    ",
       "  GOG GOG  GOG GGG  ",
       "  O ddd ffff ddd G  ",
       "  GgG GaG  GgG GaG  ",
       "   g   a    g   a   ",
       "  GgG GaG  GgG GaG  ",
       "  O fff dddd fff O  ",
       "  GaG GgG  GaG GgG  ",
       "   a   g    a   g   ",
       "  GaG GgG  GaG GgG  ",
       "  G ddd ffff ddd O  ",
       "  GGG GOG  GOG GOG  ",
       "                    "}

create_world_by_map(level)
draw_map(0,0,stmap,stones)

set_item("it-yinyang",2,10)
set_item("it-yinyang",17,2)

set_item("it-ring",3,11)
set_item("it-ring",16,1)

set_item("it-cherry",3,10)
set_item("it-cherry",16,2)

set_actor("ac-whiteball",3.5,10.5,{player=1,name="wb"})
set_actor("ac-blackball",16.5,2.5,{player=0,name="bb"})

oxyd_shuffle()
