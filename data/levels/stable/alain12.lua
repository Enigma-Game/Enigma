-- Cluenigma, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
fill_floor("fl-sahara", 0,0, level_width,level_height)
draw_border("st-marble")

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="G" then
			set_stone("st-glass", i-1, line)
		elseif c=="a" then	--peacock 1
		   	set_floor("fl-water",i-1,line)
			set_stone("st-glass",i-1,line)
		elseif c=="b" then	--mustard 2
		   	set_floor("fl-sand",i-1,line)
			set_stone("st-glass",i-1,line)
		elseif c=="c" then	--plum 3
		   	set_floor("fl-rough-red",i-1,line)
			set_stone("st-glass",i-1,line)
		elseif c=="d" then	--green 4
		   	set_floor("fl-leaves",i-1,line)
			set_stone("st-glass",i-1,line)
		elseif c=="e" then	--white 5
		   	set_floor("fl-white",i-1,line)
			set_stone("st-glass",i-1,line)
		elseif c=="f" then	--scarlett 6
		   	set_floor("fl-red",i-1,line)
			set_stone("st-glass",i-1,line)
		elseif c=="A" then
			set_stone("st-switch",i-1,line,{action="callback",target="pa"})
		elseif c=="B" then
			set_stone("st-switch",i-1,line,{action="callback",target="pb"})
		elseif c=="C" then
			set_stone("st-switch",i-1,line,{action="callback",target="pc"})
		elseif c=="D" then
			set_stone("st-switch",i-1,line,{action="callback",target="pd"})
		elseif c=="E" then
			set_stone("st-switch",i-1,line,{action="callback",target="pe"})
		elseif c=="F" then
			set_stone("st-switch",i-1,line,{action="callback",target="pf"})
		elseif c=="h" then	--bomb 1
		   	set_item("it-blackbomb",i-1,line)
		elseif c=="i" then	--dagger 2
		   	set_item("it-sword",i-1,line)
		elseif c=="j" then	--poison 3
		   	set_item("it-booze",i-1,line)
		elseif c=="k" then	--wrench 4
		   	set_item("it-wrench",i-1,line)
		elseif c=="l" then	--candlestick 5
		   	set_item("it-dynamite",i-1,line)
		elseif c=="m" then	--hammer 6
		   	set_item("it-hammer",i-1,line)
		elseif c=="H" then
			set_stone("st-switch",i-1,line,{action="callback",target="wa"})
		elseif c=="I" then
			set_stone("st-switch",i-1,line,{action="callback",target="wb"})
		elseif c=="J" then
			set_stone("st-switch",i-1,line,{action="callback",target="wc"})
		elseif c=="K" then
			set_stone("st-switch",i-1,line,{action="callback",target="wd"})
		elseif c=="L" then
			set_stone("st-switch",i-1,line,{action="callback",target="we"})
		elseif c=="M" then
			set_stone("st-switch",i-1,line,{action="callback",target="wf"})
		elseif c=="P" then
			set_stone("st-switch",i-1,line,{action="callback",target="ra"})
		elseif c=="Q" then
			set_stone("st-switch",i-1,line,{action="callback",target="rb"})
		elseif c=="R" then
			set_stone("st-switch",i-1,line,{action="callback",target="rc"})
		elseif c=="S" then
			set_stone("st-switch",i-1,line,{action="callback",target="rd"})
		elseif c=="T" then
			set_stone("st-switch",i-1,line,{action="callback",target="re"})
		elseif c=="U" then
			set_stone("st-switch",i-1,line,{action="callback",target="rf"})
		elseif c=="w" then
			set_stone("st-marble",i-1,line)
		elseif c=="x" then
			set_stone("st-switch",i-1,line,{action="callback",target="submit"})
		elseif c=="X" then
			doorh(i-1,line,{name="door1"})
		elseif c=="y" then
			document(i-1,line,"Your uncle Sir Blackball has been murdered last night in his manor, in one of his 6 rooms, by one of the weapons which are in the cellar; the suspects are the shy Mrs Peacock, the secret Colonel Mustard, the absent-minded Professor Plum, the mysterious Rev. Green, the nurse Mrs Whiteball, who discovered the corpse, and Miss Scarlett whose beauty might be fatal...; you must find who killed your uncle, where and how")
		elseif c == "o" then
			oxyd( i-1, line)
  		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {name="ac0"})
		end
	end	
end

function pa()
	murderer=1
	redraw_person()
end

function pb()
	murderer=2
	redraw_person()
end

function pe()
	murderer=5
	redraw_person()
end

function pf()
	murderer=6
	redraw_person()
end

function pc()
	murderer=3
	redraw_person()
end

function pd()
	murderer=4
	redraw_person()
end

function redraw_person()
	for i=1,6 do
		set_stone("st-glass",1,i+4)
	end
	set_stone("st-white3",1,murderer+4)
end

function wa()
	weapon=1
	redraw_weapon()
end

function wb()
	weapon=2
	redraw_weapon()
end

function wc()
	weapon=3
	redraw_weapon()
end

function wd()
	weapon=4
	redraw_weapon()
end

function we()
	weapon=5
	redraw_weapon()
end

function wf()
	weapon=6
	redraw_weapon()
end

function redraw_weapon()
	for i=1,6 do
		set_stone("st-glass",6,i+4)
	end
	set_stone("st-white3",6,weapon+4)
end

function ra()
	room=1
	redraw_room()
end

function rb()
	room=2
	redraw_room()
end

function rc()
	room=3
	redraw_room()
end

function rd()
	room=4
	redraw_room()
end

function re()
	room=5
	redraw_room()
end

function rf()
	room=6
	redraw_room()
end

function redraw_room()
	for i=1,2 do
		for j=1,3 do
			enigma.KillStone(13+2*i,4+2*j)
		end
	end
	if room==1 then
		set_stone("st-white3",15,6)
	elseif room==2 then
		set_stone("st-white3",15,8)
	elseif room==3 then
		set_stone("st-white3",15,10)
	elseif room==4 then
		set_stone("st-white3",17,6)
	elseif room==5 then
		set_stone("st-white3",17,8)
	elseif room==6 then
		set_stone("st-white3",17,10)
	end
end

function care_person()
	p11="The murderer is not "
	if murderer==1 then
		p12="Mrs Peacock"
	elseif murderer==2 then
		p12="Colonel Mustard"
	elseif murderer==3 then
		p12="Professor Plum"
	elseif murderer==4 then
		p12="Reverend Green"
	elseif murderer==5 then
		p12="Mrs Whiteball"
	elseif murderer==6 then
		p12="Miss Scarlett"

	end
	p1=p11..p12
	document(11,5,p1)
end
function care_weapon()
	w11="Sir Blackball has not been killed with "
	if weapon==1 then
		w12="the bomb"
	elseif weapon==2 then
		w12="the dagger"
	elseif weapon==3 then
		w12="poison"
	elseif weapon==4 then
		w12="the wrench"
	elseif weapon==5 then
		w12="the candlestick"
	elseif weapon==6 then
		w12="the hammer"

	end
	w1=w11..w12
	document(11,5,w1)
end

function care_room()
	document(11,5,"This room is not the crime scene")
end

function submit()
	rg=random(6)
	if rg==1 then
		if murderer~=true_murderer then
			care_person()
		elseif weapon~=true_weapon then
			care_weapon()
		elseif room~=true_room then
			care_room()
		else
			SendMessage("door1","open")
		end
	elseif rg==2 then
		if weapon~=true_weapon then
			care_weapon()
		elseif murderer~=true_murderer then
			care_person()
		elseif room~=true_room then
			care_room()
		else
			SendMessage("door1","open")
		end
	elseif rg==3 then
		if murderer~=true_murderer then
			care_person()
		elseif room~=true_room then
			care_room()
		elseif weapon~=true_weapon then
			care_weapon()
		else
			SendMessage("door1","open")
		end
	elseif rg==4 then
		if weapon~=true_weapon then
			care_weapon()
		elseif room~=true_room then
			care_room()
		elseif murderer~=true_murderer then
			care_person()
		else
			SendMessage("door1","open")
		end
	elseif rg==5 then
		if room~=true_room then
			care_room()
		elseif murderer~=true_murderer then
			care_person()
		elseif weapon~=true_weapon then
			care_weapon()
		else
			SendMessage("door1","open")
		end
	elseif rg==6 then
		if room~=true_room then
			care_room()
		elseif weapon~=true_weapon then
			care_weapon()
		elseif murderer~=true_murderer then
			care_person()
		else
			SendMessage("door1","open")
		end
	end
end


--remark: the probability that the first random guess is the good one is 1 over 216

--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , " o                o ")
renderLine(02 , " wwwwwwwwXwwwwwwwww ")
renderLine(03 , "             PQRSTU ")
renderLine(04 , " GGG  GGG           ")
renderLine(05 , " GaA  GhH  y  wwwww ")  
renderLine(06 , " GbB  GiI     w w w ")
renderLine(07 , " GcC  GjJ  z  wwwww ")
renderLine(08 , " GdD  GkK     w w w ")
renderLine(09 , " GeE  GlL     wwwww ")
renderLine(10 , " GfF  GmM     w w w ")
renderLine(11 , " GGG  GGG  x  wwwww ")
renderLine(12 , "                    ") 
--               01234567890123456789


true_murderer=random(6)
true_weapon=random(6)
true_room=random(6)
murderer=random(6)
weapon=random(6)
room=random(6)
redraw_person()
redraw_weapon()
redraw_room()

oxyd_shuffle()













