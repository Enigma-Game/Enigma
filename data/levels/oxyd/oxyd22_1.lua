--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "c"

fill_floor( "fl-gray")

draw_border( "st-actorimpulse")

fill_stones( "st-chameleon", 1, 1, 18, 11)

set_stone( "st-actorimpulse", 6, 3)
set_stone( "st-actorimpulse", 2, 6)
set_stone( "st-actorimpulse", 4,10)
set_stone( "st-actorimpulse", 7,10)
set_stone( "st-actorimpulse", 8, 6)
set_stone( "st-actorimpulse",11, 3)
set_stone( "st-actorimpulse",11, 9)
set_stone( "st-actorimpulse",14, 2)
set_stone( "st-actorimpulse",14,11)
set_stone( "st-actorimpulse",16, 6)


oxyd( 3, 3)
oxyd( 3, 9)
oxyd( 6, 6)
oxyd(13, 6)
oxyd(16, 3)
oxyd(16, 9)

oxyd_shuffle()

set_actor("ac-blackball", 10, 6.5)











