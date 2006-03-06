-- Swamp of Balls, a level for enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
--------------------------------------------------
-- Function to open the oxyds
grad = 1
function comeback()
  if(grad == 1) then
    draw_floor("fl-wood", {18,1}, {0,2}, 6)
    grad = 2
  else
    draw_floor("fl-gradient", {18,1}, {0,2}, 6, {type=3})
    grad = 1
  end
end
--------------------------------------------------
-- Environment
CreateWorld(20, 13) 
draw_border("st-rock1")
fill_floor("fl-wood", 0,0, 20,13)
fill_floor("fl-swamp", 0,0, 17,13)
draw_stones("st-white4", {18,1}, {0,2}, 6)
draw_stones("st-rock1", {18,2}, {0,2}, 5)
draw_floor("fl-gradient", {18,1}, {0,2}, 6, {type=3})
set_floor("fl-wood", 4,4)
set_actor("ac-whiteball", 4.5,8.5, {mouseforce=0})
set_floor("fl-wood", 4,8)
set_actor("ac-whiteball", 4.5,4.5, {mouseforce=0})
set_floor("fl-wood", 8,4)
set_actor("ac-whiteball", 8.5,4.5, {mouseforce=0})
set_floor("fl-wood", 12,4)
set_actor("ac-whiteball", 12.5,4.5, {mouseforce=0})
set_floor("fl-wood", 8,8)
set_actor("ac-whiteball", 8.5,8.5, {mouseforce=0})
set_floor("fl-wood", 12,8)
set_actor("ac-whiteball", 12.5,8.5, {mouseforce=0})
oxyd_default_flavor = "c"
oxyd(19,1)
oxyd(19,3)
oxyd(19,5)
oxyd(19,7)
oxyd(19,11)
oxyd(19,9)
oxyd_shuffle()
set_item("it-trigger", 17,6, {action="callback", target="comeback"})
set_actor("ac-blackball",17.5,6.5, {player=0})
























