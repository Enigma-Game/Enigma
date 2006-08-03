-- Laserpowered, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

Require("levels/lib/ant.lua")

levelh=13
levelw=20

enigma.FrictionFactor=2.5

oxydlist = {}
oxydnumber = 0
triggernumber = 0
colournumber = 0
even = 0

cells={}
items={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxydnumber = oxydnumber + 1
    even = 1 - even
    if even == 1 then
      colournumber = colournumber + 1 
    end
    set_stone("st-oxyd", x, y, {flavor=f, color=tostring(colournumber),
             name="myoxyd"..tostring(oxydnumber)})
end

function set_trigger(x,y)
    -- Assume the stones had been set before the items - then oxydnumber
    -- holds the total number of oxyds!
    triggernumber = triggernumber + 1
    local r
    repeat
      r = random(1,oxydnumber)
    until (oxydlist[r] == nil)
    oxydlist[r] = 1
    set_item("it-trigger", x, y, 
        {action="trigger", target="myoxyd"..tostring(triggernumber), invisible=TRUE})
end

stones["n"]=cell{stone="st-laser-n"}
stones["e"]=cell{stone="st-laser-e"}
stones["s"]=cell{stone="st-laser-s"}
stones["w"]=cell{stone="st-laser-w"}
stones["#"]=cell{stone="st-glass3"}
stones["L"]=cell{stone="st-lightpassenger"}
stones["o"]=cell{parent={{file_oxyd,"b"}}}

items["h"]=cell{item="it-abyss"}
items["t"]=cell{parent={{set_trigger}}}

cells[" "]=cell{floor="fl-abyss"}
--cells["-"]=cell{floor="fl-black"}
cells["-"]=cell{floor="fl-abyss_fake"}
cells["+"]=cell{floor="fl-white"}

level={"                    ",
       " ------             ",
       " ------      +      ",
       " ------             ",
       " -+++++     + +     ",
       " -+---+    +   +    ",
       " -+---+  +       +  ",
       " -+---+    +   +    ",
       " -+++++     + +     ",
       " ------             ",
       " ------      +      ",
       " ------             ",
       "                    "}

itmap={"             h      ",
       "                    ",
       "             t      ",
       "                    ",
       "            t t     ",
       "           t   t    ",
       "       h t       t h",
       "           t   t    ",
       "            t t     ",
       "                    ",
       "             t      ",
       "                    ",
       "             h      "}

if difficult then
stmap={"########s##ss#ss##s#",
       "#      e           w",
       "###### # oo     oo #",
       "#      # o#     #o #",
       "#      e           w",
       "#  # # e    #n#    w",
       "#      #   Lw#eL   #",
       "#  # # e    #s#    w",
       "#      e           w",
       "#      # o#     #o #",
       "###### # oo     oo #",
       "#      e           w",
       "########n##nn#nn##n#"}
else
stmap={"########s##ss#ss##s#",
       "#      e           w",
       "###### # oo     oo #",
       "#      # o#     #o #",
       "#      e     L     w",
       "#  # # e    #n#    w",
       "#      #   Lw#eL   #",
       "#  # # e    #s#    w",
       "#      e     L     w",
       "#      # o#     #o #",
       "###### # oo     oo #",
       "#      e           w",
       "########n##nn#nn##n#"}
end

create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)

--oxyd_shuffle()

set_actor("ac-whiteball",4.5,6.5,{player=0,controllers=1})

--LASERRING COUNTERCLOCKWISE:
set_stone("st-switch_white", 1, 1, {action = "callback", target = "ring1"})
function ring1()
  SendMessage(enigma.GetStone(8,0), "onoff")
  SendMessage(enigma.GetStone(19,1), "onoff")
  SendMessage(enigma.GetStone(7,11), "onoff")
  SendMessage(enigma.GetStone(18,12), "onoff")
end

--LASERRING CLOCKWISE:
set_stone("st-switch_white", 1, 11, {action = "callback", target = "ring2"})
function ring2()
  SendMessage(enigma.GetStone(7,1), "onoff")
  SendMessage(enigma.GetStone(18,0), "onoff")
  SendMessage(enigma.GetStone(8,12), "onoff")
  SendMessage(enigma.GetStone(19,11), "onoff")
end

--TRIGGER TO OPEN THE OXYD STONES:
--tkoords={{11,5},{12,4},{14,4},{15,5},{15,7},{14,8},{12,8},{11,7}}

--for i = 1, 8 do
--    set_item("it-trigger", tkoords[i][1], tkoords[i][2],{action = "callback", target = "oxon", invisible=TRUE, mynr = i})
--end

--activate the oxyds:
function oxon(trigger,sender)
    local t = enigma.GetAttrib(sender, "mynr")
    local oxyds={{12,5},{14,5},{14,7},{12,7}}   
    if t==1 or t==2 then
        koords=oxyds[1]
    elseif t==3 or t==4 then
         koords=oxyds[2]
    elseif t==5 or t==6 then
         koords=oxyds[3]
    elseif t==7 or t==8 then
         koords=oxyds[4]
    end
    local ob=enigma.GetStone(koords[1],koords[2])
    SendMessage (ob, 'trigger')
end

--TRIGGERFELD TO ACTIVATE THE LASER 
for i = 1, 5 do
    for j = 1, 5 do
        --to only get the a ring with triggers and not a area:
        if ((i==1) or (i==5)) or ((j==1) or (j==5)) then
        set_item("it-trigger", 2+i-1, 4+j-1,
           {action = "callback", target = "laserron", invisible=TRUE, myx =
	   2+i-1, myy = 4+j-1})
	   end
    end
end

function laserron(onoff,sender)
  local x,y = enigma.GetPos(sender)
  --local x = enigma.GetAttrib(sender, "myx")
  --local y = enigma.GetAttrib(sender, "myy")
  local lx, ly
  if (x == 2) or (x == 6) then
   if x==2 and y==6 then
     SendMessage(enigma.GetStone(12, 6), "onoff")
   elseif x==6 and y==6 then
     SendMessage(enigma.GetStone(14, 6), "onoff")   
   else
     lx = (x - 2) * 3 + 7
     SendMessage(enigma.GetStone(lx, y), "onoff")
   end
  end
  if (y == 4) or (y == 8) then
    if x==4 and y==4 then
     SendMessage(enigma.GetStone(13, 5), "onoff")
    elseif x==4 and y==8 then
     SendMessage(enigma.GetStone(13, 7), "onoff")
    else
     ly = (y - 4) * 3
     SendMessage(enigma.GetStone(x + 9, ly), "onoff")
    end
  end
end
