levelw = 20
levelh = 25

create_world(levelw, levelh)
draw_border("st-glass")
draw_stones("st-glass", {0, 12}, {1, 0}, levelw-3)
fill_floor("fl-leaves")

draw_stones("st-grate1", {7, 13}, {0, 1}, levelh-14)
draw_stones("st-brick_magic", {4, 13}, {0, 1}, levelh-14)
draw_stones("st-brick_magic", {5, 23}, {1, 0}, levelw-6)

set_attrib(laser(0, 0, TRUE, EAST), "name", "laser")
draw_stones("st-glass", {5,1}, {2,0}, levelw-1)

for i=6, levelw-4, 2 do
    mirrorp(i, 0, FALSE, TRUE, 2)
    
    doorh(i, 1, {name="l_door"..i})
    SendMessage("l_door"..i, "open")
    
    if i>8 then
        mirror3(i, levelw-i+11, TRUE, FALSE, 3)
        oxyd(i, levelh-1)
        oxyd(2, levelw-i+11)
    end
    
    set_item("it-trigger", i-1, 11, { action="open", target="l_door"..i })
    --	set_item("it-trigger", i-1, 10, { action="open", target="p_door"..i })
end

set_item("it-trigger", 18, 1, { action="callback", target="close_all" })
draw_stones("st-glass", {17, 2}, {0, 1}, 3)
doorh(18, 2, { name="switchdoor" })
doorh(18, 4, { name="entrance" })
SendMessage("entrance", "open")

set_item("it-magicwand", 1, 1)
draw_stones("st-glass", {2, 1}, {0, 1}, 4)

for i=2, 4 do
    doorh(1,i, { name="w_door"..i })
    set_item("it-trigger", 1, i+5, { action="openclose", target="w_door"..i})
end

set_item("it-trigger", 18, 11, { action="openclose", target="switchdoor"})
set_stone("st-brownie", 11, 16)

oxyd_shuffle()
set_actor("ac-blackball", 18.5, 3.5)

function close_all()
    for i=6, levelw-4, 2 do
	SendMessage("l_door"..i, "close")
    end
end

