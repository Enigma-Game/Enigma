-- Laser Room -- Oxyd Magnum -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-21

Require("levels/lib/ant.lua")
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells["."] = abyss
cells[" "] = cell{floor="fl-metal"}
cells["#"] = cell{stone="st-rock2"}
cells["W"] = cell{stone="st-wood"}
cells["-"] = cell{{{mirrorp, FALSE, FALSE, 1}}}
cells["/"] = cell{{{mirrorp, FALSE, TRUE, 2}}}
cells["^"] = cell{{{mirror3, FALSE, FALSE, 3}}}
cells["o"] = cell{actor={"ac-blackball", {player=0}}}
cells["L"] = cell{stone={"st-laser", {on=FALSE, dir=NORTH, name="laser1"}}}
cells["S"] = cell{{{switch, "laser1", "onoff"}}}
cells["0"] = oxyd

level = {
   "###0##0#####0##0####",
   "# ......   ......  #",
   "#                  #",
   "#                  #",
   "#     W            #",
   "#                  #",
   "#  -  ^  -  /  -   #",
   "#.         o      .#",
   "0.    -     -     .0",
   "#.                .#",
   "#                  #",
   "#        LS        #",
   "####################"
}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "d"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()










