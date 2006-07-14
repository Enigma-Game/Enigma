-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Triple-Maze

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="B" then
			set_stone( "st-black1", i-1, line)
		elseif c =="W" then
			set_stone( "st-white1", i-1, line)
		elseif c =="H" then
			set_stone( "st-grate3", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {name="a1",player=0})
		   set_item("it-yinyang",i-1,line)
		elseif c=="y" then
		   set_actor("ac-whiteball", i-.5,line+.5, {name="a3",player=1})
		   set_item("it-yinyang",i-1,line)
		elseif c=="h" then
		   set_actor("ac-horse", i-.5,line+.5, {name="a2",player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"# B   #       H  Wo#")
renderLine(02,"# # #   # ###### ###")
renderLine(03,"# #W#H###     B    #")
renderLine(04,"# # H B W #H#W#  # #")
renderLine(05,"# # #B###H# # H  # #")
renderLine(06,"# # # #   B # #BW# #")
renderLine(07,"#   #   ### # #  # #")
renderLine(08,"# #H##HB# W # #  #H#")
renderLine(09,"# #  #  # #B# H  H #")
renderLine(10,"# ## # ## # W ## ###")
renderLine(11,"# B  B  zhy # W  Bo#")
renderLine(12,"####################")

oxyd_shuffle()

a1=enigma.GetNamedObject("a1")
a2=enigma.GetNamedObject("a2")
a3=enigma.GetNamedObject("a3")

AddRubberBand(a1,a2,800,3)
AddRubberBand(a2,a3,800,3)


















