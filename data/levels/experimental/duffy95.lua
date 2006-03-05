-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Automaton

levelw=21
levelh=13

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="w" then
			set_stone( "st-glass", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="C" then
		   	set_stone( "st-rotator-right", i-1, line)
		elseif c=="c" then
		   	set_stone( "st-rotator-left", i-1, line)
		elseif c=="H" then
		   	set_stone( "st-blocker", i-1, line)
 		elseif c == "N" then
		   set_stone("st-bolder", i-1,line, {name="bolder1",direction=NORTH})
 		elseif c == "E" then
		   set_stone("st-bolder", i-1,line, {name="bolder1",direction=EAST})
 		elseif c == "S" then
		   set_stone("st-bolder", i-1,line, {name="bolder1",direction=SOUTH})
 		elseif c == "W" then
		   set_stone("st-bolder", i-1,line, {name="bolder1",direction=WEST})
		   set_floor("fl-water",i-1,line)
 		elseif c == "A" then
		   set_stone("st-coinslot",i-1,line,{action="onoff", target="laser1"})
 		elseif c == "I" then
			set_item("it-trigger", i-1,line, {action="callback", target="funcc2"})
 		elseif c == "B" then
			doorv( i-1,line, {name="door1"})
 		elseif c == "X" then
			set_attrib(laser(i-1,line, FALSE, WEST), "name", "laser1")
 		elseif c == "b" then
		   set_stone("st-break_bolder",i-1,line)
 		elseif c == "l" then
		   set_stone("st-break_acwhite",i-1,line)
 		elseif c == ";" then
			mirrorp(i-1,line,FALSE,FALSE, 2)
 		elseif c == "/" then
			mirrorp(i-1,line,FALSE,FALSE, 4)
 		elseif c == ":" then
			mirrorp(i-1,line,FALSE,TRUE, 2)
 		elseif c == "?" then
			mirrorp(i-1,line,FALSE,TRUE, 4)
 		elseif c == "s" then
		   set_stone("st-laserswitch",i-1,line,{action="callback",target="funcc1"})
		elseif c == "t" then
			set_item("it-coin2", i-1,line)
		elseif c == "T" then
		   set_stone("st-switch",i-1,line,{action="callback",target="funcc2"})
		elseif c == "R" then
                    set_stone("st-oneway", i-1,line, {orientation=enigma.EAST})
	     end
	end	
end

renderLine(00,"##############c###s##")
renderLine(01,"#  B   / Cxxxxx /w:X#")
renderLine(02,"#oo#;     x ? x ?####")
renderLine(03,"####?    ?x cxxxxxlW#")
renderLine(04,"#C   : :  x  xx;  lc#")
renderLine(05,"#xxxxxccxxxxxxx  Cl##")
renderLine(06,"#x  /x  x x ?Cxxxx ##")
renderLine(07,"#x cxxxxx;x /  : x?##")
renderLine(08,"#x  xx  C?xz  ? /x ##")
renderLine(09,"cxxxxxxxxxxxxxxxxxc##")
renderLine(10,"#   cC####H####   :##")
renderLine(11,"#    R      tA#/  /##")
renderLine(12,"##########!##########")

ff1=0
function funcc1()
   if ff1==0 then
      SendMessage("door1","open")
      ff1=1
   end
end

















