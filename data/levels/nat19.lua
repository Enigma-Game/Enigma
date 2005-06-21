-- Eccentric Orbits
-- A level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Licensed under the GPL version 2.

create_world( 20, 13 )
fill_floor( "fl-tigris" )
fill_floor( "fl-leaves", 2, 2, level_width-4, level_height-4 )
draw_border( "st-rock5" )
draw_border( "st-white1", 2, 2, level_width-4, level_height-4 )

set_stone( "st-rotator-left", 1, 0 )
set_stone( "st-rotator-left", level_width-1, 1 )
set_stone( "st-rotator-left", level_width-2, level_height-1 )
set_stone( "st-rotator-left", 0, level_height-2 )

bolder1 = set_stone( "st-bolder-n", 1, 6)
bolder2 = set_stone( "st-bolder-s", level_width-2, 6)
white = set_actor( "ac-whiteball", level_width/2, level_height/2,
                   {player=0,controllers=0} )
killer1 = set_actor( "ac-killerball", 5.5, level_height/2, {controllers=0} )
killer2 = set_actor( "ac-killerball", level_width-5.5, level_height/2, {controllers=0} )

AddRubberBand( killer1, bolder1, 10, 4 )
AddRubberBand( killer1, white, 2, 1 )
AddRubberBand( killer2, white, 2, 1 )
AddRubberBand( killer2, bolder2, 10, 4 ) 

oxyd_default_flavor = "d"
oxyd(6,0)
oxyd(13,0)
oxyd(6,12)
oxyd(13,12)
oxyd_shuffle()

set_actor( "ac-blackball", level_width/2, 7.5 )


