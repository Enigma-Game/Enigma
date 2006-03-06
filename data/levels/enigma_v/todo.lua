-- created by Martin Hawlisch

levelw = 20
levelh = 13

create_world( levelw, levelh)

draw_border("st-rock1")

fill_floor("fl-abyss", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		end
	end
end

-- Floor:   " "
-- Border:  "#"

renderLine(  2, "  ###  #  ##   #")
renderLine(  3, "   #  # # # # # #")
renderLine(  4, "   #  # # # # # #")
renderLine(  5, "   #  # # # # # #")
renderLine(  6, "   #   #  ##   #")

hollow( 3, 10)
hollow( 7, 10)
hollow(12, 10)
hollow(16, 10)
set_floor( "fl-space", 3, 10)
set_floor( "fl-space", 7, 10)
set_floor( "fl-space",12, 10)
set_floor( "fl-space",16, 10)


set_actor("ac-whiteball-small",  3.5,10.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small",  7.5,10.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 12.5,10.5, {player=0, mouseforce=1})
set_actor("ac-whiteball-small", 16.5,10.5, {player=0, mouseforce=1})



















