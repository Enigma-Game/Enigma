-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Worse Nightmare

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.FrictionFactor=-0.5

fill_floor("fl-rough-blue", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-stone1", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c == "L" then
		   set_item("it-landmine",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
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
renderLine(01,"#oL    L oo   L L o#")
renderLine(02,"#  L L   L LL L    #")
renderLine(03,"#L L    L      L L #")
renderLine(04,"#     L  L  L L   L#")
renderLine(05,"#      L   L   L L #")
renderLine(06,"#o  z    L   LL L o#")
renderLine(07,"#     L  L LL     L#")
renderLine(08,"#         L  L LL  #")
renderLine(09,"# L  LL  L    L  L #")
renderLine(10,"# L    L  L L   L  #")
renderLine(11,"#o   L   oo   L  Lo#")
renderLine(12,"####################")

oxyd_shuffle()
