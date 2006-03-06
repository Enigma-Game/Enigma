-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Fool the Warden

levelw=58
levelh=37

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE
enigma.TwoPlayerGame = TRUE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
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
                    set_actor("ac-blackball", i-.5,line+.5)
--                    set_item("it-yinyang", i-1, line)
		elseif c=="y" then
                    set_actor("ac-whiteball", i-.5,line+.5)
--                    set_item("it-yinyang", i-1, line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="r" then
		   local ac0=	   set_actor("ac-rotor", i,line+.5, {attacknearest=0, range=100, force=100,name="ac0"})
			abyss(i-1,line)
		elseif c=="R" then
local ac1=		   set_actor("ac-killerball", i-.5,line+.5, {mouseforce=0, range=100, force=0,name="ac1"})
		elseif c=="Q" then
local ac2=		   set_actor("ac-killerball", i-.5,line+.5, {mouseforce=0, range=100, force=0,name="ac2"})
			abyss(i-1,line)
		elseif c == "b" then
			yy1( "black",  i-1, line)
		elseif c == "w" then
			yy1( "white",  i-1, line)
	     end
	end	
     end

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
end

renderLine(00,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
renderLine(01,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(02,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(03,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(04,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(05,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(06,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(07,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(08,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(09,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(10,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(11,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(12,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(13,"%!!!!!!!!!!!!!!!!!!!o! !       !    bo!!!!!!!!!!!!!!!!!!!%")
renderLine(14,"%!!!!!!!!!!!!!!!!!!! ! !!!!!!! ! !! yb!!!!!!!!!!!!!!!!!!!%")
renderLine(15,"%!!!!!!!!!!!!!!!!!!!   !     !   !    !!!!!!!!!!!!!!!!!!!%")
renderLine(16,"%!!!!!!!!!!!!!!!!!!!!! ! !!!!!!!Q! !! !!!!!!!!!!!!!!!!!!!%")
renderLine(17,"%!!!!!!!!!!!!!!!!!!!   ! !         !  !!!!!!!!!!!!!!!!!!!%")
renderLine(18,"%!!!!!!!!!!!!!!!!!!! !!! !!!r!!!!! ! !!!!!!!!!!!!!!!!!!!!%") 
renderLine(19,"%!!!!!!!!!!!!!!!!!!!     !         !  !!!!!!!!!!!!!!!!!!!%")
renderLine(20,"%!!!!!!!!!!!!!!!!!!!!!!!R! ! !!!! !!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(21,"%!!!!!!!!!!!!!!!!!!!       ! !    !   !!!!!!!!!!!!!!!!!!!%")
renderLine(22,"%!!!!!!!!!!!!!!!!!!!wz !!!!! !!!!!! ! !!!!!!!!!!!!!!!!!!!%")
renderLine(23,"%!!!!!!!!!!!!!!!!!!!ow !            !o!!!!!!!!!!!!!!!!!!!%")
renderLine(24,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(25,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(26,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(27,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(28,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(29,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(30,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(31,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(32,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(33,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(34,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(35,"%!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%")
renderLine(36,"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")

oxyd_shuffle()

ac0=enigma.GetNamedObject("ac0")
ac1=enigma.GetNamedObject("ac1")
ac2=enigma.GetNamedObject("ac2")

enigma.AddRubberBand(ac0,ac1,100,4)
--enigma.AddRubberBand(ac0,ac1,-100,4)
enigma.AddRubberBand(ac0,ac2,100,4)
--enigma.AddRubberBand(ac0,ac2,-100,4)
enigma.AddRubberBand(ac1,ac2,100,6)
enigma.AddRubberBand(ac1,ac2,-10,0)
















