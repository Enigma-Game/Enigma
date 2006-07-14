-- A meditation level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 20
levelh = 13

create_world(levelw, levelh)
fill_floor("fl-leaves", 0,0, level_width,level_height)
display.SetFollowMode(display.FOLLOW_SCROLLING)
enigma.BumperForce=200


function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone("st-rock5", i-1, line)
		elseif c=="h" then
		   	set_item("it-hollow",i-1,line)
		elseif c=="a" then
			doorh(i-1,line,{name="door1"})
		elseif c=="b" then
			doorh(i-1,line,{name="door2"})
		elseif c=="c" then
			doorh(i-1,line,{name="door3"})
		elseif c=="d" then
			doorv(i-1,line,{name="door4"})
		elseif c=="1" then
			set_stone("st-switch", i-1,line, {action="openclose", target="door1"})
		elseif c=="2" then
			set_stone("st-switch", i-1,line, {action="openclose", target="door2"})
		elseif c=="3" then
			set_stone("st-switch", i-1,line, {action="openclose", target="door3"})
		elseif c=="4" then
			set_stone("st-switch", i-1,line, {action="openclose", target="door4"})
		elseif c=="+" then
			set_stone("st-actorimpulse", i-1,line)
		elseif c=="q" then
			set_actor("ac-top",i-0.5,line+0.5,{range=40,force=20,name="top1",gohome=FALSE})
		elseif c=="r" then
			set_actor("ac-top",i-0.5,line+0.5,{range=40,force=20,name="top2",gohome=FALSE})
		elseif c=="s" then
			set_actor("ac-top",i-0.5,line+0.5,{range=40,force=20,name="top3",gohome=FALSE})
		elseif c=="t" then
			set_actor("ac-top",i-0.5,line+0.5,{range=40,force=20,name="top4",gohome=FALSE})
		elseif c=="z" then
            	set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
		end
	end	
end

--               01234567890123456789
renderLine(00 , "h                  h")
renderLine(01 , " ################a# ")
renderLine(02 , " #z            z# # ")
renderLine(03 , " # ++++++++++++ # # ")
renderLine(04 , " # +q        r+ #b# ")
renderLine(05 , " # + 1 2  3 4 + # # ")  
renderLine(06 , " # +          + # # ")
renderLine(07 , " # + 4 3  2 1 + #c# ")
renderLine(08 , " # +s        t+ # # ")
renderLine(09 , " # ++++++++++++ # # ")
renderLine(10 , " #z            zd # ")
renderLine(11 , " ################## ")
renderLine(12 , "h                  h") 
--               01234567890123456789


oxyd_shuffle()














