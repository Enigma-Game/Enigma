-- Enigmastermind, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
fill_floor("fl-black", 0,0, level_width,level_height)
display.SetFollowMode(display.FOLLOW_SCROLLING)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone("st-glass",i-1,line)
		elseif c=="!" then
			document(i-1,line,"This switch is to validate your guess")
		elseif c==";" then
			document(i-1,line,"This switch is to reset the game")
		elseif c=="/" then
			document(i-1,line,"Each color appears at most once; each good color gives whites if not at the good place, blacks if it is also well placed")
		elseif c=="a" then	--blue 1
		   	set_floor("fl-water",i-1,line)
		elseif c=="b" then	--green 2
		   	set_floor("fl-leaves",i-1,line)
		elseif c=="c" then	--red 3
		   	set_floor("fl-red",i-1,line)
		elseif c=="d" then	--yellow 4
		   	set_floor("fl-sahara",i-1,line)
		elseif c=="e" then	--orange 5
		   	set_floor("fl-sand",i-1,line)
		elseif c=="f" then	--purple 6
		   	set_floor("fl-rough-red",i-1,line)
		elseif c=="A" then
			set_stone("st-switch",i-1,line,{action="callback",target="ca"})
		elseif c=="B" then
			set_stone("st-switch",i-1,line,{action="callback",target="cb"})
		elseif c=="C" then
			set_stone("st-switch",i-1,line,{action="callback",target="cc"})
		elseif c=="D" then
			set_stone("st-switch",i-1,line,{action="callback",target="cd"})
		elseif c=="E" then
			set_stone("st-switch",i-1,line,{action="callback",target="ce"})
		elseif c=="F" then
			set_stone("st-switch",i-1,line,{action="callback",target="cf"})
		elseif c=="G" then
			set_stone("st-switch",i-1,line,{action="callback",target="pg"})
		elseif c=="H" then
			set_stone("st-switch",i-1,line,{action="callback",target="ph"})
		elseif c=="I" then
			set_stone("st-switch",i-1,line,{action="callback",target="pi"})
		elseif c=="J" then
			set_stone("st-switch",i-1,line,{action="callback",target="pj"})
		elseif c=="*" then
		   	set_stone("st-metal",i-1,line)
		elseif c=="=" then
			doorh(i-1,line,{name="door1"})
		elseif c=="+" then
			set_stone("st-switch",i-1,line,{action="callback",target="play"})
		elseif c=="-" then
			set_stone("st-switch",i-1,line,{action="callback",target="reset"})
		elseif c=="o" then
			oxyd(i-1,line)
  		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {name="ac0"})
		end
	end	
end


-- functions to choose one color amongst the 6

function ca()
	current_color=1
	redraw_color()
end

function cb()
	current_color=2
	redraw_color()
end

function cc()
	current_color=3
	redraw_color()
end

function cd()
	current_color=4
	redraw_color()
end

function ce()
	current_color=5
	redraw_color()
end

function cf()
	current_color=6
	redraw_color()
end

-- functions to choose a position amongst 4 where to put the current color

function pg()
	current_position=1
	redraw_position()
end

function ph()
	current_position=2
	redraw_position()
end

function pi()
	current_position=3
	redraw_position()
end

function pj()
	current_position=4
	redraw_position()
end

-- display the next chosen color and the guess made by the player

function redraw_color()
 if current_game<6 then
	for i=1,6 do enigma.KillStone(18,1+i) end
	set_stone("st-glass",18,1+current_color)
 end
end

function redraw_position()
 if current_game<6 then
	if current_color==1 then
		set_floor("fl-water",1+2*current_game,2+current_position)
	elseif current_color==2 then
		set_floor("fl-leaves",1+2*current_game,2+current_position)
	elseif current_color==3 then
		set_floor("fl-red",1+2*current_game,2+current_position)
	elseif current_color==4 then
		set_floor("fl-sahara",1+2*current_game,2+current_position)
	elseif current_color==5 then
		set_floor("fl-sand",1+2*current_game,2+current_position)
	elseif current_color==6 then
		set_floor("fl-rough-red",1+2*current_game,2+current_position)
	end
	tcur[current_position]=current_color
 end
end

function draw_sol()
	for i=1,4 do
	if ttrue[i]==1 then
		set_floor("fl-water",18,8+i)
	elseif ttrue[i]==2 then
		set_floor("fl-leaves",18,8+i)
	elseif ttrue[i]==3 then
		set_floor("fl-red",18,8+i)
	elseif ttrue[i]==4 then
		set_floor("fl-sahara",18,8+i)
	elseif ttrue[i]==5 then
		set_floor("fl-sand",18,8+i)
	elseif ttrue[i]==6 then
		set_floor("fl-rough-red",18,8+i)
	end
	end
end

-- analyse the guess made by the player and give it a note

function play()
if current_game<6 then
	sb=0
	sw=0
	for i=1,4 do
		for j=1,4 do
			if tcur[i]==ttrue[j] then
				if i==j then
					sb=sb+1
					--sw=sw-1
				else
					sw=sw+1
				end
			end
		end
	end
	if sb==4 then
		SendMessage("door1","open")
	else
		if current_game==5 then
			draw_sol()
		end
	end
	for i=1,4 do
		if sb>0 then
			sb=sb-1
			set_stone("st-blackballs",1+2*current_game,7+i)
		elseif sw>0 then
			sw=sw-1
			set_stone("st-whiteballs",1+2*current_game,7+i)
		end
	end
	
end
current_game=current_game+1			
end


--               01234567890123456789
renderLine(00 , "o                  o")
renderLine(01 , "***************=****")
renderLine(02 , "#############    Aa#")
renderLine(03 , "# # # # # # G    Bb#")
renderLine(04 , "# # # # # # H /  Cc#")
renderLine(05 , "# # # # # # I    Dd#")  
renderLine(06 , "# # # # # # J    Ee#")
renderLine(07 , "#############    Ff#")
renderLine(08 , "# # # # # # #  z ###")
renderLine(09 , "# # # # # # #    # #")
renderLine(10 , "# # # # # # #    # #")
renderLine(11 , "# # # # # # #!  ;# #")
renderLine(12 , "#############+  -# #") 
--               01234567890123456789

function reset()

 for i=1,6 do
  for j=1,4 do
   set_floor("fl-black",-1+2*i,2+j)
   set_floor("fl-black",-1+2*i,7+j)
   enigma.KillStone(-1+2*i,7+j)
  end
 end
 for i=1,4 do
  set_floor("fl-black",18,8+i)
 end
 tcur={}
 ttrue={}
	
 ttrue[1]=random(6)
 for j=1,1 do
	r=random(6)
	if r==ttrue[1] then j=j-1 else ttrue[2]=r end
 end
 for j=1,1 do
	r=random(6)
	if (r==ttrue[1] or r==ttrue[2]) then j=j-1 else ttrue[3]=r end
 end
 for j=1,1 do
	r=random(6)
	if (r==ttrue[1] or r==ttrue[2] or r==ttrue[3]) then j=j-1 else ttrue[4]=r end
 end
 current_color=random(6)
 current_position=random(4)
 current_game=0
 redraw_color()
 redraw_position()
end

reset()
oxyd_shuffle()














