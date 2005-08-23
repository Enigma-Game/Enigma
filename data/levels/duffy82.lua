-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Flood Zone

levelw=21
levelh=13

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-wood", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="%" then
			set_stone( "st-greenbrown", i-1, line)
			set_floor("fl-normal",i-1,line)
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
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="-" then
			puzzle(i-1,line,PUZ_0101)
		elseif c=="i" then
			puzzle(i-1,line,PUZ_1010)
		elseif c=="4" then
			puzzle(i-1,line,PUZ_0100)
		elseif c=="6" then
			puzzle(i-1,line,PUZ_0001)
		elseif c=="j" then
			puzzle(i-1,line,PUZ_1001)
		elseif c=="/" then
			puzzle(i-1,line,PUZ_0110)
		elseif c=="X" then
			puzzle(i-1,line,PUZ_1111)
		elseif c=="m" then
		   set_item("it-magicwand",i-1,line)
		elseif c=="x" then
			set_floor("fl-normal",i-1,line)
		elseif c=="S" then
		   set_stone("st-switch",i-1,line,{action="callback",target="funcc1"})
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#                 S#")
renderLine(02,"#                  #")
renderLine(03,"#                  #")
renderLine(04,"# 6j  %%%%%%%%xxx  #")
renderLine(05,"#  -  %o+% x  xx%  #")
renderLine(06,"#z i  % +%     %% m#")
renderLine(07,"#  -  %o+     x%%  #")
renderLine(08,"# 4/  %%%%%%%%xxx  #")
renderLine(09,"#                  #")
renderLine(10,"#                  #")
renderLine(11,"#                 S#")
renderLine(12,"####################")

oxyd_shuffle()

ff1=0
function funcc1()
if ff1==0 then
   fill_floor("fl-water",7,5,2,3)
   fill_floor("fl-water",10,5,1,3)
   set_floor("fl-water",9,7)
   fill_floor("fl-water",11,6,1,2)
   fill_floor("fl-water",12,5,2,3)
   set_floor("fl-water",14,6)
ff1=1
end
end