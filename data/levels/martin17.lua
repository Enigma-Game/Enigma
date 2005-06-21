--
-- A level for Enigma
--
-- Copyright (c) 2003 Martin Hawlisch
-- Licensed under the GPL version 2.

levelw = 39
levelh = 25

floor = "fl-wood"
stone = "st-glass"

create_world( levelw, levelh)

fill_floor("fl-water", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_floor( floor, i-1, line)
			set_stone( stone, i-1, line)
		elseif c == "o" then
			set_floor( floor, i-1, line)
			oxyd( i-1, line)
		elseif c == "=" then
			set_floor( floor, i-1, line)
		elseif c == "x" then
			set_floor( floor, i-1, line)
			set_stone( "st-wood", i-1, line)
		end
	end
end

-- Floor:   "="
-- Border:  "#"
-- Oxyd:    "o"

renderLine(  0, "                  o o##################")
renderLine(  1, " ======= x=x == ==x ====== x x x x x  #")
renderLine(  2, " =   = = === x===== =======######## x #")
renderLine(  3, " =   = = =          =  =x==#======#xxx#")
renderLine(  4, " =   = = =======    =  =x==#==#=xx#== #")
renderLine(  5, " =   = =       =    =#=##==#==#=x=#o# #")
renderLine(  6, " =   = = =======    =#==####==###=### #")
renderLine(  7, " =   = = =          =#=x==##===##=====#")
renderLine(  8, " =   = = =          o##=#=##===#===#==#")
renderLine(  9, " =   = = =  ==== x=== #=#==#===#===####")
renderLine( 10, " x   = = =  =   =   # x==#=#===#####   ")
renderLine( 11, "o=   = ===  =   =   # ===x=#           ")
renderLine( 12, "     =      =   x  #########           ")
renderLine( 13, "o x==========      ####====#           ")
renderLine( 14, " =              =  ##==x##=##          ")
renderLine( 15, " =              =  #==x=x==== #######  ")
renderLine( 16, " =              =  #===x=x=## #=====###")
renderLine( 17, " ===============x  ######==#  #=x==x==#")
renderLine( 18, "                     #==####  #x####==#")
renderLine( 19, "           ======    #=   ==  ====##==#")
renderLine( 20, "           = xxx     #    ====#=#==x=##")
renderLine( 21, "           = x===x= ====   ####=##x=x=#")
renderLine( 22, "           ====    #=##=#  #=x==x=x=x=#")
renderLine( 23, "                   #=##=#  #====#=====#")
renderLine( 24, "                    o  o   ############")

oxyd_shuffle()

doorh( 20,22, { name="door1"})
doorh( 20,23, { name="door2"})
doorv( 21,21, { name="door3"})
doorh( 23,22, { name="door4"})
doorh( 23,23, { name="door5"})

set_item( "it-trigger", 20,15, { action="openclose", target="door1"})
set_item( "it-trigger", 20,16, { action="openclose", target="door2"})
set_item( "it-trigger", 21,14, { action="openclose", target="door3"})
set_item( "it-trigger", 21,15, { action="openclose", target="door4"})
set_item( "it-trigger", 21,16, { action="openclose", target="door5"})

document( 23,18, "This level is much bigger! Press F3!")

set_actor("ac-blackball", 33.5, 19.5)
