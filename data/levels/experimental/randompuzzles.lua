-- Puzzle Labor, a Test-Level for Enigma
-- Copyright (C) 2005 Raoul
-- Licensed under GPL v2.0 or above
-- Beispiel fuer die Zufallsverteilung von Puzzlesteinen...
-- Experimental

-- GENERAL --
Require("levels/lib/libpuzzle.lua")

levelw = 39
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- Design --
boden="fl-sahara"
wand="st-glass"
boden2="fl-black"


-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)
fill_floor(boden2,7,3,8,6)

-- STONES --
draw_border(wand)
draw_stones(wand,{19,1},{0,1},11)



set_stone("st-switch", 18, 1, {action="callback",target="wrap1",name="sg"})
set_stone("st-switch", 18, 2, {action="callback",target="wrap2",name="sg"})
set_stone("st-switch", 18, 11, {action="callback",target="del",name="sd"})

set_stone("st-switch", 20, 1, {action="callback",target="write_vals",name="sw"})
set_stone("st-switch", 20, 11, {action="callback",target="leere",name="sl"})

-- ACTORS --
local ac1=set_actor("ac-blackball", 1.5,1.5, {player=0})
local ac2=set_actor("ac-whiteball", 21.5,2.5, {player=1})

-- ITEMS
set_item("it-yinyang",1,1)
set_item("it-yinyang",21,2)

---------------------
-- Zeichne Schalter und Eventhandler dazu:
nr=0
array={}
for i=1,6 do
   for j=1,8 do
      nr = nr+1
      set_stone("st-switch",21+2*j,-1+2*i, {action="callback", target="sw"..nr, name="swi"..nr})

      func = "function sw"..nr.."() k="..nr.."; if array[k]==0 then array[k]=1 else array[k]=0 end end"
      dostring(func)
      array[nr] = 0
   end
end
---------------------
-- Zeichne die Figur mit durchmischten Teilen
function wrap1()
 matrix2places(matrix,8,6,7,3)
 which_piece(matrix,8,6)
 teil(anzteile,locs,teile,2)
end

-- Zeichne die Figur geloest
function wrap2()
 matrix2places(matrix,8,6,7,3)
 which_piece(matrix,8,6)
 draw_direct(anzteile,locs,teile,2)
end

-- Setzte die Schalter zurueck
function leere()
 for o=1,48 do
  enigma.SendMessage(enigma.GetNamedObject("swi"..o),"off",nil)
 end
end

-- loesche nicht loesbare Uebereste
function del()
 for i=1,8 do
  for j=1,6 do
   enigma.KillStone(6+i,2+j)
  end
 end
end

-- Speichere die Schalter
function write_vals()
matrix={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}}
 zz=1
 zs=1

 for h=1,48 do
   matrix[zz][zs]=array[h]

  if zs==8 then
   zs=0
   zz=zz+1
  end
  zs=zs+1
 end
end

-- Initial Code execution:
write_vals()












