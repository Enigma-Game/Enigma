-- Awale, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 105
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
display.SetFollowMode(display.FOLLOW_SCROLLING)
fill_floor("fl-hay", 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-glass1", i-1, line)
		elseif c=="D" then
			set_floor("fl-leaves",i-1,line)
			set_stone("st-glass1",i-1,line)
  		elseif c=="z" then
--			set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
			set_actor("ac-blackball",i-0.5,line+0.5,{player=0})
		elseif c=="k" then
			set_stone("st-switch",i-1,line,{action="callback", target="okokok"})
		elseif c=="r" then
			set_stone("st-switch",i-1,line,{action="callback", target="reset"})
		elseif c=="a" then
			document(i-1,line,"The witch of this switch waits for your wish")
		elseif c=="b" then
			document(i-1,line,"The witch of this switch resets the awale game")
		elseif c=="c" then
			document(i-1,line,"A turn consists in taking the walnuts of a row behind the windowy walls and sowing them 1 by 1 in the following rows")
		elseif c=="d" then
			document(i-1,line,"Warning: What awaits the wailing warrior at the end of a turn ? If there are 2 or 3 walnuts in a row, they are a warm award for the warrior, as are the walnuts in the rows which are in the way to this row, if they are 2 or 3 per row")
		elseif c==";" then
			document(i-1,line,"Warning: If one warrior's walls are empty, this awesome warrior wins")
		elseif c=="e" then
			document(i-1,line,"When a warrior has awarded more than half the walnuts, he wins. What does he win ? The way to the waiting oxyds he wants")
		elseif c=="*" then
			set_stone("st-stone2",i-1,line)
		elseif c=="-" then
			doorh(i-1,line,{name="sesame"})
		elseif c=="1" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch1"})
		elseif c=="2" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch2"})
		elseif c=="3" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch3"})
		elseif c=="4" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch4"})
		elseif c=="5" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch5"})
		elseif c=="6" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch6"})
		elseif c=="7" then
			set_stone("st-switch",i-1,line,{action="callback", target="switch7"})
		elseif c=="0" then
			set_floor("fl-dunes",i-1,line)
		elseif c=="+" then
--			set_item("it-hollow", i-1, line)
			oxyd(i-1,line)
		end
	end	
end

-- choosing a row -- -- -- -- -- -- -- -- -- -- -- -- -- --

function switch1()
	my_choice=1
	display_choice()
end

function switch2()
	my_choice=2
	display_choice()
end

function switch3()
	my_choice=3
	display_choice()
end

function switch4()
	my_choice=4
	display_choice()
end

function switch5()
	my_choice=5
	display_choice()
end

function switch6()
	my_choice=6
	display_choice()
end


-- display functions -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

function display_game()
	for i=1,6 do
		for j=1,48 do
			enigma.KillStone(50-j,i)
			enigma.KillStone(54+j,7-i)
		end
		num=tab[i]
		if num>=0 then
			for j=1,num do
				set_stone("st-rock3",50-j,i)
			end
		end
		num=tab[i+6]
		if num>=0 then
			for j=1,num do
				set_stone("st-rock3",54+j,7-i)
			end
		end
	end
	for i=1,48 do
		enigma.KillStone(50-i,10)
		enigma.KillStone(54+i,10)
	end
	if gain1>0 then
		for i=1,gain1 do
			set_stone("st-rock3",50-i,10)
		end
	end
	if gain2>0 then
		for i=1,gain2 do
			set_stone("st-rock3",54+i,10)
		end
	end
end

function display_choice()
	for i=1,6 do
		set_floor("fl-hay",50,i)
		set_floor("fl-hay",54,7-i)
--		enigma.KillStone(50,i)
--		enigma.KillStone(54,7-i)
--		set_stone("st-glass1",50,i)
--		set_stone("st-glass1",54,13-i)
	end
	set_floor("fl-water",50,my_choice)
	set_floor("fl-red",54,13-BB_choice)
--	set_stone("st-glass3",50,my_choice)
--	set_stone("st-glass3",54,7-BB_choice)
end

-- sowing the stones -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

function okokok()
	som=tab[my_choice]
	if som>0 then
		tab[my_choice]=0
		j=my_choice
		for i=1,som do
			j=j+1
			if j==13 then j=1 end
			tab[j]=tab[j]+1
		end
		take1(j)
		display_game()
		if hurra1==1 then
			for i=1,1 do -- Now Big Brother enters in action -- -- -- -- -- -- -- -- -- --
				BB_choice=random(6)+6
				som=tab[BB_choice]
				if som==0 then
					i=i-1
				end
			end
		else
			bouh=1
		end
		for i=7,12 do -- Big Brother tries to avoid a danger -- -- -- -- -- -- -- -- --
			if tab[i]==1 or tab[i]==2 then
				for j=1,6 do
					if tab[i]==j-i then
						BB_choice=i
					end
				end
			end
		end
		for i=1,6 do -- but Big Brother prefers to attack than defend himself -- -- -- --
			if tab[i]==1 or tab[i]==2 then
				for j=7,12 do
					if tab[j]==12+i-j then
						BB_choice=j
					end
				end
			end
		end
		som=tab[BB_choice]
		tab[BB_choice]=0
		j=BB_choice
		for i=1,som do
			j=j+1
			if j==13 then j=1 end
			tab[j]=tab[j]+1
		end
		take2(j)
		display_game()
		dammit0=0
		for i=1,6 do
			if tab[i]>0 then dammit0=1 end
		end
		if dammit1==0 or gain1>=24 then
			won=1
		end
		dammit1=dammit0
		if won==1 then
			document(52,9,"Congratulations ! You are the most aware awale player of the valley...")
			SendMessage("sesame","open")
		end
		hurra0=0
		for i=1,6 do
			if tab[6+i]>0 then hurra0=1 end
		end
		if gain2>=24 then
			bouh=1
		end
		hurra1=hurra0
		if bouh==1 then
			document(52,9,"Lost in the savannah ? Don't wait and walk on the way to the reset switch on the far left ...")
		end
	end
end

function take2(case)
	if case<7 then
		compt1=tab[case]
		if compt1==2 or compt1==3 then
			gain2=gain2+compt1
			tab[case]=0
			if case>1 then
				take2(case-1)
			end
		end
	end
end

function take1(case)
	if case>6 then
		compt2=tab[case]
		if compt2==2 or compt2==3 then
			gain1=gain1+compt2
			tab[case]=0
			if case>7 then
				take1(case-1)
			end
		end
	end
end


function reset()
	gain1=0
	gain2=0
	dammit1=1
	hurra1=1
	won=0
	bouh=0
	for i=1,12 do
		tab[i]=4
	end
	display_game()
	my_choice=random(6)
	BB_choice=6+random(6)
end


--               012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234
renderLine(00 , "r##################################################   ################################################## ")
renderLine(01 , " #   000    00000   0000      0    000     000    #1  #    000    0   0    000    0       00000   0    # ")
renderLine(02 , "b#  0   0   0       0   0     0   0   0   0   0   #2  #   0   0   0   0   0   0   0       0       0    # ")
renderLine(03 , " #  0   0   0       0   0     0   0       0   0   #3  #   0   0   0   0   0   0   0       000     0    # ")
renderLine(04 , " #  00000   0000    0000      0   0       00000   #4  #   00000   0   0   00000   0       0       0    # ")
renderLine(05 , " #  0   0   0       0   0     0   0   0   0   0   #5  #   0   0   0 0 0   0   0   0       0            # ")
renderLine(06 , " #  0   0   0       0   0     0    000    0   0   #6  #   0   0    0 0    0   0   00000   00000   0    # ")
renderLine(07 , " ##################################################   ################################################## ")
renderLine(08 , "                                                 c d ; e                                                 ")
renderLine(09 , " #########D#########D#########D#########D########## z ##########D#########D#########D#########D######### ")
renderLine(10 , " #                                                #a k#                                                # ")
renderLine(11 , "****************************************************-****************************************************")
renderLine(12 , "                                           +                 +                                           ")
--               012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678

tab={}
reset()
oxyd_shuffle()


