levelw = 39
levelh = 25

create_world(levelw, levelh)
-- draw_border("st-brownie")

fill_floor("fl-abyss")


current_point = {0,0}
current_dir = NORTH
function set_point(x, y) 
    current_point={x,y}
end
function set_dir(dir)
    current_dir=dir
end
function advance_point()
    local xinc = {0,1,0,-1}
    local yinc = {-1,0,1,0}
    current_point[1] = current_point[1] + xinc[current_dir+1]
    current_point[2] = current_point[2] + yinc[current_dir+1]
end

function draw_with_floor(name, commands)
    for i=1,strlen(commands) do
        local c = strsub(commands, i,i)
        if c=="r" then -- turn right
            local tab = {EAST,SOUTH,WEST,NORTH}
            current_dir = tab[current_dir+1]
        elseif c=="l" then -- turn left
            local tab = {WEST,NORTH,EAST,SOUTH}
            current_dir = tab[current_dir+1]
        else
            for j=1,tonumber(c) do
                -- draw floor
                advance_point()
                set_floor(name, current_point[1], current_point[2])
            end
        end
    end
end

fl="fl-gray"
fill_floor(fl, 1,1,3,3)
fill_floor(fl, 6,6,3,3)
fill_floor(fl, 13,2,3,3)
fill_floor(fl, 15,10,3,3)
fill_floor(fl, 2,7,3,3)
-- fill_floor(fl, 3,12,3,3)

set_stone("st-bombs", 10,7)

set_point(2,2)
set_dir(EAST)
--advance_point()
draw_with_floor(fl, "5r5l4l4r3r5l2r3r94")
set_point()

--fl="fl-sand"
-- draw_floor(fl, {4,2}, {1,0}, 3)
-- draw_floor(fl, {7,2}, {0,1}, 5)
-- draw_floor(fl, {9,8}, {1,0}, 2)
-- draw_floor(fl, {11,8}, {0,-1}, 6)
-- draw_floor(fl, {11,3}, {1,0}, 2)
-- draw_floor(fl, {14,5}, {0,1}, 3)
-- draw_floor(fl, {15,10}, {0,1}, 5)


-- oxyd(3,3)
-- oxyd(level_width-4,level_height-4)
-- oxyd(level_width-4, 3)
oxyd(3,level_height-4)
--oxyd_shuffle()
set_stone("st-chameleon", 3,3)

set_actor("ac-blackball", 2,2)

