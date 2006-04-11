-- Blet, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 20
levelh = 13

if difficult then 
	numax=12
elseif not difficult then
	numax=13
end

create_world(levelw, levelh)
oxyd_default_flavor = "a"
display.SetFollowMode(display.FOLLOW_SCROLLING)
fill_floor("fl-rough-blue", 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-glass1", i-1, line)
  		elseif c=="z" then
			set_actor("ac-blackball",i-0.5,line+0.5,{player=0})
		elseif c=="k" then
			set_stone("st-switch",i-1,line,{action="callback", target="turncoins"})
		elseif c=="V" then
		   	mirrorp(i-1,line,FALSE,FALSE, 2)
		elseif c=="/" then
		   	mirrorp(i-1,line,FALSE,FALSE, 4)
		elseif c=="<" then
		   	mirror3(i-1,line,FALSE,FALSE, 2)
		elseif c=="-" then
			set_attrib(laser(i-1,line,FALSE, EAST), "name", "las1")
		elseif c=="d" then
			document(i-1,line,"When you change the color of one square, the colors of the 2 nearest squares change as well; you must get as much green squares as possible")
		elseif c=="A" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch1"})
		elseif c=="B" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch2"})
		elseif c=="C" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch3"})
		elseif c=="D" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch4"})
		elseif c=="E" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch5"})
		elseif c=="F" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch6"})
		elseif c=="G" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch7"})
		elseif c=="H" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch8"})
		elseif c=="I" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch9"})
		elseif c=="J" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch10"})
		elseif c=="K" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch11"})
		elseif c=="L" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch12"})
		elseif c=="M" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch13"})
		elseif c=="N" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch14"})
		elseif c=="O" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch15"})
		elseif c=="P" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch16"})
		elseif c=="o" then
			oxyd(i-1,line)
		end
	end	
end

-- display function -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

function display()
	if tab[1]==1 then set_floor("fl-leaves",12,4) else set_floor("fl-red",12,4) end
	if tab[2]==1 then set_floor("fl-leaves",11,2) else set_floor("fl-red",11,2) end
	if tab[3]==1 then set_floor("fl-leaves",10,1) else set_floor("fl-red",10,1) end
	if tab[4]==1 then set_floor("fl-leaves",8,0) else set_floor("fl-red",8,0) end
	if tab[5]==1 then set_floor("fl-leaves",5,0) else set_floor("fl-red",5,0) end
	if tab[6]==1 then set_floor("fl-leaves",3,1) else set_floor("fl-red",3,1) end
	if tab[7]==1 then set_floor("fl-leaves",2,2) else set_floor("fl-red",2,2) end
	if tab[8]==1 then set_floor("fl-leaves",1,4) else set_floor("fl-red",1,4) end
	if tab[9]==1 then set_floor("fl-leaves",1,7) else set_floor("fl-red",1,7) end
	if tab[10]==1 then set_floor("fl-leaves",2,9) else set_floor("fl-red",2,9) end
	if tab[11]==1 then set_floor("fl-leaves",3,10) else set_floor("fl-red",3,10) end
	if tab[12]==1 then set_floor("fl-leaves",5,11) else set_floor("fl-red",5,11) end
	if tab[13]==1 then set_floor("fl-leaves",8,11) else set_floor("fl-red",8,11) end
	if tab[14]==1 then set_floor("fl-leaves",10,10) else set_floor("fl-red",10,10) end
	if tab[15]==1 then set_floor("fl-leaves",11,9) else set_floor("fl-red",11,9) end
	if tab[16]==1 then set_floor("fl-leaves",12,7) else set_floor("fl-red",12,7) end
	num=0
	for n=1,16 do if tab[n]==1 then num=num+1 end end
	if num>= numax then SendMessage("las1","on") end
end

-- switch functions -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

function switch1()
	tab[16]=1-tab[16]
	tab[1]=1-tab[1]
	tab[2]=1-tab[2]
	display()
end

function switch2()
	tab[1]=1-tab[1]
	tab[2]=1-tab[2]
	tab[3]=1-tab[3]
	display()
end

function switch3()
	tab[2]=1-tab[2]
	tab[3]=1-tab[3]
	tab[4]=1-tab[4]
	display()
end

function switch4()
	tab[3]=1-tab[3]
	tab[4]=1-tab[4]
	tab[5]=1-tab[5]
	display()
end

function switch5()
	tab[4]=1-tab[4]
	tab[5]=1-tab[5]
	tab[6]=1-tab[6]
	display()
end

function switch6()
	tab[5]=1-tab[5]
	tab[6]=1-tab[6]
	tab[7]=1-tab[7]
	display()
end

function switch7()
	tab[6]=1-tab[6]
	tab[7]=1-tab[7]
	tab[8]=1-tab[8]
	display()
end

function switch8()
	tab[7]=1-tab[7]
	tab[8]=1-tab[8]
	tab[9]=1-tab[9]
	display()
end

function switch9()
	tab[8]=1-tab[8]
	tab[9]=1-tab[9]
	tab[10]=1-tab[10]
	display()
end

function switch10()
	tab[9]=1-tab[9]
	tab[10]=1-tab[10]
	tab[11]=1-tab[11]
	display()
end

function switch11()
	tab[10]=1-tab[10]
	tab[11]=1-tab[11]
	tab[12]=1-tab[12]
	display()
end

function switch12()
	tab[11]=1-tab[11]
	tab[12]=1-tab[12]
	tab[13]=1-tab[13]
	display()
end

function switch13()
	tab[12]=1-tab[12]
	tab[13]=1-tab[13]
	tab[14]=1-tab[14]
	display()
end

function switch14()
	tab[13]=1-tab[13]
	tab[14]=1-tab[14]
	tab[15]=1-tab[15]
	display()
end

function switch15()
	tab[14]=1-tab[14]
	tab[15]=1-tab[15]
	tab[16]=1-tab[16]
	display()
end

function switch16()
	tab[15]=1-tab[15]
	tab[16]=1-tab[16]
	tab[1]=1-tab[1]
	display()
end

function reset()
	for i=0,7 do
		tab[2*i+1]=0
		tab[2*i+2]=1
	end
	display()
end

--               01234567890123456789
renderLine(00 , "               /V/Vo")
renderLine(01 , "                    ")
renderLine(02 , "      ED            ")
renderLine(03 , "     F  C           ")
renderLine(04 , "    G    B          ")  
renderLine(05 , "   H  z   A     V/V/")
renderLine(06 , "   I   d  P   -<    ")
renderLine(07 , "    J    O      /V/V")
renderLine(08 , "     K  N           ")
renderLine(09 , "      LM            ")
renderLine(10 , "                    ")
renderLine(11 , "                    ")
renderLine(12 , "               V/V/o")
--               01234567890123456789

tab={}
reset()
oxyd_shuffle()


