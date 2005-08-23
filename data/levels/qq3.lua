levelw = 20
levelh = 13

stone="st-rock3"

create_world(levelw, levelh)
fill_floor("fl-samba", 0, 0, levelw, levelh)
draw_border(stone)
draw_stones(stone, {2, 10}, {1, 0}, levelw-3)
draw_stones(stone, {2, 11}, {1, 0}, levelw-3)

draw_stones(stone, {1, 1}, {0, 1}, 3)
draw_stones(stone, {18, 1}, {0, 1}, 3)

for i=3,17,2 do
    set_stone("st-door_b", i, 10, {name="door"..i, type="h"})
    oxyd(i, 11)
    set_item("it-trigger", i, 2, { action="openclose", target="door"..i})
end

set_stone("st-bolder-e", 1, 2)
set_stone("st-bolder-w", 18, 2)
set_item("it-magicwand", 2, 9)

oxyd_shuffle()
set_actor("ac-blackball", 1.5, 10.5)
