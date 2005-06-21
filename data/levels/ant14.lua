-- Dancers -- the Enigma Level
-- (c) 2003 Petr Machata/ant_39
-- Licensed under GPL v2.0 or above
-- 2003-01-11
-- 2003-02-10 -- keeping up to date with latest additions to ant.lua

dofile(enigma.FindDataFile("levels/ant.lua"))

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
count_revert = nil
revert_dir = nil

function revert_bolders()
   --open/close all doors in map
   send_group_message(doors, "openclose", nil)

   --on every odd triggger invert bolder dir
   if (not(count_revert)) then
      if (revert_dir)
      then send_group_message(bolders, "direction",EAST)
      else send_group_message(bolders, "direction",WEST)
      end
      revert_dir = not(revert_dir)
   end

   count_revert = not(count_revert)
end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

cells[" "]=cell{floor="fl-marble"}
cells["."]=cell{floor="fl-water"}
cells["#"]=cell{parent=cells["."],stone="st-marble"}
cells["V"]=cell{{{laser, TRUE, SOUTH}}}
cells["^"]=cell{{{laser, TRUE, NORTH}}}
cells["+"]=cell{item={"it-trigger", {action="callback", target="revert_bolders"}}}

bolders = {}
doors   = {}
cells[">"]=cell{{{add_multistone, "st-bolder-e", bolders}}}
cells["="]=cell{{{add_multistone, "st-door_a", doors, {type="h"}}, cells["."]}}

cells["0"]=oxyd
cells["O"]=cell{actor={"ac-blackball", {player=0}}}

level = {
   "#########V##########",
   "#.......#=#........#",
   "#..0............0..#",
   "#+.>     >       ..#",
   "#......... ........#",
   "#..0...... .....0..#",
   "#..>   O >       ..#",
   "#..0..... ......0..#",
   "#........ .........#",
   "#..>     >       .+#",
   "#..0............0..#",
   "#........#=#.......#",
   "##########^#########"
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
oxyd_default_flavor="c"
set_default_parent(cells[" "])
create_world_by_map(level)
oxyd_shuffle()
