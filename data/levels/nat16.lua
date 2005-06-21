-- Squaring the Circle
-- A level for Enigma
--
-- Copyright (c) 2003 Nat Pryce
-- Licensed under the GPL version 2.

enigma.ConserveLevel = FALSE 

create_world( 20, 13 )
draw_checkerboard_floor( "fl-rough-blue", "fl-rough-red",
                         0, 0, level_width, level_height )
draw_border( "st-wood_001" )

oxyd_default_flavor = "a"
oxyd(3,0)
oxyd(7,0)
oxyd(12,0)
oxyd(16,0)
oxyd(3,12)
oxyd(7,12)
oxyd(12,12)
oxyd(16,12)
oxyd(0,6)
oxyd(19,6)
oxyd_shuffle()

PLAYER_X = 10
PLAYER_Y = 6.5
player1 = set_actor( "ac-blackball", PLAYER_X, PLAYER_Y )

if not difficult then
    RADIUS = 2.5 -- easy mode
else
    RADIUS = 1.5 -- difficult mode
end
BALL_COUNT = 20
BAND_STRENGTH = 10
BAND_LENGTH = 2*RADIUS*sin(360/(2*BALL_COUNT))

last_ball = nil
first_ball = nil
for theta=0,359,360/BALL_COUNT do
    x = PLAYER_X + RADIUS*cos(theta)
    y = PLAYER_Y + RADIUS*sin(theta)
    
    new_ball = set_actor( "ac-killerball", x, y, 
                          {mouseforce=1, controllers=1} )
    if last_ball then
        AddRubberBand( last_ball, new_ball, BAND_STRENGTH, BAND_LENGTH )
    else
        first_ball = new_ball
    end
    last_ball = new_ball
end
AddRubberBand( last_ball, first_ball, BAND_STRENGTH, BAND_LENGTH )
