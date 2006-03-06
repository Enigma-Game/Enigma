-- Too Many Holes
-- A Meditation Level for Enigma
--
-- Copyright (c) 2005 Nat Pryce
-- Licensed under the GPL version 2.

randomseed(enigma.GetTicks())

create_world(58,37)
draw_border("st-glass1")
fill_floor("fl-leaves")
draw_border("st-glass1", 19, 12, 20, 13)

set_item("it-hollow", 22, 15, {essential=1})
set_item("it-hollow", 25, 15, {essential=1})
set_item("it-hollow", 28, 15, {essential=1})

set_item("it-hollow", 29, 21, {essential=1})
set_item("it-hollow", 32, 21, {essential=1})
set_item("it-hollow", 35, 21, {essential=1})

set_actor("ac-whiteball-small", 23, level_height/2, {player=0})
set_actor("ac-whiteball-small", 27, level_height/2, {player=0})
set_actor("ac-whiteball-small", 31, level_height/2, {player=0})
set_actor("ac-whiteball-small", 35, level_height/2, {player=0})


function random_x()
    return 19 + random(1,18)
end

function random_y()
    return 12 + random(1,11)
end

TOP=1
BOTTOM=2
LEFT=3
RIGHT=4

side = random(1,4)
if side == TOP then
    x = random_x()
    y = 12
elseif side == BOTTOM then
    x = random_x()
    y = 24
elseif side == LEFT then
    x = 19
    y = random_y()
else -- side == RIGHT
    x = 38
    y = random_y()
end

set_stone( "st-glass1_move", x, y )





























