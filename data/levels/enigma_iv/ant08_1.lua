-- Mourning Palace -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-07
-- 2003-02-20 -- rewrites

Require("levels/lib/ant.lua")

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
function construct(x, y) set_floor("fl-normal", x, y) end
function destruct(x, y)  set_floor("fl-abyss", x, y) end
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[";"]=cell{floor={face="fl-metal"}}
cells["'"]=cell{floor={face="fl-normal"}}
cells[","]=cell{floor={face="fl-tigris"}}
cells["."]=cell{floor={face="fl-abyss"}}
cells[" "]=cell{parent={{randomfloor,{cells[","], cells[";"], 10}}}}
cells["="]=cell{stone={face="st-glass"}}
cells["O"]=cell{actor={face="ac-blackball", attr={player=0}}}

path = {}
loco = {}
cells["!"]=cell{parent={cells["."], {add_multicell, path, 0}}}
cells["_"]=cell{parent={cells["!"], {add_multicell, path, 1}, cells["'"]}}
cells["c"]=cell{parent={cells["_"], {add_multicell, loco, construct}}}
cells["d"]=cell{parent={cells["!"], {add_multicell, loco, destruct}}}
rail = new_rail(loco, path)

cells["~"]=cell{stone={face="st-timer", attr={action="callback", target="rail", interval=0.15}}}
cells["0"]=oxyd

level = {
   "==========================================================",
   "=!!!!!!!!!!d______c!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!=",
   "=!0                ====================                0!=",
   "=!!!!!!!!!!!!c_ = !!!!!!!!!!!c______d!!!!!!!!!!!!!!!!!!!!=",
   "==============_   !==================================== ==",
   "=!!!!!!!!!!!!=_ = !=!!c______d!!!!!!!!=!!!!!!!!!!!!!!=!!!=",
   "=!          !=_ = !=!               0!=!           0!=! !=",
   "=!          !=_ = ! ! !!!!!!!!!!!!!!!!=! !!!!!!!!!!!!=! !=",
   "=!  0    O  ! _   !=! !================! !============! !=",
   "=!          !=d = !=! !=!!!!d______c!!!! !=!!d______c!! !=",
   "=!          !=!   !=! ! !0           0=  ! !0           !=",
   "=c______d!!!!=!!!!!=!!!=!!!!!!!!!!!!!!!!!!=!!!!!!!!!!!!!!=",
   "~========================================================="
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
















