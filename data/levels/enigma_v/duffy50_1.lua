-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Take Turns

levelw=39
levelh=13

create_world( levelw, levelh)
enigma.TwoPlayerGame = TRUE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
			abyss(i-1,line)
		elseif c =="%" then
			set_stone( "st-invisible", i-1, line)
			abyss(i-1,line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
                elseif c == "~" then
--			abyss(i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
local ac0=           set_actor("ac-blackball", i-.5,line+.5, {name="ac0"})
--			set_item("it-yinyang", i-1, line)
		elseif c=="y" then
local ac1=		set_actor("ac-whiteball", i-.5,line+.5, {name="ac1"})
--                       set_item("it-yinyang", i-1, line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="r" then
		   set_actor("ac-rotor", i-.5,line+.5, {mouseforce=0, range=5, force=3})
		elseif c == "b" then
			yy1( "black",  i-1, line)
		elseif c == "w" then
			yy1( "white",  i-1, line)
		elseif c == "B" then
		   	set_item("it-flagblack", i-1, line)
		elseif c == "W" then
		   	set_item("it-flagblack", i-1, line)
	     end
	end	
     end

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
end

renderLine(00,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
renderLine(01,"!!!!!!!!!!!!!!     !         !o!!oo!!o!")
renderLine(02,"!!          !!     ! #!!#!!# !b!!ww!!b!")
renderLine(03,"!! z        !!  !#   !   !   !        !")
renderLine(04,"!!          !!  !!  #! ! ! ! !        !")
renderLine(05,"!!###!!!!!!     !!!    !   !          !")
renderLine(06,"!!!!!!!!!!!#!!!#!!!#!!#!#!#!#!       r!")
renderLine(07,"!!###!!!!!!     !!!    !   !          !")
renderLine(08,"!!          !!  !!  #! ! ! ! !        !")
renderLine(09,"!! y        !!  !#   !   !   !        !")
renderLine(10,"!!          !!     ! #!!#!!# !b!!ww!!b!")
renderLine(11,"!!!!!!!!!!!!!!     !         !o!!oo!!o!")
renderLine(12,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

oxyd_shuffle()

ac0=enigma.GetNamedObject("ac0")
ac1=enigma.GetNamedObject("ac1")

enigma.AddRubberBand(ac0,ac1,1,0)


















