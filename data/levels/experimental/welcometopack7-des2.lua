-- Welcome VI, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN --
boden="fl-sahara"
wand="st-plain"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
draw_border(wand)

if difficult then
 set_stone("st-fart",6,6,{name="fart1"})
 set_stone("st-timer",13,6,{action="trigger", target="fart1", interval=20})
end

-- ITEMS --
set_item("it-document", 8, 6, {text="Welcome to Enigma VI"})

-- ACTORS --
set_actor("ac-blackball", 11.5,6.5, {player=0})

-- OXYD --
oxyd(2,4)
oxyd(2,6)
oxyd(2,8)
oxyd(2,10)

oxyd(4,10)
oxyd(6,10)
oxyd(8,10)

oxyd(11,2)
oxyd(13,2)
oxyd(15,2)
oxyd(17,2)

oxyd(17,4)
oxyd(17,6)
oxyd(17,8)

oxyd_default_flavor = "d"
oxyd_shuffle()













