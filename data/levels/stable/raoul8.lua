-- Keystone, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 
-- Another Level with Puzzles...

-- GENERAL --
Require("levels/lib/libpuzzle.lua")
levelw = 20
levelh = 37
create_world(levelw, levelh)
enigma.ConserveLevel = TRUE

-- DESIGN --
boden="fl-black"
wand="st-glass"

-- FLOOR --
fill_floor("fl-abyss_fake", 0,0,levelw,levelh)
fill_floor(boden, 3,0,15,13)
fill_floor(boden, 5,26,11,9)

-- STONES --
draw_border(wand,3,0,15,13)
draw_border(wand,5,26,11,9)

-- second room --
if not difficult then
set_stone("st-switch_white", 8, 29, {action="callback",target="per1",name="rot1"})
set_stone("st-switch_white", 8, 31, {action="callback",target="per2",name="rot2"})
set_stone("st-switch_white", 9, 30, {action="callback",target="per3",name="rot3"})
set_stone("st-switch_white", 10, 30, {action="callback",target="per4",name="rot4"})
set_stone("st-switch_white", 11, 30, {action="callback",target="per5",name="rot5"})
set_stone("st-switch_white", 12, 29,{action="callback",target="per6",name="rot6"})
set_stone("st-switch_white", 12, 31, {action="callback",target="per7",name="rot7"})
else
set_stone("st-switch_white", 8, 29, {action="callback",target="per7",name="rot1"})
set_stone("st-switch_white", 8, 31, {action="callback",target="per6",name="rot2"})
set_stone("st-switch_white", 9, 30, {action="callback",target="per5",name="rot3"})
set_stone("st-switch_white", 10, 30, {action="callback",target="per4",name="rot4"})
set_stone("st-switch_white", 11, 30, {action="callback",target="per3",name="rot5"})
set_stone("st-switch_white", 12, 29,{action="callback",target="per2",name="rot6"})
set_stone("st-switch_white", 12, 31, {action="callback",target="per1",name="rot7"})
end

-- PUZZLES --
--blaue:
puzzle({{1,0,0,1,0},{1,1,1,1,1},{1,0,1,0,0}},4,5,"blue","no")

puzzle({{0,1,0,0,1},{1,1,1,1,1},{0,1,0,0,1}},12,5,"blue","no")

puzzle({{1,1,1},{0,1,0},{1,1,1},{0,1,0},{0,1,0}},9,1,"blue","no")

puzzle({{0,1,0},{0,1,1},{1,1,0},{0,1,0},{1,1,1}},9,7,"blue","no")

-- ACTORS --
set_actor("ac-blackball", 15.5,10.5, {player=0})
set_actor("ac-whiteball", 7.5,28.5, {player=1})
set_item("it-yinyang", 15,10)
set_item("it-yinyang", 7,28)

-- OXYD --
oxyd(3,6)
oxyd(10,0)
oxyd(10,12)
oxyd(17,6)
oxyd_default_flavor = "d"
oxyd_shuffle()

-- LUA-FUNCTIONS --
--Overwrite the PUZZLE_SHUFFLE() in libpuzzle.lua:
waswo={"","","","","","","","","","",""}

function puzzle_shuffle(teile)
 
 shuffled_pieces={}

 local restteile={}
 local anz=anz_stones
 local zyklen=anz
 local i,j,k
 local counter=1

 for i=1,zyklen do

  --shuffle pieces:
  t=random(1,anz)
  
  aktteil=teile[t]
  shuffled_pieces[counter]=aktteil
  counter=counter+1
  
 --add this line for this level:
  waswo[i]=aktteil

  --prepare the teile array for next cycle
  local restteile={}
  local schogse=0

  --copy the teile array, mark the piece we just have used with "0"
  for k=1,anz do
   if teile[k]==aktteil and schogse==0 then
    restteile[k]="0"
    schogse=1
   else
    restteile[k]=teile[k]
   end
  end

  --clear teile array:
  teile={}
  local t=1

  for j=1,anz do
   if restteile[j]~="0" then
    teile[t]=restteile[j]
    t=t+1
   end 
  end
  
  --we have used one piece:
  anz=anz-1
 end

 return shuffled_pieces

end

--Oranges Puzzleteil
shuffle_method="random"
puzzle({{1,1,0,1,1},{0,1,1,1,0},{1,1,0,1,1}},8,5,"red","yes")

---------------
function per1()
 localram={"",""}
 localram[1]=waswo[1]
 localram[2]=waswo[2]
 tmp=localram[2]
 localram[2]=localram[1]
 localram[1]=tmp
 waswo[1]=localram[1]
 waswo[2]=localram[2]
 set_stone("st-puzzle".."2".."-"..localram[1],8,5)
 set_stone("st-puzzle".."2".."-"..localram[2],9,5)
end
---------------
function per2()
 localram={"",""}
 localram[1]=waswo[8]
 localram[2]=waswo[9]
 tmp=localram[2]
 localram[2]=localram[1]
 localram[1]=tmp 
 waswo[8]=localram[1]
 waswo[9]=localram[2]
 set_stone("st-puzzle".."2".."-"..localram[1],8,7)
 set_stone("st-puzzle".."2".."-"..localram[2],9,7)
end
---------------
function per3()
 localram={"","",""}
 localram[1]=waswo[2]
 localram[2]=waswo[5]
 localram[3]=waswo[9]
 tmp=localram[3]
 localram[3]=localram[2]
 localram[2]=localram[1]
 localram[1]=tmp
 waswo[2]=localram[1]
 waswo[5]=localram[2]
 waswo[9]=localram[3]
 set_stone("st-puzzle".."2".."-"..localram[1],9,5)
 set_stone("st-puzzle".."2".."-"..localram[2],9,6)
 set_stone("st-puzzle".."2".."-"..localram[3],9,7)
end
---------------
function per4()
 localram={"","",""}
 localram[1]=waswo[5]
 localram[2]=waswo[6]
 localram[3]=waswo[7]
 tmp=localram[3]
 localram[3]=localram[2]
 localram[2]=localram[1]
 localram[1]=tmp
 waswo[5]=localram[1]
 waswo[6]=localram[2]
 waswo[7]=localram[3]
 set_stone("st-puzzle".."2".."-"..localram[1],9,6)
 set_stone("st-puzzle".."2".."-"..localram[2],10,6)
 set_stone("st-puzzle".."2".."-"..localram[3],11,6)
end
---------------
function per5()
 localram={"","",""}
 localram[1]=waswo[3]
 localram[2]=waswo[7]
 localram[3]=waswo[10]
 tmp=localram[3]
 localram[3]=localram[2]
 localram[2]=localram[1]
 localram[1]=tmp
 waswo[3]=localram[1]
 waswo[7]=localram[2]
 waswo[10]=localram[3]
 set_stone("st-puzzle".."2".."-"..localram[1],11,5)
 set_stone("st-puzzle".."2".."-"..localram[2],11,6)
 set_stone("st-puzzle".."2".."-"..localram[3],11,7)
end
---------------
function per6()
 localram={"",""}
 localram[1]=waswo[3]
 localram[2]=waswo[4]
 tmp=localram[2]
 localram[2]=localram[1]
 localram[1]=tmp
 waswo[3]=localram[1]
 waswo[4]=localram[2]
 set_stone("st-puzzle".."2".."-"..localram[1],11,5)
 set_stone("st-puzzle".."2".."-"..localram[2],12,5)
end
---------------
function per7()
 localram={"",""}
 localram[1]=waswo[10]
 localram[2]=waswo[11]
 tmp=localram[2]
 localram[2]=localram[1]
 localram[1]=tmp
 waswo[10]=localram[1]
 waswo[11]=localram[2]
 set_stone("st-puzzle".."2".."-"..localram[1],11,7)
 set_stone("st-puzzle".."2".."-"..localram[2],12,7)
end
