-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Islands

levelw=50
levelh=50

create_world( levelw, levelh)
enigma.SetCompatibility("oxyd1")

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
--			abyss(i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="r" then
			oneway(i-1,line, enigma.EAST)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="*" then
			set_stone( "st-wood", i-1, line)
			fill_floor("fl-water", i-1,line, 1,1)
		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"####################")
renderLine(01,"#  #  !!!!!!!!!!! o#")
renderLine(02,"#  ++ !!!!!!!!!!!  #")
renderLine(03,"#    !!!!!!!!!!!!  #")
renderLine(04,"#  ##+   +!!!!! !  #")
renderLine(05,"#  #!  +  !!!!! !  #")
renderLine(06,"#  #! +z+ !!!!! !  #")
renderLine(07,"#  #!  +  !!!!! !  #")
renderLine(08,"# !!!+   +!!!!!+#  #")
renderLine(09,"#  !!!!! !!!!!!!!  #")
renderLine(10,"#!+!!####*!!!   !  #")
renderLine(11,"#!      !!! + + # o#")
renderLine(12,"####################")

oxyd_shuffle()














