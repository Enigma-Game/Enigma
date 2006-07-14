-- Walk the Dog, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v. 2.0 or above
-- Contact: Lucky_Luc@web.de
-- Thanks to Petr Machata for his ant.lua script
------------------------------------------------
--environment
wand="st-marble"
wandinnen="st-marble"


Require("levels/lib/ant.lua")
cells={}
cells[" "]=cell{floor="fl-marble"}
cells["+"]=cell{floor="fl-rough"}
cells["a"]=cell{floor="fl-abyss"}
cells["*"]=cell{parent=cells[" "], stone=wandinnen}
cells["_"]=cell{parent=cells["a"], stone="st-glass"}
cells["#"]=cell{parent=cells[" "], stone=wand}
cells["f"]=cell{parent=cells["a"], item={"it-trigger", {invisible=1, action="callback", target="new1"}}}
cells["B"]=cell{parent=cells[" "], stone={"st-fart", {name="fart"}}}
cells["O"]=cell{parent=cells["+"], actor={"ac-blackball", {player=0}}}
cells["o"]=cell{parent=cells[" "], item={"it-trigger",{action="onoff",target="laser"}}}
level = {
  "B###################",
  "# f   f      f     #",
  "# f f f f_ff   f_f #",
  "#   f   _0   f _0_ #",
  "#_fffffff***ff f_f #",
  "#0     f**+**  fo  #",
  "#_ffff f*+O+*f ffff#",
  "#      f**+**      #",
  "# fffffff***ffff_f #",
  "#    f f   f    0_ #",
  "# f  f   f   fff_f #",
  "# f    f f f       #",
  "####################"
}
oxyd_default_flavor = "d"
create_world_by_map(level)
oxyd_shuffle()
set_attrib(laser(19,3,FALSE,WEST),"name","laser")
set_stone("st-bolder", 1,1, {name="bolder", direction=NORTH})
----------------------------------------------
--functions to control the "dog"
function gon()
  enigma.SendMessage(enigma.GetNamedObject("bolder"), "direction", NORTH)
end
function goe()
  enigma.SendMessage(enigma.GetNamedObject("bolder"), "direction", EAST)
end
function gow()
  enigma.SendMessage(enigma.GetNamedObject("bolder"), "direction", WEST)
end
function gos()
  enigma.SendMessage(enigma.GetNamedObject("bolder"), "direction", SOUTH)
end
-------------------------------------------
--functions to restart the level
ontr = 0
function new1()
--bugfixing:
  if(ontr == 0) then
    ontr = 1
--end of bugfixing
  else
    set_stone("st-switch", 8,6, {action="callback", target="new2"})
    ontr = 0
  end
end

function new2()
  set_stone(wandinnen, 8,6)
  set_stone("st-bolder", 1,1, {name="bolder", direction=NORTH})

  if difficult then 
   enigma.SendMessage(enigma.GetNamedObject("fart"), "trigger", nil)
  end

end
-------------------------------------------
--some triggers
set_item("it-trigger", 10,7, {action="callback", target="gos"})
set_item("it-trigger", 9,6, {action="callback", target="gow"})
set_item("it-trigger", 11,6, {action="callback", target="goe"})
set_item("it-trigger", 10,5, {action="callback", target="gon"})



























