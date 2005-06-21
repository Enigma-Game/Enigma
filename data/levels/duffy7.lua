-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Jump!

levelw = 100
levelh = 100

create_world(levelw, levelh)

draw_border("st-brownie")
fill_floor("fl-hay")



function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			abyss(i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "s" then
			set_item("it-spring1",i-1,line)
		elseif c == "m" then
			fill_floor("fl-metal", i-1,line, 1,1)
		elseif c == "i" then
			set_stone( "st-invisible", i-1, line)
		elseif c == "h" then
			set_item("it-trigger", i-1,line, {action="openclose",target="door1"})
		elseif c == "H" then
			set_stone("st-door", i-1, line , {name="door1", type="v"})
		elseif c == "*" then
			set_stone("st-brownie", i-1, line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
                elseif c == "u" then
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "r" then
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "b" then
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "l" then
			set_floor("fl-gradient",  i-1,  line, {type=4})
 		elseif c == "d" then
			set_stone("st-death", i-1,line)
                elseif c == "1" then
			oneway(i-1,line, enigma.NORTH)
                elseif c == "4" then
			oneway(i-1,line, enigma.SOUTH)
                elseif c == "3" then
			oneway(i-1,line, enigma.WEST)
 		elseif c == "2" then
			oneway(i-1,line, enigma.EAST)
		elseif c == "~" then
			fill_floor("fl-water", i-1,line, 1,1)
                elseif c == "!" then
			abyss(i-1,line)
                elseif c == "@" then
			document(i-1,line,"I'ts a race!")
                elseif c == "+" then
		        set_stone("st-greenbrown",i-1,line)
 		elseif c == "f" then
		        fakeoxyd(i-1,line)
		elseif c == "v" then
		        oneway(i-1,line, enigma.WEST)
		elseif c == "y" then
		        oneway(i-1,line, enigma.NORTH)
		elseif c == "z" then	
			set_actor("ac-blackball" ,i-1, line)
                end
	end
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"


renderLine(00,"dddddddddddddddddddddddddddddddddddddddddddddddddddddddddd")
renderLine(01,"d########################+###########++++++++++++++######d")
renderLine(02,"d#o    ##################+o####                    ######d")
renderLine(03,"d#     ##    ############+###########              ######d")
renderLine(04,"d#  z  ##    ############+###########              ######d")
renderLine(05,"d# s   ##   ###  ########ddd#########              ######d")
renderLine(06,"d#     ##   ###     #######+#########              ######d")
renderLine(07,"d##############      ######+#########++##########++######d")
renderLine(08,"d#######+########    ####################################d")
renderLine(09,"d######+##   ############################################d")
renderLine(10,"d#####+###   ############################################d")
renderLine(11,"d####+####   ##    ######################################d")
renderLine(12,"d###+##########   #######m########d########  ############d")
renderLine(13,"d#########  ##   ####mmmmm########d########  ############d")
renderLine(14,"d########    #    ###mmmmmmmm+#############  ############d")
renderLine(15,"d#######    ##    vvvmmmmm#################  ############d")
renderLine(16,"d#######  #####################d###########  ############d")
renderLine(17,"d###########+++########     ###d###########  ############d")
renderLine(18,"d###########+f+########     ###d###########  ############d")
renderLine(19,"d######################     ##+d###########  ############d")
renderLine(20,"d###########   ########yyoyy##+############  ############d")
renderLine(21,"d###########        ###      #+############  ############d")
renderLine(22,"dd ######            #       #+############  ############d")
renderLine(23,"dd ######       ######       #+d####   #         #  +  ##d")
renderLine(24,"dd ###################     ####d####   #         #     ##d")
renderLine(25,"d# ########    ################d#### f ###########     ##d")
renderLine(26,"d# #######    ########rrrrrbb#######iii###########     ##d")
renderLine(27,"d#           #########rrrrrbb#######iii###########     ##d")
renderLine(28,"d#######    ##########uu###bb############################d")
renderLine(29,"d#####################uu###bb############################d")
renderLine(30,"d#####################uu###bb###           ### ##### ####d")
renderLine(31,"d####    #############uulllll##            ### ##### ####d")
renderLine(32,"d###    ##############uulllll##              #       ####d")
renderLine(33,"d##    ############################################# ####d")
renderLine(34,"d#o   ############################################## ####d")
renderLine(35,"d########################################################d")
renderLine(36,"dddddddddddddddddddddddddddddddddddddddddddddddddddddddddd")

oxyd_shuffle()
display.SetFollowMode(display.FOLLOW_SCROLLING)
