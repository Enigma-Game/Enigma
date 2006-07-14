-- Wells -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-03-07

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
cells = {}

cells[" "] = cell{floor="fl-himalaya"}
cells["#"] = cell{stone="st-rock4"}
cells["S"] = cell{{{switch, "switchb", "callback"}}}

cells["e"] = cell{item ="it-extralife"}
cells["s"] = cell{item ="it-spring1"}
cells["O"] = cell{actor={face="ac-blackball", attr={player=0}, mode=2}}

doors1 = {}
doors2 = {}
cells["1"] = cell{{{add_multistone, "st-door_a", doors1, {type="v"}}}}
cells["2"] = cell{{{add_multistone, "st-door_a", doors2, {type="v"}}}}
switch1    = cell{{{send_group_message, doors1, "openclose", nil; mode=-1}}}
switch2    = cell{{{send_group_message, doors2, "openclose", nil; mode=-1}}}
switchb    = cell{{switch1, switch2}}

cells["%"] = cell{{cells["#"], switch2}}

level = {
   "####.##########.####",
   "#0 ###        ### s#",
   "#   1          2   #",
   "#   1          2   #",
   "#   1          2   #",
   "#  ###        ###  #",
   "S  X.#    O   #.X  S",
   "#  ###        ###  #",
   "#   1          2   #",
   "#   1          2   #",
   "#   1          2   #",
   "#e ###        ### 0#",
   "####.##########.###%"
}
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

oxyd_default_flavor = "c"
set_default_parent(cells[" "])
create_world_by_map(level)
















