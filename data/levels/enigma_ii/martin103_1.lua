--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 96
levelh = 13
create_world(levelw, levelh)

fill_floor("fl-gradient1")

draw_border("st-actorimpulse")

oxyd( 10, 2)
oxyd( 20, 10)
oxyd( 30, 7)
oxyd( 40, 9)
oxyd( 50, 4)
oxyd( 60, 5)
oxyd( 70, 1)
oxyd( 80, 8)

oxyd_shuffle()

set_actor("ac-rotor", 79, 6.5, {controllers=1, range=70, force=50})
set_actor("ac-blackball", 94, 6.5)

display.SetFollowMode(display.FOLLOW_SCROLLING)
























