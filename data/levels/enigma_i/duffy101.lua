-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Variety Pack

rooms_wide=3
rooms_high=3

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="W" then
			set_stone( "st-greenbrown", i-1, line)
		   set_floor("fl-water",i-1,line)
		elseif c =="S" then
			set_stone( "st-greenbrown", i-1, line)
		   set_floor("fl-space",i-1,line)
		elseif c =="q" then
			set_stone( "st-actorimpulse_invisible", i-1, line)
		elseif c =="%" then
			set_stone( "st-rock6", i-1, line)
			set_floor("fl-space",i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "O" then
			oxyd( i-1, line)
		   set_floor("fl-water",i-1,line)
		elseif c == "&" then
			oxyd( i-1, line)
		   set_floor("fl-space",i-1,line)
		elseif c == "@" then
			oxyd( i-1, line)
			abyss(i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c == "m" then
		   set_floor("fl-metal",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c == "Q" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
			abyss(i-1,line)
		elseif c == "a" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		   set_floor("fl-metal",i-1,line)
		elseif c == "G" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
			abyss(i-1,line)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="R" then
		   set_actor("ac-rotor", i-.5,line+.5, {range=7, force=30})
		elseif c == "-" then
		   puzzle(i-1,line,PUZ_0101)
		elseif c == "_" then
		   puzzle(i-1,line,PUZ_0101)
			abyss(i-1,line)
		elseif c == "i" then
		   puzzle(i-1,line,PUZ_1010)
		elseif c == "/" then
		   puzzle(i-1,line,PUZ_0110)
		elseif c == "`" then
		   puzzle(i-1,line,PUZ_0011)
		elseif c == "^" then
		   puzzle(i-1,line,PUZ_1001)
		elseif c == ";" then
		   puzzle(i-1,line,PUZ_1100)
		elseif c == "2" then
		   puzzle(i-1,line,PUZ_1000)
		elseif c == "4" then
		   puzzle(i-1,line,PUZ_0100)
		elseif c == "6" then
		   puzzle(i-1,line,PUZ_0001)
		elseif c == "8" then
		   puzzle(i-1,line,PUZ_0010)
		elseif c == "$" then
		   puzzle(i-1,line,PUZ_0000)
		elseif c == "5" then
		   puzzle(i-1,line,PUZ_0100)
		   abyss(i-1,line)
		elseif c=="U" then
                    set_stone("st-oneway-n", i-1,line)
--		elseif c=="R" then
-- 			set_stone("st-oneway", i-1,line, {orientation=enigma.EAST})
		elseif c=="D" then
			set_stone("st-oneway-s", i-1,line)	
		elseif c=="L" then
 			set_stone("st-oneway-w", i-1,line)
		elseif c=="?" then
		   fill_floor("fl-ice_001",i-2,line,19,12)
		elseif c=="b" then
			yy1( "black",  i-1, line)
		elseif c=="l" then
			set_attrib(laser(i-1,line, TRUE, WEST), "name", "laser1")
		   set_floor("fl-water",i-1,line)
		elseif c=="<" then
			set_attrib(laser(i-1,line, TRUE, EAST), "name", "laser2")
		elseif c==">" then
			set_attrib(laser(i-1,line, TRUE, WEST), "name", "laser3")
		elseif c=="f" then	
			set_item("it-floppy", i-1,line)
		elseif c=="F" then
			set_stone( "st-floppy", i-1,line,{action="callback",target="funcc1"})
		elseif c=="A" then
		   set_stone("st-door_a",i-1,line,{name="door1"})
		elseif c=="B" then
		   set_stone("st-door_a",i-1,line,{name="door2"})
		elseif c=="C" then
		   set_stone("st-door_a",i-1,line,{name="door3"})
	     end
	end	
     end

function yy1( color, x, y)
        stone = format( "st-%s4", color)
        set_stone( stone, x, y)
end

renderLine(00,"############################oo#########SSSSSSSSSSSSSSSSSSS")
renderLine(01,"#                  #    <        >    #==================S")
renderLine(02,"#~~~~~~~~~         #      DUUUUD      #==m============mm=S")
renderLine(03,"#~o     +~         #        RR        #===============mm=S")
renderLine(04,"#~~~~~~~~~         #                  #========%%========S")
renderLine(05,"#                  #                  #==================S")
renderLine(06,"#                  *                  +a===%=m==========%S")
renderLine(07,"#        qq        #                  #===%========%=====S")
renderLine(08,"#                  #       ~~~~       #==%==========%====S")
renderLine(09,"#                  #                  #==============%===S")
renderLine(10,"#                  #                  #==m=====mm========S")
renderLine(11,"#                  #                  #========mm==%====&S")
renderLine(12,"#########  #################gg#################  #########")
renderLine(13,"#o             ~~  #f                 #       #          #")
renderLine(14,"#              *~  #                  #       ########## #")
renderLine(15,"#              ~~  #                  # gg    + + + +  # #")
renderLine(16,"#              ~~  #                  # +++  ggggggggg## #")
renderLine(17,"#              ~~  #                  #  g+  ~~~~~~~~~   #")
renderLine(18,"#g~~~~~~~~~~~~~l~  g         z        g+ +   ~~~~~~~~Wo#o#")
renderLine(19,"#              ~~  #                  #  g+  ~~~~~~~~~   #")
renderLine(20,"#              ~~  #                  # +++  ggggggggg## #")
renderLine(21,"#              ~~  #                  # gg    + + + +  # #")
renderLine(22,"#              *~  #                  #       ########## #")
renderLine(23,"#o             ~~  #                 F#       #          #")
renderLine(24,"#########  #################AB#################  #########")
renderLine(25,"#? /-----$--6      #                  #    !!      !!!!#o#")
renderLine(26,"#  i               #  +  +  +  +  +   #    !!#     !!!! 8#")
renderLine(27,"#  i               #!!!!!!!!!!!!!!!!!!#    G5$--$--__`g i#")
renderLine(28,"#  i               #QQQQQQQQ!!QQQQQQQQ#    !! !!  !!!i  i#")
renderLine(29,"#  i               #!!!!!!!!!!!!!!!!!!#    !! !!# !!!i  i#")
renderLine(30,"#  $               g   +!!!!!!!!!!+   g    !!D!!!U!! $C $#")
renderLine(31,"#  i               # +++!!!!!!!!!!+++ #              2# i#")
renderLine(32,"#  i               #   +!!!!!!!!!!+   #               # i#")
renderLine(33,"#  i               #!!!!!@!!!!!!@!!!!!#              ## i#")
renderLine(34,"#  i               #!!!!!!!!!!!!!!!!!!#              ## i#")
renderLine(35,"#o ;----------------__________________------------------^#")
renderLine(36,"##########################################################")

oxyd_shuffle()

ff1=0
function funcc1()
if ff1==0 then
   SendMessage("door1", "open")
   SendMessage("door2", "open")
   SendMessage("door3", "close")
ff1=1
elseif ff1==1 then
    SendMessage("door1","close")
    SendMessage("door2","close")
    SendMessage("door3","open")
ff1=0
end
end














