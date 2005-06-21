-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- Esprit #050

dofile(enigma.FindDataFile("levels/ant.lua"))
cells={}

cells[" "]=cell{floor="fl-leaves"}
cells["#"]=cell{stone="st-invisible"}
cells["B"]=cell{stone="st-actorimpulse_invisible"}
cells["1"]=cell{{{document,"x=4*4-30/2"}}}
cells["2"]=cell{{{document,"x=-7-3^2*(-1)^3"}}}
cells["3"]=cell{{{document,"x=4*3+5-2*7"}}}
cells["4"]=cell{{{document,"2^x=16"}}}
cells["e"]=cell{floor="fl-bluegray",stone="st-glass"}
cells["h"]=cell{parent=cells[" "],item="it-hollow"}
cells["H"]=cell{parent=cells[" "],item={"it-hollow", {essential=1}}}
cells["T"]=cell{stone={"st-timer", {name="test",action="callback", target="checkout", interval=1}}}

level = {
--  01234567890123456789
   "#B################B# ",--00     
   "Bh3              2hB ",--01     
   "#33              22# ",--02     
   "#                  # ",--03     
   "#               e  # ",--04     
   "#           e   e  #H",--05     
   "#       e   e   e  #T",--06     
   "#   e   e   e   e  # ",--07     
   "#                  # ",--08     
   "#                  # ",--09     
   "#44              11# ",--10     
   "Bh4              1hB ",--11     
   "#B################B# " --12     
}   
set_default_parent(cells[" "])

create_world_by_map(level,cells)

set_actor("ac-whiteball-small", 4.5,8.5,  {player=0, name="s1"})
set_actor("ac-whiteball-small",8.5,8.5,  {player=0,name="s2", mouseforce=1})
set_actor("ac-whiteball-small", 12.5,8.5, {player=0,name="s3", mouseforce=1})
set_actor("ac-whiteball-small",16.5,8.5, {player=0,name="s4", mouseforce=1})

ok=0

function checkout()
local x1,y1 = enigma.GetPos(enigma.GetNamedObject("s1"))
local x2,y2 = enigma.GetPos(enigma.GetNamedObject("s2"))
local x3,y3 = enigma.GetPos(enigma.GetNamedObject("s3"))
local x4,y4 = enigma.GetPos(enigma.GetNamedObject("s4"))
if (x1==18) and (y1==11) then ok=1  else ok=0 end
if (x2==18) and (y2==1)  then ok=ok else ok=0 end
if (x3==1)  and (y3==1)  then ok=ok else ok=0 end
if (x4==1)  and (y4==11) then ok=ok else ok=0 end
if ok==1 then 
enigma.KillItem(20,5) -- if every ball is in its own hole remove Essential hole for finishing level
end
end
     
x=0
y=0                                                             

for x=1,19,2 do
  for y=1,13,2 do
set_floor("fl-leavese1",x,y)
set_floor("fl-leavese2",x,y-1)
set_floor("fl-leavese3",x-1,y)
set_floor("fl-leavese4",x-1,y-1)
end
end
                                 
                                 