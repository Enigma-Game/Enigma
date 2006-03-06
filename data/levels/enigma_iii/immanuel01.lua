-- Portable Laser
-- (c) 2003 by Immanuel Herrmann
-- Licensed under GPL v2.0 or above

create_world(20, 13)

oxyd_default_flavor = "a"

draw_border("st-rock5")

fill_floor("fl-sahara", 0, 0, 20, 13)
fill_floor("fl-abyss", 16, 1, 3, 11)

set_stone("st-switch", 4, 4, {action="onoff", target="laser1"})
set_stone("st-wood", 4, 8)
set_stone("st-swap", 8, 4)
set_stone("st-wood_001", 8, 8)

set_attrib(laser(12, 4, FALSE, EAST), "name", "laser1")
set_stone("st-grate1", 12, 8)

oxyd(19, 4)
oxyd(19, 8)

set_actor("ac-blackball", 6.5, 6.5)
















