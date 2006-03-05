-- Trigger Happy, a level for Enigma
-- Copyright (C) 2005 Xerxes M. Dynatos
-- Licensed under GPL v2.0 or above 

levelw=42
levelh=18

create_world( levelw, levelh)

fill_floor("fl-tigris", 0,0,levelw,levelh) --fl-leaves --fl-acblack

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-glass1", i-1, line)

		elseif c == "=" then
			set_stone( "st-glass", i-1, line)

		elseif c == "o" then
			oxyd( i-1, line)

		elseif c == "H" then
			set_stone( "st-glass1_hole", i-1, line)

                elseif c == "!" then
			fill_floor("fl-water", i-1,line, 1,1)

		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)

		elseif c=="+" then
			set_stone( "st-wood", i-1, line)

		elseif c=="A" then
			doorv(i-1,line,{name="door1"})

		elseif c=="a" then
			set_stone("st-switch", i-1, line, {action="onoff", target="laser1"})

		elseif c == "K" then
			set_stone( "st-knight", i-1, line)
			set_item("it-key_a",i-1, line)

		elseif c == "k" then
			set_stone( "st-knight", i-1, line)
			set_item("it-key_b",i-1, line)

		elseif c == "y" then
			set_stone("st-key_a", i-1, line, {action="openclose", target="door3"})

		elseif c == "f" then
			set_stone("st-key_b", i-1, line, {action="openclose", target="door5"})

		elseif c=="E" then
			doorh(i-1,line,{name="door4"})

		elseif c=="F" then
			doorh(i-1,line,{name="door5"})

		elseif c=="r" then
			doorv(i-1,line,{name="door3"})

		elseif c == "m" then
			set_stone( "st-mirror-p/", i-1, line,{transparent=TRUE})

		elseif c == "G" then
			set_stone( "st-mirror-p\\", i-1, line,{transparent=TRUE})

		elseif c == "g" then
			set_stone( "st-mirror-p/", i-1, line,{transparent=TRUE})

		elseif c == "I" then
			set_stone( "st-mirror-p-", i-1, line,{transparent=TRUE})

		elseif c == "B" then
			set_stone( "st-laser-w", i-1, line, {name="laser1",on=TRUE,dir=WEST})

		elseif c == "h" then
			set_item( "it-hammer", i-1, line)

		elseif c == "b" then
			set_stone("st-switch", i-1, line, {action="onoff", target="laser1"})

		elseif c=="C" then
			set_stone( "st-door_c", i-1, line, {name="door2"})

		elseif c=="c" then
			set_stone("st-switch", i-1, line, {action="openclose", target="door2"})

		elseif c == "S" then
			set_stone( "st-shogun-s", i-1, line)

		elseif c == "M" then
			set_stone( "st-shogun-m", i-1, line)

		elseif c == "L" then
			set_stone( "st-shogun-l", i-1, line)

		elseif c == "d" then
			shogundot3(i-1, line, {action="openclose", target="door1"})

		elseif c == "D" then
			set_stone( "st-stone_break", i-1, line)

		elseif c == "$" then
			set_item( "it-coin1", i-1, line)

		elseif c== "e" then
			set_stone("st-coinslot", i-1, line, {action="openclose", target="door4"})

		elseif c== "T" then
			set_item("it-trigger", i-1, line, {action="onoff", target="laser1"})

		elseif c== "t" then
			set_item("it-trigger", i-1, line, {action="open", target="door1"})

		elseif c== "i" then
			set_stone("st-laserswitch", i-1, line, {action="openclose", target="door4"})

		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"m#########m#############m====B============")
renderLine(01,"#TTTT##z  #       tt  o =     !!=f      o=")
renderLine(02,"# STTTTTTT#     +       = ++  !!=   tt   =")
renderLine(03,"#  M ##   A             =  ++ !!=        =")
renderLine(04,"# L  ##a###     t t     = ++  !!======   =")
renderLine(05,"#    ##      t   ttt       + D!!!!!!!K t =")
renderLine(06,"#TTTT##                      D!!!!!!!k t =")
renderLine(07,"#    ##                $=     !!======   =")
renderLine(08,"# d  ## +  tttttttt     =     !!=        =")
renderLine(09,"#    ##                 =     !!=   tt   =")
renderLine(10,"#TTTT##       ttt       =     !!=        =")
renderLine(11,"#   h##ttt            o =     !!ie      o=")
renderLine(12,"##################E#####==================")
renderLine(13,"==================F=======================")
renderLine(14,"=o rttTtttttttttttt ttttttDDttt  tTttttDo=")
renderLine(15,"G  =       Ttttttt+tttttttDDt tt TtttttD y")
renderLine(16,"=o =ttt ttttttt tttttTttttDDtttt tt tTtDo=")
renderLine(17,"G=========================gg====I======g==")

oxyd_shuffle()

display.SetFollowMode(display.FOLLOW_SCROLLING)
















