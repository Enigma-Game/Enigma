levelw = 20
levelh = 13

create_world(levelw, levelh)
draw_border("st-glass")

fill_floor("fl-normal")

function draw_hole( x, y)
	hollow( x, y)
	draw_floor("fl-gradient", {x-2,y-1}, {0,1}, 3, {type=3})
	set_floor("fl-gradient", x-1, y, {type=3})
	draw_floor("fl-gradient", {x+2,y-1}, {0,1}, 3, {type=4})
	set_floor("fl-gradient", x+1, y, {type=4})
	draw_floor("fl-gradient", {x-1,y-2}, {1,0}, 3, {type=1})
	set_floor("fl-gradient", x, y-1, {type=1})
	draw_floor("fl-gradient", {x-1,y+2}, {1,0}, 3, {type=2})
	set_floor("fl-gradient", x, y+1, {type=2})
	
	draw_floor("fl-gradient", {x-2,y-2}, { 1, 1}, 2, {type=5})
	draw_floor("fl-gradient", {x+2,y-2}, {-1, 1}, 2, {type=6})
	draw_floor("fl-gradient", {x-2,y+2}, { 1,-1}, 2, {type=7})
	draw_floor("fl-gradient", {x+2,y+2}, {-1,-1}, 2, {type=8})
end

draw_hole( 3, 3)
draw_hole( 3, 9)
draw_hole(16, 3)
draw_hole(16, 9)

draw_floor("fl-gradient", {6,1}, {0,1}, 11, {type=4})
draw_floor("fl-gradient", {7,1}, {0,1}, 11, {type=4})
draw_floor("fl-gradient", {8,1}, {0,1}, 11, {type=4})
draw_floor("fl-gradient", {9,1}, {0,1}, 11, {type=4})

draw_floor("fl-gradient", {10,1}, {0,1}, 11, {type=3})
draw_floor("fl-gradient", {11,1}, {0,1}, 11, {type=3})
draw_floor("fl-gradient", {12,1}, {0,1}, 11, {type=3})
draw_floor("fl-gradient", {13,1}, {0,1}, 11, {type=3})


set_actor("ac-whiteball-small", 2, 6.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 3, 6.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 4, 6.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 5, 6.5, {player=0, mouseforce=1})

