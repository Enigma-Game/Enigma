-- Keystone, a level for Enigma
-- Copyright (C) 2006 Raoul
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
matr={{1,0,0,1,0},{1,1,1,1,1},{1,0,1,0,0}}
matrix2places(matr,5,3,4,5)
which_piece(matr,5,3)
draw_direct(anzteile,locs,teile,"")

matr={{0,1,0,0,1},{1,1,1,1,1},{0,1,0,0,1}}
matrix2places(matr,5,3,12,5)
which_piece(matr,5,3)
draw_direct(anzteile,locs,teile,"")

matr={{1,1,1},{0,1,0},{1,1,1},{0,1,0},{0,1,0}}
matrix2places(matr,3,5,9,1)
which_piece(matr,3,5)
draw_direct(anzteile,locs,teile,"")

matr={{0,1,0},{0,1,1},{1,1,0},{0,1,0},{1,1,1}}
matrix2places(matr,3,5,9,7)
which_piece(matr,3,5)
draw_direct(anzteile,locs,teile,"")

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
--Overwrite the TEIL() in libpuzzle.lua:
waswo={"","","","","","","","","","",""}

function teil(anz,orte,teile,art)
 restteile={}
 zyklen=anz
 for i=1,zyklen do
  t=random(1,anz)
  aktteil=teile[t]

  set_stone("st-puzzle".."2".."-"..aktteil, orte[1][i],orte[2][i])
  waswo[i]=aktteil

  restteile={}
  schogse=0

  for k=1,anz do
   if teile[k]==aktteil and schogse==0 then
    restteile[k]="0"
    schogse=1
   else
    restteile[k]=teile[k]
   end
  end

  teile={}
 
  z2=1
  for j=1,anz do
   if restteile[j]~="0" then
    teile[z2]=restteile[j]
    z2=z2+1
   end 
  end

  anz=anz-1
 end
end

--Oranges Puzzleteil
matr={{1,1,0,1,1},{0,1,1,1,0},{1,1,0,1,1}}
matrix2places(matr,5,3,8,5)
which_piece(matr,5,3)
teil(anzteile,locs,teile,"2")
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
