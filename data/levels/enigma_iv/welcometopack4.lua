-- Welcome IV, a level for Enigma
-- Copyright (C) 2006 Raoul
-- Licensed under GPL v2.0 or above 

-- GENERAL --
levelw = 20
levelh = 13
create_world(levelw, levelh)
enigma.ConserveLevel = FALSE

-- DESIGN --
boden="fl-sand"
wand="st-marble"

-- FLOOR --
fill_floor(boden, 0,0,levelw,levelh)

-- STONES --
draw_border(wand)

if difficult then
 set_stone("st-fart",9,6,{name="fart1"})
 set_stone("st-timer",10,6,{action="trigger", target="fart1", interval=10})
end

-- ITEMS --
set_item("it-document", 17, 6, {text="Welcome to Enigma IV"})

-- ACTORS --
actor1=set_actor("ac-blackball", 2.5,6.5, {player=0})

-- OXYD --
oxyd(2,3)
oxyd(2,9)
oxyd(5,6)
oxyd(8,3)
oxyd(8,9)

oxyd(11,3)
oxyd(11,9)
oxyd(14,6)
oxyd(17,3)
oxyd(17,9)


oxyd_default_flavor = "d"
oxyd_shuffle()













