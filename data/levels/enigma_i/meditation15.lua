-- Siegfried Fennig 26.12.2002 Rev. 0

levelw = 59
levelh = 38

create_world(levelw, levelh)
fill_floor("fl-abyss", 0,0, levelw,levelh)
fill_floor("fl-normal",20,18, 18,1)
fill_floor("fl-normal",28,14, 2,2)
fill_floor("fl-normal_x",28,21, 2,2)

draw_floor("fl-gradient", {38,17}, {0,1}, 3, {type=4})
draw_floor("fl-gradient", {19,17}, {0,1}, 3, {type=3})
draw_floor("fl-gradient", {22,17}, {1,0}, 1, {type=1})
draw_floor("fl-gradient", {26,17}, {1,0}, 6, {type=1})
draw_floor("fl-gradient", {35,17}, {1,0}, 1, {type=1})
draw_floor("fl-gradient", {22,19}, {1,0}, 1, {type=2})
draw_floor("fl-gradient", {26,19}, {1,0}, 6, {type=2})
draw_floor("fl-gradient", {35,19}, {1,0}, 1, {type=2})

draw_floor("fl-gradient", {21,17}, {1,0}, 1, {type=11})
draw_floor("fl-gradient", {23,17}, {1,0}, 1, {type= 9})
draw_floor("fl-gradient", {25,17}, {1,0}, 1, {type=11})
draw_floor("fl-gradient", {32,17}, {1,0}, 1, {type= 9})
draw_floor("fl-gradient", {34,17}, {1,0}, 1, {type=11})
draw_floor("fl-gradient", {36,17}, {1,0}, 1, {type= 9})
draw_floor("fl-gradient", {21,19}, {1,0}, 1, {type=12})
draw_floor("fl-gradient", {23,19}, {1,0}, 1, {type=10})
draw_floor("fl-gradient", {25,19}, {1,0}, 1, {type=12})
draw_floor("fl-gradient", {32,19}, {1,0}, 1, {type=10})
draw_floor("fl-gradient", {34,19}, {1,0}, 1, {type=12})
draw_floor("fl-gradient", {36,19}, {1,0}, 1, {type=10})

function dh1( x, y)
       hollow( x, y)
       draw_floor("fl-gradient", {x-1,y}, {0,1}, 3, {type=3})
       set_floor("fl-gradient",   x, y-1,           {type=1})
       draw_floor("fl-gradient", {x+1,y}, {0,1}, 3, {type=4})
       set_floor("fl-gradient",   x-1, y-1,         {type=5})
       set_floor("fl-gradient",   x+1, y-1,         {type=6})
       draw_floor("fl-normal",   {x,y}, {0,1}, 4)
end

dh1 (20,14)
dh1 (24,14)
dh1 (33,14)
dh1 (37,14)

function dh2( x, y)
       hollow( x, y)
       draw_floor("fl-gradient", {x-1,y-2}, {0,1}, 3,{type=3})
       set_floor("fl-gradient",   x, y+1,            {type=2})
       draw_floor("fl-gradient", {x+1,y-2}, {0,1}, 3,{type=4})
       set_floor("fl-gradient",   x-1, y+1,          {type=7})
       set_floor("fl-gradient",   x+1, y+1,          {type=8})
       draw_floor("fl-normal",   {x,y-3}, {0,1}, 4)
end

dh2 (20,22)
dh2 (24,22)
dh2 (33,22)
dh2 (37,22)

function ac1( x, y)
       draw_floor("fl-gradient", {x-1,y}, {0,1}, 2, {type=3})
       draw_floor("fl-gradient", {x, y-1},{1,0}, 2, {type=1})
       draw_floor("fl-gradient", {x+2,y}, {0,1}, 2, {type=4})
       set_floor("fl-gradient",   x-1, y-1,         {type=5})
       set_floor("fl-gradient",   x+2, y-1,         {type=6})
       set_floor("fl-gradient",   x-1, y+2,         {type=7})
       set_floor("fl-gradient",   x+2, y+2,         {type=8})
       draw_floor("fl-gradient", {x,y+2}, {1,0}, 2, {type=2})
end

ac1 (28,14)
ac1 (28,21)

set_actor("ac-whiteball-small", 28.5, 14.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 29.5, 14.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 28.5, 15.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 29.5, 15.5, {player=0, mouseforce=1, essential=1})

set_actor("ac-whiteball-small", 28.5, 21.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 29.5, 21.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 28.5, 22.5, {player=0, mouseforce=1, essential=1})
set_actor("ac-whiteball-small", 29.5, 22.5, {player=0, mouseforce=1, essential=1})













