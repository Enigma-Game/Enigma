-- Welcome V, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN --
boden="fl-metal"
wand="st-metal"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
draw_border(wand)

if difficult then
 set_stone("st-fart",14,6,{name="fart1"})
 set_stone("st-timer",5,6,{action="trigger", target="fart1", interval=15})
end

-- ITEMS --
set_item("it-document", 10, 6, {text="Welcome to Enigma V"})

-- ACTORS --
set_actor("ac-blackball", 9.5,6.5, {player=0})

-- OXYD --
oxyd(2,2)
oxyd(5,2)
oxyd(2,10)
oxyd(5,10)

oxyd(14,2)
oxyd(17,2)
oxyd(14,10)
oxyd(17,10)

oxyd(8,4)
oxyd(8,8)

oxyd(11,4)
oxyd(11,8)

oxyd_default_flavor = "d"
oxyd_shuffle()















