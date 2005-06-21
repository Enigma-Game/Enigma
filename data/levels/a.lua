levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-brownie")
fill_floor("fl-hay")


oxyd(3,3)
oxyd(level_width-4,level_height-4)
oxyd(level_width-4, 3)
oxyd(3,level_height-4)
--oxyd_shuffle()

set_actor("ac-blackball", 10,6.5)

