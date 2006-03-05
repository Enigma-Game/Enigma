-- Danger Slide, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
------------------------------------------------
-- Environment
CreateWorld(20, 13) 
draw_border("st-actorimpulse")
fill_floor("fl-ice_001", 0,0, 20,13)
set_stone("st-death", 3,3)
set_stone("st-death", 3,9)
set_stone("st-death", 16,3)
set_stone("st-death", 16,9)
set_item("it-magnet", 1,1, {on=TRUE})
set_item("it-magnet", 1,11, {on=TRUE})
set_item("it-magnet", 18,1, {on=TRUE})
set_item("it-magnet", 18,11, {on=TRUE})
oxyd_default_flavor = "d"
oxyd(0,1)
oxyd(0,11)
oxyd(19,1)
oxyd(19,11)
oxyd_shuffle()
set_actor("ac-blackball", 10,6.5, {player=0})



















