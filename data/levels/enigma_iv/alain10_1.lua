-- Sierpinski, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 150
levelh = 100

create_world(levelw, levelh)
oxyd_default_flavor = "a"
fill_floor("fl-space", 0,0, level_width,level_height)

dir=0
x1=4
y1=46


function av2()
	set_floor("fl-rough-blue",x1,y1)
	if dir==0 then
		x2=x1+1
		y2=y1
	elseif dir==1 then
		x2=x1
		y2=y1-1
	elseif dir==2 then
		x2=x1-1
		y2=y1
	elseif dir==3 then
		x2=x1
		y2=y1+1
	end
	x1=x2
	y1=y2
end

function av1()
	av2()
	av2()
end

function sierp(lev)
	if lev==1 then
		av1()
		dir=dir+1
		if dir==4 then dir=0 end
		av1()
		dir=dir-1
		if dir==-1 then dir=3 end
		av1()
		dir=dir-1
		if dir==-1 then dir=3 end
		av1()
		av1()
		dir=dir+1
		if dir==4 then dir=0 end
		av1()
		dir=dir+1
		if dir==4 then dir=0 end
		av1()
		dir=dir-1
		if dir==-1 then dir=3 end
		av1()
	else
		sierp(lev-1)
		dir=dir+1
		if dir==4 then dir=0 end
		sierp(lev-1)
		dir=dir-1
		if dir==-1 then dir=3 end
		sierp(lev-1)
		dir=dir-1
		if dir==-1 then dir=3 end
		sierp(lev-1)
		sierp(lev-1)
		dir=dir+1
		if dir==4 then dir=0 end
		sierp(lev-1)
		dir=dir+1
		if dir==4 then dir=0 end
		sierp(lev-1)
		dir=dir-1
		if dir==-1 then dir=3 end
		sierp(lev-1)
	end
end

oxyd(x1,y1)
sierp(3)
oxyd(x2,y2)
set_actor("ac-blackball", 75-.5,54+.5, {player=0})


oxyd_shuffle()

display.SetFollowMode(display.FOLLOW_SCROLLING)

















