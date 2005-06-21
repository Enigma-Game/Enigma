--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.


create_world( 20, 13)
oxyd_default_flavor = "a"

fill_floor( "fl-ice_001")

draw_border( "st-actorimpulse")

oxyd( 3, 0)
oxyd( 7, 0)
oxyd(12, 0)
oxyd(16, 0)
oxyd( 3,12)
oxyd( 7,12)
oxyd(12,12)
oxyd(16,12)
oxyd( 0, 3)
oxyd(19, 3)
oxyd( 0, 9)
oxyd(19, 9)

oxyd_shuffle()

set_stone( "st-death", 1, 0)
set_stone( "st-death", 0, 1)
set_stone( "st-death",18, 0)
set_stone( "st-death",19, 1)
set_stone( "st-death", 1,12)
set_stone( "st-death", 0,11)
set_stone( "st-death",18,12)
set_stone( "st-death",19,11)

set_item( "it-magnet", 1, 1, {name="magnet1", on=1})
set_item( "it-magnet",18, 1, {name="magnet2", on=1})
set_item( "it-magnet", 1,11, {name="magnet3", on=1})
set_item( "it-magnet",18,11, {name="magnet4", on=1})

set_actor("ac-blackball", 10, 6.5)
