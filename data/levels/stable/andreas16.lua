-- The Dark Outside, a level for Enigma
-- Copyright (C) 2006 Andreas Lochmann
-- Licensed under GPL v2.0 or above 
-- Created with the help of BBE 1.05

Require("levels/lib/ant.lua")
function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end
levelh=25
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
cells["!"]=cell{floor="fl-brick"}
stones["!"]=cell{stone="st-rock4"}
cells["#"]=cell{floor="fl-abyss"}
function ac_blackball(x,y)
n=""
p=0
f=0
 if (x==1) and (y==1) then
  n="ac1x1"
  p=0
  mf=1,66666666666667
 end
set_actor("ac-blackball",x+0.5,y+0.5,{player=p,name=n,mouseforce=mf})
end
actors["!"]=cell{parent={{ac_blackball}}}
--stones["#"]=cell{stone="st-invisible"}
stones["#"]=cell{stone="st-stonebrush"}
items["!"]=cell{item="it-brush"}
stones["$"]=cell{parent={{file_oxyd,"a"}}}
level={"!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
       "!!!!!!!!!!!!!!!!!!!!!!!!!!!############",
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
       "#######################################"}
   acmap={"                                       ",
          " !                                     ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
          "                                       ",
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
  itmap={"                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
         "                                       ",
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
 stmap={"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
        "!  #      #        #                  !",
        "!  #   #  ###   ####                  !",
        "!  #####    #   #                     !",
        "!        #  # # #  #                  !",
        "!######  #    # #  #                  !",
        "!     #  # #  # #  #                  !",
        "!   # # ## #  # #  #                  !",
        "!   # ###  #### #  #                  !",
        "!## #      #    #  #                  !",
        "!   #   #### ####  #                  !",
        "!   #      #       #                  !",
        "!!!!!!!!!!!!!!!!!!!!!!!!!!!           !",
        "!                                     !",
        "!                                     !",
        "!                                     !",
        "!                                     !",
        "!                                     !",
        "!                                     !",
        "!  $                             $    !",
        "!                                     !",
        "!                                     !",
        "!                                     !",
        "!                                     !",
        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"}
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)
draw_map(0,0,acmap,actors)

randomseed( enigma.GetTicks() )

--colour = {}
uhrlaeuft = 1.0
richtung = 1.0
posx = 1.0
posy = 10.0
zerstoert = 0.0
count = 0
malen = 0
hinweis = 0
TIMERINT = 0.8

function timercallback()  -- Every 0.8 seconds something happens...
  if (uhrlaeuft == 1.0) and (zerstoert == 0.0) then -- forget these
    -- remove clock from its old position
    kill_stone(posx,posy)
    -- change direction each 5 steps
    count = count + 1
    if count == 5 then
      if difficult then
        richtung = richtung + 1
        if richtung == 5 then richtung = 1 end
        count = 1
      else
        richtung = richtung - 1
        if richtung == 0 then richtung = 4 end
        count = 1
      end
    end
    -- save old position and compute new
    oldposx = posx
    oldposy = posy
    if richtung == 1.0 then  posy = posy - 1  end
    if richtung == 2.0 then  posx = posx + 1  end
    if richtung == 3.0 then  posy = posy + 1  end
    if richtung == 4.0 then  posx = posx - 1  end
    -- find the stone at the new position
    hier = enigma.GetStone(posx,posy)
    -- there is a stone? Well, then keep the old position
    if not(hier == nil) then
      posx = oldposx
      posy = oldposy
    end
    -- put the clock at the new (or old) position
    set_stone("st-fourswitch",posx,posy, {action="callback", target="hitclock"})
    -- this's me again
    mir = enigma.GetStone(posx,posy)
    -- change the direction that's shown on the clock
    richtung2=richtung
    richtung=1
    for j=1,richtung2+3 do
      SendMessage(mir, "trigger")
    end
    -- maybe there is some item under the new position?
    teilchen = enigma.GetItem(posx,posy)
    if not(teilchen == nil) then
      if teilchen == enigma.GetNamedObject("thisbrush") then
        malen = 1
        kill_item(posx,posy)
      end
    end
    -- paint your way to a brighter day...
    if malen == 1 then
      set_floor("fl-concrete", posx, posy)
    end
    -- in case the clock is still hungry and in the dark...
    if malen == 0 then
      if posy > 12 then
        if hinweis == 0 then
          set_item("it-document", 1, 1, {text="What a pity, and there's so much paint left..."})
          hinweis = 1
        end
      end
    end        
  end
end

function hitclock()
  richtung = richtung + 1.0
  if richtung == 5.0 then
    richtung = 1.0
  end
end

set_stone("st-timer", 0, 0, {action="callback", target="timercallback", interval=TIMERINT} )
set_stone("st-fourswitch",posx,posy, {action="callback", target="hitclock"})
set_item("it-brush", 2, 3, {name="thisbrush"})

oxyd_shuffle()



















