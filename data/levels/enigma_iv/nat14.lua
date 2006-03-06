-- Snowball Fight
-- A level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Licensed under the GPL version 2.


create_world( 20, 13 )
fill_floor( "fl-samba" )
fill_floor( "fl-ice_001", 7, 1, level_width-8, level_height-2 )
fill_floor( "fl-samba", 1, 1, 6, level_height-2 )

draw_border( "st-actorimpulse")
draw_stones( "st-white4", {7,1}, {0,1}, level_height-2 )  

oxyd( level_width-1, 1 )
oxyd( level_width-1, 3 )
oxyd( level_width-1, 5 )
oxyd( level_width-1, 7 )
oxyd( level_width-1, 9 )
oxyd( level_width-1, 11 )
oxyd_shuffle()

anchor = set_stone( "st-stone1", 0, 6 ) 
black = set_actor( "ac-blackball", 2.5, 6.5 )
white1 = set_actor( "ac-whiteball", 5.5, 3.5 )
white2 = set_actor( "ac-whiteball", 5.5, 9.5 )

AddRubberBand( black, anchor, 10, 2 )















