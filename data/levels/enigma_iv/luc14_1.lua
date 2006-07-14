-- Pull it!, a level for enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
------------------------------------------------
-- Environment
CreateWorld(20, 13) 
draw_border("st-yellow")
fill_floor("fl-sand", 0,0, 20,13)
set_stone("st-pull", 1,11)
set_stone("st-pull", 1,2)
draw_stones("st-yellow", {2,1}, {0,1}, 3)
draw_stones("st-yellow", {9,10}, {0,1}, 2)
draw_stones("st-yellow", {11,10}, {0,1}, 2)
draw_stones("st-wood", {18,10}, {0,1}, 2)
set_stone("st-door", 1,3, {type="h", name="doora"})
set_stone("st-door", 10,10, {type="h", name="doorb"})
set_item("it-trigger", 1,1, {action="openclose", target="doorb"})
set_item("it-trigger", 18,1, {action="openclose", target="doora"})
set_item("it-puller-w", 1,5)
set_item("it-puller-w", 2,7)
set_item("it-puller-s", 2,5)
set_item("it-puller-s", 1,6)
set_item("it-puller-e", 2,6)
set_item("it-puller-e", 1,7)
oxyd_default_flavor = "c"
oxyd(10,12)
oxyd(19,11)
oxyd_shuffle()
set_actor("ac-blackball", 10,6.5, {player=0})




















