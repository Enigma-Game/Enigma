-- Automaton Magic -- the Enigma Level
-- (c) 2002 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua
-- 2003-06-23 -- changed abyss to water, because st-bolder now falls into abyss [ralf]

dofile(enigma.FindDataFile("levels/lib/ant.lua"))
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

stepon = nil
function turnbolder()
   stepon = not(stepon)
   if (stepon) then
      local bolder_dir=enigma.GetAttrib(enigma.GetNamedObject("bolder1"), "direction") -1
      while (bolder_dir<0) do bolder_dir = bolder_dir +4 end
      SendMessage("bolder1", "direction",bolder_dir)
   end
end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}
cells[" "]=cell{floor="fl-sand"}
cells["."]=cell{floor="fl-water"}
cells["#"]=cell{stone="st-greenbrown"}
cells["O"]=cell{actor={"ac-blackball", {player=0}}}
cells["m"]=cell{item= {"it-magicwand"}}
cells["T"]=cell{parent=cells["."],item={"it-trigger", {action="callback", target="turnbolder", invisible=1}}}
cells[">"]=cell{stone={"st-bolder-e", {name="bolder1"}}}
cells["0"]=oxyd

level = {
   "####0##########0####",
   "#....          ....#",
   "#.                .#",
   "#.                .#",
   "0.  T   >      T  .0",
   "#                  #",
   "#       m O        #",
   "#                  #",
   "0.  T          T  .0",
   "#.                .#",
   "#.                .#",
   "#....          ....#",
   "####0##########0####"
}


-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor = "a"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()















