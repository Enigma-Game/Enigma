-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Snowing

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)+1
levelh=1+(12*rooms_high)

create_world( levelw, levelh)

difficult = (options.Difficulty==2)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c=="t" then
if difficult==false then
		   set_actor("ac-top", i-.5,line+.5, {force=25,range=30,controllers=0,player=1})
else
		   set_actor("ac-top", i-.5,line+.5, {force=40,range=30,controllers=0,player=1})
end
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="T" then
		   set_stone("st-timer",i-1,line,{loop=1,interval=0.2,action="callback",target="funcc1"})
	     end
	end	
end

renderLine(00,"####################T")
renderLine(01,"#       o  o       # ")
renderLine(02,"#    o        o    # ")
renderLine(03,"#  o            o  # ")
renderLine(04,"#                  # ")
renderLine(05,"#o                o# ")
renderLine(06,"#    t   ##  z     # ")
renderLine(07,"#o                o# ")
renderLine(08,"#                  # ")
renderLine(09,"#  o            o  # ")
renderLine(10,"#    o        o    # ")
renderLine(11,"#       o  o       # ")
renderLine(12,"#################### ")

oxyd_shuffle()

function funcc1()
   x=random(21)-1
   y=random(13)-1
   set_floor("fl-ice_001",x,y)
end















