-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: More Space Tubes

levelw=60
levelh=30

create_world( levelw, levelh)

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "=" then
			fill_floor("fl-bluegreen", i-1,line, 1,1)
		elseif c == "+" then
			fill_floor("fl-space", i-1,line, 1,1)
		elseif c == "&" then
			set_stone( "st-greenbrown", i-1, line)
			fill_floor("fl-space", i-1,line, 1,1)
		elseif c == "a" then
  			set_stone("st-switch", i-1,line, {action="openclose", target="door1"})
		elseif c == "A" then
			set_stone("st-door", i-1, line , {name="door1", type="v"})
		elseif c == "z" then
			set_actor("ac-blackball", i-1+.5,line+.5)
		elseif c == "d" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "u" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "r" then --3-r
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "l" then --4-l
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c == "1" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c == "3" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
		elseif c == "7" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
		elseif c == "9" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"##########################################################")
renderLine(01,"#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!uuuuuurruuduulluuuudd!#")
renderLine(02,"#!dddduddldrrdddudddr++lr++lr++lr++l!!!!!!!!!!!!!!!!!!!=!#")
renderLine(03,"#!ouuuduulurruuuduuur++lr++lr++lr++l!l==r++++r==l!!!!!!=!#")
renderLine(04,"#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l==r++++r==++==+=+=!#")
renderLine(05,"#!3ddd1!3ddddd1!3dd1!!!!!!3dddddd1!!!!++!!!!!9uu7!!!!!!!!#")
renderLine(06,"#!r===1d3+++++1d1==l!!!!!!r++++++l!!!!++!!!!!!!!!!!!3dd1!#")
renderLine(07,"#!roz=++++++&++++==l!!!!!!r+7uu9+l!!!!++!!!!!!!!!!!!r=+l!#")
renderLine(08,"#!r===7u9+++++7u7==l!!!!!!r+l!!r+l!!!3dd1!=rrr++ll=++==l!#")
renderLine(09,"#!9uuu7!9uuuuu7!93+7!3dddd!+!dd3+l!!!r==l!=!!!!!!!!!93+l!#")
renderLine(10,"#!!!!!!!!!!!!!!!!r+l!r+++++++++++l!!!r==l!=llrr++l=!!r+l!#")
renderLine(11,"#!ar===l==!!!!!!!r+l!r+7uu!+!uuuu7!!!9uu7!!!!!!!!!=!!r+l!#")
renderLine(12,"#!!!!!!!++!!!!!!!r+l!r+l!!r+l!!!!!!!!!!!=rrrr+llll=!!r+l!#")
renderLine(13,"#!3dddddddddddd1!r+1d3+l!!r+l!3ddddddd1!!!!!!!!!!!!!!l+r!#")
renderLine(14,"#!r+7u9l+r++++!l!r+++++l!!!+!!r+++++++dd#udduuddddd1!l+r!#")
renderLine(15,"#!!+l+r1d3dd1++!!9uuuuu7!!r+l!r+3ddd1+++A++++++++++l!l+r!#")
renderLine(16,"#!!+1!+d!r++l+!!!!!!!!!!!!r+1d3+r+++l+uu#duuddu3+++l!rul!#")
renderLine(17,"#!r+++r+l9uu7++l!!!!!!!!!!r+++++r+++l+!!!!!!!!!r+++l!rul!#")
renderLine(18,"#!r+++9u7++++++l7!!!!!!!!!9uuu9+r+++l+ddddd1!!!!+++l!rul!#")
renderLine(19,"#!r+3d1++7u9+7u97uu9!!!!!!!!!!r+9uuu7++++++l!!!r+++!!rul!#")
renderLine(20,"#!r+r!l++l+r+l+rl==r!!!!!7uu9!r+++++++uuu9+l!!!r+++1d3+l!#")
renderLine(21,"#!r+9u7!+1d3+1d3l==r+++++l==r!9uuuuuuu7!!r+l!!!r+++++++l!#")
renderLine(22,"#!9uuuuuuuuuuuuu1dd3+++++l==r!!!!!!!!!!!!r+l!!!9uuuuuuu7!#")
renderLine(23,"#!!!!!!!!!!!!!!!!!!!!!!!!1dd3dddudduddddd3+l!!!!!!!!!!!!!#")
renderLine(24,"##########################################################")

fill_floor("fl-space", 40,15, 1,1)

oxyd_shuffle()















