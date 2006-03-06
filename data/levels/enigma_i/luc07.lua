-- Flying, a level for Enigma
-- Copyright (C) 2005 Lukas Schueller
-- Licensed under GPL v2.0 or above
-------------------------------------------------
-- Environment
CreateWorld(39, 13) 
draw_border("st-rock2")
fill_floor("fl-metal", 0,0, 20,13)
fill_floor("fl-abyss", 20,0, 19,13)
oxyd_default_flavor = "c"
oxyd(1,1)
oxyd(1,11)
oxyd(1,6)
oxyd(19,1)
oxyd(19,11)
oxyd(37,1)
oxyd(37,11)
oxyd(37,6)
oxyd_shuffle()
set_item("it-umbrella", 10,6)
set_item("it-document", 19,6, {text="You can fly a little time, if you use the umbrella"})
set_actor("ac-blackball",9.5,6.5, {player=0})






















