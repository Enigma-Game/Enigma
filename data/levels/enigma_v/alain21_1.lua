-- A bayou by you, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 105
levelh = 33

create_world(levelw, levelh)
oxyd_default_flavor = "a"
display.SetFollowMode(display.FOLLOW_SCROLLING)
fill_floor("fl-leaves", 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		elseif c=="*" then
			set_stone("st-rock1_move",i-1,line)
		elseif c=="." then
			set_stone("st-greenbrown",i-1,line)
		elseif c=="x" then
			set_stone("st-death",i-1,line)
		elseif c=="X" then
			document(i-1,line,"The most dangerous animals of the bayou are the alligators, the mosquitoes ... and the zombies ! (But the zombies at least, don't move ...)")
		elseif c=="u" then
			set_item("it-umbrella",i-1,line)
		elseif c=="a" then
			set_floor("fl-abyss",i-1,line)
		elseif c=="+" then
			set_stone("st-black4",i-1,line)
		elseif c=="|" then
 	           puzzle(i-1,line,PUZ_1010)
		elseif c=="-" then
	            puzzle(i-1,line,PUZ_0101)
		elseif c=="A" then
 	           puzzle(i-1,line,PUZ_0110)
		elseif c=="B" then
	            puzzle(i-1,line,PUZ_0011)
		elseif c=="C" then
 	           puzzle(i-1,line,PUZ_1100)
		elseif c=="D" then
	            puzzle(i-1,line,PUZ_1001)
		elseif c=="/" then
			set_item("it-trigger",i-1,line,{action="callback", target="flytox"})
		elseif c=="0" then
			set_floor("fl-swamp",i-1,line)
		elseif c=="1" then
			set_actor("ac-rotor",i-0.5,line+0.5,{range=40,force=20,name="mosquito1",gohome=FALSE})
		elseif c=="2" then
			set_actor("ac-rotor",i-0.5,line+0.5,{range=40,force=20,name="mosquito2",gohome=FALSE})
		elseif c=="3" then
			set_actor("ac-rotor",i-0.5,line+0.5,{range=40,force=20,name="mosquito3",gohome=FALSE})
		elseif c=="4" then
			set_actor("ac-rotor",i-0.5,line+0.5,{range=40,force=20,name="mosquito4",gohome=FALSE})
		elseif c=="5" then
			set_actor("ac-rotor",i-0.5,line+0.5,{range=40,force=20,name="mosquito5",gohome=FALSE})
		elseif c=="6" then
			set_actor("ac-rotor",i-0.5,line+0.5,{range=40,force=20,name="mosquito6",gohome=FALSE})
		elseif c=="7" then
			set_actor("ac-top",i-0.5,line+0.5,{range=40,force=20,name="alligator1",gohome=TRUE})
		elseif c=="8" then
			set_actor("ac-top",i-0.5,line+0.5,{range=20,force=15,name="alligator2",gohome=TRUE})
		elseif c=="w" then
			set_floor("fl-water",i-1,line)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5, {name="maroon",player=0})
		elseif c=="o" then
			oxyd(i-1,line)
		end
	end	
end

-- piege a moustiques

function flytox()
	for i=19,22 do
		set_stone("st-black4",82,i)
	end
end

--               012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234
renderLine(00 , "    0000000000000000000000000000000000000000000000000000000000000000000000000000        00000000000000000")
renderLine(01 , " zX000000000000000000000                00000  00000   0000    000000000000000      x    0000000000000000")
renderLine(02 , "    00000000    00000      000000000    00000  000  0  00       000000000000000        00000000000000000")
renderLine(03 , " 00   000000       00   000000     0000   00000  0  00  0   000   000000000000           00000000   00000")
renderLine(04 , "000   00000   000      0000000  00  0000   00000   0000    00000   000000000    0000000   000000  7  0000")
renderLine(05 , "0000   000   00000    0000000  0000  0000   0000000000    000000   0000000     000000000   000000    0000")
renderLine(06 , "00000   00   000000   000000  000000  0000   0000000    0000000   000000    0000000  0000   000000   0000")
renderLine(07 , "000000       0000000  000000  00000  000000            0000000   000000   00000000  u 0000   000000  0000")
renderLine(08 , "000000     000ww0000  000000000000  00000000          00000000   00000   0000000000    0000   00000000000")
renderLine(09 , "000000   0000www000     000000000  000000000  00000      000000   000   000000000000   000000   000000000")
renderLine(10 , "000000   000www000  0    0000000  0000000000  00000000      0000       00000000    00   0000000    000000")
renderLine(11 , "00  000   00ww000  000   0000000    000000000  00000000000    0000    000000000      0   0000000     0000")
renderLine(12 , "0  u 000   0ww000  00000  000000  u  000000000  0000000000000   00000000000000   00   0   00000       000")
renderLine(13 , "00   0000   0ww00  00000  0000000    0000000000  0000000000000   000000ww0000   00000     0000    #    00")
renderLine(14 , "000   000   00ww00  0000  00000000   00000000000  000000   0000   0000wwww000   0000000  00000   #o*   00")
renderLine(15 , "0000   00   00ww000  0000  0000000  00000www00000  000000   0000   000wwww000   0000000  000000   #   000")
renderLine(16 , "0000   000   0000000  0000  00000  000000wwww00000  000000   000   000www000    0000000  00000000    0000")
renderLine(17 , "0000   0000   000000   0000  000  0000000wwww000000 000000 u 000   000www00      0000000  000000   000000")
renderLine(18 , "00000   0000   0000  u  0000  0  000000wwwwwww00000 000000   000   000ww00         000000  00000   000000")
renderLine(19 , "0000    00000   0000    0000    0000000wwwwww000000 0000000  000   0000000   +++++   00000  00000   00000")
renderLine(20 , "0000  u  0000   000000  00000  00000000wwwwww000000x00000000      00000000  /+        00000  0000   00000")
renderLine(21 , "00000   000000   000000000000  00000000wwwwww000000 000000000    0000000000  +        000000  0    000000")
renderLine(22 , "00000   000000   00000000000  00000000wwwww0000000  0000000000   00000wwww00 +++++   0000000      0000ww0")
renderLine(23 , "00000   0000000        0000  000000000wwwww0000000  00000000000   000wwwww00        000000     000000wwww")
renderLine(24 , "000000   000000              000000000wwww0000000    00000ww0000   00www000    000   0000   00000000wwwww")
renderLine(25 , "0000000   000000  00000      0000000000ww0000000      00000ww0000   000000   000000   00   00000000wwwwww")
renderLine(26 , "0000000   000000  0000000     00000000000000000    #   00000ww0000    000   00000000      00000000wwwwwww")
renderLine(27 , "0000000    ....    00000 ..+..  000000000000000   #o#  000000ww00000       0000000000    0000000wwwwwwwww")
renderLine(28 , "0000000    .8      00000 .   .    0000000000000    *   000000www000000    00000www0000000000000wwwww123ww")
renderLine(29 , "0000000000 ....    0000  .   .     00000000000000    00000000wwww00000000000000wwww00000000000wwwwww456ww")
renderLine(30 , "0000000000000000  00000  . . .     00000000000000000000000000wwwww0000000000000wwww00000000000wwwwwwwwwww")
renderLine(31 , "000000000000000000000000 *   *    0000000000000000000000000000wwwwww00000000000wwwww0000000000wwwwwwwwwww")
renderLine(32 , "0000000000000000000000000        00000000000000000000000000000wwwwww00000000000wwwww0000000000wwwwwwwwwww")
--               012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234

oxyd_shuffle()











