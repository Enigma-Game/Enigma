-- Hole_Test, a Testlevel for Enigma
-- Copyright (C) 2006 Raoul+Richi
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN --
boden="fl-sahara"
wand="st-glass"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
draw_border(wand)

-- ITEMS --
set_item("it-document", 10, 6, {text="Absturz bei Holes mit Austrittsort==Hole"})

set_item("it-wormhole", 6, 6, {targetx="6.5",targety="6.5"})

set_item("it-wormhole", 11, 6, {targetx="15.5",targety="6.5"})
set_item("it-wormhole", 15, 6, {targetx="11.5",targety="6.5"})

-- ACTORS --
actor1=set_actor("ac-blackball", 4.5,4.5, {player=0})

-- OXYD --
oxyd(1,1)
oxyd(1,11)
oxyd(18,1)
oxyd(18,11)
oxyd_default_flavor = "d"
oxyd_shuffle()












