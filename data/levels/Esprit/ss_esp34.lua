-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above

-- ESPRIT #034

-- problem: here is a spermbird needed (more intelligent killer)



levelw = 191

levelh = 13

create_world(levelw, levelh)

oxyd_default_flavor = "d"

set_actor("ac-blackball", 3.5,6.5, {player=0})

set_actor("ac-rotor", 188.5,3.5, {range=200, force=60})

fill_floor("fl-brick",0,0,levelw,levelh)

draw_border("st-rock1")

draw_stones("st-rock1", { 1, 1},{ 1, 0},189)

draw_stones("st-rock1", { 1, 11},{ 1, 0},189)

draw_stones("st-rock1", { 3, 3},{ 1, 0},181)

draw_stones("st-rock1", { 3, 9},{ 1, 0},181)

draw_stones("st-rock1", { 1, 5},{ 1, 0},187)

draw_stones("st-rock1", { 1, 7},{ 1, 0},187)

set_stones("st-rock1",{{184,3},{184,9}})

set_stones("st-oneway-e",{{184,4},{184,8},{187,6}})

set_stones("st-oneway-s",{{1,3},{183,5}})

set_stones("st-oneway-n",{{1,9},{183,7}})

oxyd(0,6)

oxyd(2,3)

oxyd(2,9)

oxyd(189,6)

oxyd_shuffle()

