-- The Fuze, a level for Enigma
-- Copyright (C) 2005 capkoh
-- Licensed under GPL v2.0 or above 
-- Level had been automatically created with Star.LUA Editor v0.32 beta

create_world(39, 13)
oxyd_default_flavor="b"

-- Code
function grow_wall()
  set_stone("st-rock1", 21, 10)
end

function grow_line()
  for i = 3, 11 do
    set_stone("st-rock1", 36, i)
  end 
  document(35, 11, "End of way!")
end

-- Floor Layer
fill_floor("fl-hay")

-- Item Layer
set_item("it-trigger", 4, 1, {action="open", target="door1"})
set_item("it-trigger", 12, 1, {action="open", target="door2"})
set_item("it-trigger", 21, 1, {action="callback", target="grow_wall", invisible=FALSE})
set_item("it-trigger", 37, 1, {action="callback", target="grow_line"})
set_item("it-trigger", 1, 3, {action="onoff", target="laser1"})
set_item("it-hammer", 10, 4)
set_item("it-document", 7, 7, {text="There is two HIDDEN items!"})
set_item("it-sword", 11, 8)
set_item("it-document", 15, 10, {text="Third stone from the rock is the hole! "})
set_item("it-coin4", 6, 4)

-- Stone Layer
oxyd(38, 10)
oxyd(38, 4)
draw_stones("st-marble", {0, 0}, {1, 0}, 39)
draw_stones("st-marble", {0, 1}, {0, 1}, 12)
draw_stones("st-break_bolder", {5, 1}, {1, 0}, 7)
draw_stones("st-break_bolder", {13, 1}, {1, 0}, 8)
draw_stones("st-break_bolder", {30, 1}, {1, 0}, 7)
draw_stones("st-marble", {38, 1}, {0, 1}, 3)
draw_stones("st-marble", {3, 2}, {1, 0}, 35)
draw_stones("st-marble", {4, 3}, {0, 1}, 7)
draw_stones("st-marble", {12, 3}, {0, 1}, 4)
draw_stones("st-marble", {21, 3}, {0, 1}, 4)
draw_stones("st-marble", {23, 3}, {0, 1}, 4)
draw_stones("st-marble", {29, 3}, {0, 1}, 3)
draw_stones("st-marble", {31, 3}, {1, 0}, 4)
draw_stones("st-marble", {14, 4}, {0, 1}, 5)
draw_stones("st-marble", {25, 4}, {1, 0}, 3)
draw_stones("st-marble", {34, 4}, {0, 1}, 6)
draw_stones("st-marble", {27, 5}, {0, 1}, 6)
draw_stones("st-marble", {30, 5}, {1, 0}, 3)
draw_stones("st-marble", {38, 5}, {0, 1}, 5)
draw_stones("st-marble", {10, 6}, {0, 1}, 4)
draw_stones("st-stone_break", {11, 6}, {0, 1}, 3)
draw_stones("st-marble", {19, 6}, {0, 1}, 5)
draw_stones("st-marble", {24, 6}, {1, 0}, 2)
draw_stones("st-marble", {32, 6}, {0, 1}, 2)
draw_stones("st-marble", {25, 7}, {0, 1}, 2)
draw_stones("st-marble", {28, 7}, {1, 1}, 3)
draw_stones("st-marble", {12, 8}, {0, 1}, 5)
draw_stones("st-marble", {21, 8}, {0, 1}, 2)
draw_stones("st-marble", {23, 8}, {0, 1}, 5)
draw_stones("st-marble", {29, 9}, {0, 1}, 4)
draw_stones("st-marble", {31, 9}, {1, 0}, 2)
draw_stones("st-marble", {25, 10}, {0, 1}, 3)
draw_stones("st-marble", {4, 11}, {0, 1}, 2)
draw_stones("st-marble", {21, 11}, {0, 1}, 2)
fill_stones("st-marble", 31, 11, 2, 2)
draw_stones("st-marble", {34, 11}, {0, 1}, 2)
draw_stones("st-marble", {38, 11}, {0, 1}, 2)
draw_stones("st-marble", {5, 12}, {1, 0}, 7)
draw_stones("st-marble", {13, 12}, {1, 0}, 8)
draw_stones("st-marble", {26, 12}, {1, 0}, 3)
draw_stones("st-marble", {35, 12}, {1, 0}, 3)
set_stone("st-bolder", 1, 1, {name="bolder1", direction=EAST})
set_stone("st-stone_break", 2, 1)
set_stone("st-break_bolder", 3, 1)
set_stones("st-marble", {{1, 2}, {29, 7}, {31, 7}, {11, 9}, {1, 12}, {3, 12}, {22, 12}, {24, 12}, {30, 12}, {33, 12}})
set_stone("st-glass", 2, 2, {type="0"})
set_stone("st-brownie", 6, 4)
set_stone("st-door", 12, 7, {name="door2", type="v"})
set_stone("st-rock1", 21, 7)
set_stone("st-knight", 23, 7)
set_stone("st-coinslot", 28, 8, {action="openclose", target="door3"})
set_stone("st-door", 4, 10, {name="door1", type="v"})
set_stone("st-marble_hole", 21, 10)
set_stone("st-door", 34, 10, {name="door3", type="v"})
set_stone("st-laser", 2, 12, {name="laser1", dir=NORTH, on="0"})

-- Actor Layer
set_actor("ac-blackball", 2.5, 7.5, {player="0", mouseforce="1"})

oxyd_shuffle()


























