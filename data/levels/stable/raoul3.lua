-- Oxyd-Puzzle, a level for Enigma
-- Copyright (C) 2005 Raoul
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 32
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

--DESIGN--
boden="fl-metal"
wand="st-rock1"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
draw_border(wand)
fill_stones("st-grate1",1,1,30,11)

set_stone("st-switch", 3, 5, {action="callback",target="wrap1",name="sw1"})
set_stone("st-switch", 3, 7, {action="callback",target="wrap2",name="sw2"})

set_stone("st-switch", 8, 5, {action="callback",target="wrap3",name="sw3"})
set_stone("st-switch", 8, 7, {action="callback",target="wrap4",name="sw4"})

set_stone("st-switch", 13, 5, {action="callback",target="wrap5",name="sw5"})
set_stone("st-switch", 13, 7, {action="callback",target="wrap6",name="sw6"})

set_stone("st-switch", 18, 5, {action="callback",target="wrap7",name="sw7"})
set_stone("st-switch", 18, 7, {action="callback",target="wrap8",name="sw8"})

set_stone("st-switch", 23, 5, {action="callback",target="wrap9",name="sw9"})
set_stone("st-switch", 23, 7, {action="callback",target="wrap10",name="sw10"})

set_stone("st-switch", 28, 5, {action="callback",target="wrap11",name="sw11"})
set_stone("st-switch", 28, 7, {action="callback",target="wrap12",name="sw12"})

-- ACTORS --
set_actor("ac-blackball", 4.5,3.25, {player=0})

-- OXYD --
onemal=0
oxyd_default_flavor = "b"

function set_oxyd()
 if onemal==0 then
 onemal=1
  for o=0,5 do
   oxyd(3+o*5,0)
  end
  for o=0,4 do
   oxyd(3+o*5,12)
  end
  oxyd(28,10)
  oxyd_shuffle()
 end
end

--test
da={0,0,0,0,0,0,0,0,0,0,0,0}

function do_test()
 ist=0
 for l=1,12 do
  ist=ist+da[l]
 end
 if ist==12 then
  da={0,0,0,0,0,0,0,0,0,0,0,0}
  set_oxyd()
 end
end

--Wrap-Funktionen
function wrap1()
 teil(5,{{2,3,3,3,4},{3,3,2,1,1}},{"e","w","ns","nw","es"},2)
 da[1]=1
 --do_test()
end

function wrap2()
 teil(5,{{2,3,3,3,4},{9,9,10,11,9}},{"e","w","n","ns","esw"},2)
 da[2]=1
 do_test()
end

function wrap3()
 teil(5,{{7,7,8,8,9},{2,3,1,2,1}},{"n","w","es","es","nw"},2)
 da[3]=1
 do_test()
end

function wrap4()
 teil(5,{{7,8,9,9,9},{11,11,11,10,9}},{"e","s","ns","ew","nw"},2)
 da[4]=1
 do_test()
end

function wrap5()
 teil(5,{{12,13,13,13,14},{2,1,2,3,2}},{"n","e","w","s","nesw"},2)
 da[5]=1
 do_test()
end

function wrap6()
 teil(5,{{12,12,13,13,14},{9,10,10,11,10}},{"n","w","s","ne","esw"},2)
 da[6]=1
 do_test()
end

function wrap7()
 teil(6,{{17,18,18,18,19,19},{3,3,2,1,1,3}},{"e","w","w","es","ns","new"},2)
 da[7]=1
 do_test()
end

function wrap8()
 teil(6,{{17,17,18,19,19,19},{10,11,11,11,10,9}},{"s","s","ns","ew","ne","nw"},2)
 da[8]=1
 do_test()
end

function wrap9()
 teil(6,{{22,23,23,23,24,24},{2,1,2,3,1,3}},{"e","w","w","es","ne","nsw"},2)
 da[9]=1
 do_test()
end

function wrap10()
 teil(7,{{22,22,23,23,23,24,24},{9,11,9,10,11,9,11}},{"e","e","w","w","ns","esw","new"},2)
 da[10]=1
 do_test()
end

function wrap11()
 teil(7,{{27,27,27,28,29,29,29},{1,2,3,1,1,2,3}},{"n","n","ns","ns","ew","es","sw"},2)
 da[11]=1
 do_test()
end

function wrap12()
 teil(8,{{27,27,27,28,28,29,29,29},{9,10,11,9,11,9,10,11}},{"ns","ns","ew","ew","sw","nw","ne","es"},2)
 da[12]=1
 do_test()
end

--berechne und zeichne die Puzzles
function teil(anz,orte,teile,art)
 restteile={}
 zyklen=anz
 for i=1,zyklen do
  t=random(1,anz)
  aktteil=teile[t]

  set_stone("st-puzzle"..art.."-"..aktteil, orte[1][i],orte[2][i])

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

display.SetFollowMode(display.FOLLOW_SCROLLING)












