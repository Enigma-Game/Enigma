-- Flipper, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
-- Thaks to Petr Machata for his ant.lua script
------------------------------------------------
-- functions to control the slope force
function changeslopef()
  slopef=random(7,10)
  enigma.SlopeForce = slopef
  enigma.SlopeForce2= slopef
end
function resetsf()
  enigma.SlopeForce = 10
  enigma.SlopeForce2= 10
end
------------------------------------------------
-- Environment
Require("levels/lib/ant.lua")
cells={}
map_cell_meaning("O", cell{floor="fl-sahara", actor={"ac-blackball", {player=0, mouseforce=1}}})
map_cell_meaning("b", cell{actor={"ac-whiteball", {mouseforce=0}}})
cells[" "]=cell{floor={"fl-gradient", {type=21}}}
cells[">"]=cell{floor={"fl-gradient", {type=23}}}
cells["<"]=cell{floor={"fl-gradient", {type=24}}}
cells["_"]=cell{floor="fl-sahara"}
cells["2"]=cell{item="it-extralife"}
cells["#"]=cell{stone="st-rock4"}
cells["+"]=cell{stone="st-actorimpulse"}
map_cell_meaning(".", cell{floor="fl-abyss", item={"it-trigger", {action="callback",target="resetsf",invisible=1}}})
level = {
  "####################",
  "#   >   >   <   <  #",
  "#   0   0   0   0  #",
  "#+    >   <   <   +#",
  "#+    0   0   0   +#",
  "#b      >+ +<      #",
  "#2      0   0      #",
  "#2  ++    >    ++  #",
  "#>>>>>    0   <<<<<#",
  "######>>    <<######",
  "########....########",
  "#######___O__#######",
  "####################"
}
-------------------------------------------------
-- Functions to control the "Flippers"
ta = 0
tb = 0
function left_trigger()
  if(ta == 0) then
    set_stone("st-actorimpulse", 6,9)
    set_stone("st-actorimpulse", 7,9)
    ta = 1
  else
    kill_stone(6,9)
    kill_stone(7,9)
    ta = 0
  end
end
function right_trigger()
  if(tb == 0) then
    set_stone("st-actorimpulse", 12,9)
    set_stone("st-actorimpulse", 13,9)
    tb = 1
  else
    kill_stone(12,9)
    kill_stone(13,9)
    tb = 0
  end
end
oxyd_default_flavor = "d"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
set_stone("st-timer",0,0,{action="callback",target="changeslopef",interval=3})
set_item("it-trigger", 7,11, {action="callback", target="left_trigger"})
set_item("it-trigger", 12,11, {action="callback", target="right_trigger"})
--------------------------------------------------
-- Some constants
enigma.BumperForce = 1000
enigma.SlopeForce = 10
enigma.SlopeForce2 = 10












