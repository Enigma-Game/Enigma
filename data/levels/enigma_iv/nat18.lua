-- A meditation level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Licensed under the GPL version 2.

create_world( 20, 13 )
fill_floor( "fl-leaves" )
draw_border( "st-glass1" )

set_item( "it-hollow", 2, 2 )
set_item( "it-hollow", 2, 4 )
set_item( "it-hollow", 2, 6 )
set_item( "it-hollow", 2, 8 )
set_item( "it-hollow", 2, 10 )
set_item( "it-hollow", 4, 2 )
set_item( "it-hollow", 6, 2 )
set_item( "it-hollow", 8, 2 )
set_item( "it-hollow", 4, 10 )
set_item( "it-hollow", 6, 10 )
set_item( "it-hollow", 8, 10 )
set_item( "it-hollow", 10, 2 )
set_item( "it-hollow", 10, 4 )
set_item( "it-hollow", 10, 6 )
set_item( "it-hollow", 10, 8 )
set_item( "it-hollow", 10, 10 )


CENTER_X = 15
CENTER_Y = 6.5
RADIUS = 2
BALL_COUNT = 16
BAND_STRENGTH = 16
BAND_LENGTH = 2*RADIUS*sin(360/(2*BALL_COUNT))

last_ball = nil
first_ball = nil
for theta=0,359,360/BALL_COUNT do
    x = CENTER_X + RADIUS*cos(theta)
    y = CENTER_Y + RADIUS*sin(theta)
    
    new_ball = set_actor( "ac-whiteball-small", x, y, {player=0} )
    if last_ball then
        AddRubberBand( last_ball, new_ball, BAND_STRENGTH, BAND_LENGTH )
    else
        first_ball = new_ball
    end
    last_ball = new_ball
end
AddRubberBand( last_ball, first_ball, BAND_STRENGTH, BAND_LENGTH )













