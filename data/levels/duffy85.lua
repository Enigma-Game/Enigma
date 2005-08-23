-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Think Fast

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)

if not difficult then
   tspeed=40
else
   tspeed=60
end

fill_floor("fl-rock", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="%" then
			set_stone( "st-greenbrown", i-1, line)
		   set_floor("fl-water",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "O" then
			oxyd( i-1, line)
		   set_floor("fl-water",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
		   set_floor("fl-water",i-1,line)
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
		elseif c=="S" then
		   set_actor("ac-top", i-.5,line+.5, {name="t1",player=1, mouseforce=0, range=1000, force=tspeed})
		   set_floor("fl-water",i-1,line)
		elseif c=="T" then
		   set_actor("ac-top", i-.5,line+.5, {name="t2",player=1, mouseforce=0, range=1000, force=-tspeed})
		   set_floor("fl-water",i-1,line)
		elseif c=="A" then
		   set_stone("st-switch",i-1,line,{action="callback",target="funcc1"})

	     end
	end	
end

renderLine(00,"**O%##o##AA##o##%O**")
renderLine(01,"*~~~            ~~~*")
renderLine(02,"O~~S            ~~~O")
renderLine(03,"%~~~            ~~~%")
renderLine(04,"o                  o")
renderLine(05,"#                  #")
renderLine(06,"A         z        A")
renderLine(07,"#                  #")
renderLine(08,"o                  o")
renderLine(09,"%~~~            ~~~%")
renderLine(10,"O~~~            T~~O")
renderLine(11,"*~~~            ~~~*")
renderLine(12,"**O%##o##AA##o##%O**")

oxyd_shuffle()

t1=enigma.GetNamedObject("t1")
t2=enigma.GetNamedObject("t2")

ff1=0
function funcc1()
    if ff1==0 then
        enigma.SetAttrib(t1,"force",-tspeed)
        enigma.SetAttrib(t2,"force",tspeed)
        ff1=1
    elseif ff1==1 then
        enigma.SetAttrib(t1,"force",tspeed)
        enigma.SetAttrib(t2,"force",-tspeed)
        ff1=0
    end
end

display.SetFollowMode(display.FOLLOW_SCROLLING)
