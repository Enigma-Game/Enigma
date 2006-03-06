-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Boulder-Puzzle

levelw=40
levelh=13

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c =="#" then
  			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c == "U" then
			set_stone("st-bolder-n", i-1,line)
		elseif c == "R" then
			set_stone("st-bolder-e", i-1,line)
		elseif c == "L" then
			set_stone("st-bolder-w", i-1,line)
 		elseif c == "D" then
			set_stone("st-bolder-s", i-1,line)
		elseif c == "m" then
			set_item( "it-magicwand", i-1,line)
		elseif c == "z" then
			set_actor("ac-blackball", i-1,line)
 		elseif c == "s" then
		        set_stone("st-stoneimpulse",i-1,line)
		elseif c == "," then
			set_item("it-trigger", i-1,line, {action="openclose", target="door6"})
		elseif c == "<" then
                        set_stone("st-door_b", i-1,line, {name="door6", type="v"})
		elseif c == "." then
			set_item("it-trigger", i-1,line, {action="openclose", target="door7"})
		elseif c == ">" then
                        set_stone("st-door_b", i-1,line, {name="door7", type="v"})
		elseif c == "/" then
			set_item("it-trigger", i-1,line, {action="openclose", target="door8"})
		elseif c == "?" then
                        set_stone("st-door_b", i-1,line, {name="door8", type="v"})
		elseif c == ";" then
			set_item("it-trigger", i-1,line, {action="openclose", target="door9"})
		elseif c == ":" then
                        set_stone("st-door_b", i-1,line, {name="door9", type="v"})
		elseif c == "!" then
           -- bolders now fall into abyss, replaced abyss by water [ralf]
            set_floor("fl-water",i-1,line)
			--abyss(i-1,line)
		end
	end
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"


renderLine(00,"#######################################")
renderLine(01,"# Ds!          #  !!! .          <>?:o#")
renderLine(02,"#;#!!       #L! R#!sD    !#!!!   ######")
renderLine(03,"#           !s! s !!#!!! ! Ds!        #")
renderLine(04,"# !!!       !!!    !!! # !/#!! !!! #L!#")
renderLine(05,"# Ds!    m   DL    !R#L! !!!   !sD !s!#")
renderLine(06,"# g!! z     !RU    !sD     #!  #L#!!!!#")
renderLine(07,"#!!         !!s#!  !!#    !Us  !!!s!  #")
renderLine(08,"#s!        !!ssU!   #!!   !!!    #L!  #") -- renderLine(08,"#s!        !!ssU!   #!!   !!!    #L!  #")
renderLine(09,"#R#        !oR#!!   Us!          !!#  #")
renderLine(10,"#          !!!       !!          !sU  #")
renderLine(11,"#L                          ,   R#    #")
renderLine(12,"#######################################")

oxyd_shuffle()

-- bolders now fall into abyss, replaced abyss by water [ralf]
--abyss(13,6)
--abyss(14,6)
--abyss(22,5)
set_floor("fl-water",13,6)
set_floor("fl-water",14,6)
set_floor("fl-water",22,5)















