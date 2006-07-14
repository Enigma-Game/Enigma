-- Road to Eden, a level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 42
levelh = 40

create_world(levelw, levelh)
oxyd_default_flavor = "b"
display.SetFollowMode(display.FOLLOW_SCROLLING)
fill_floor("fl-leaves", 0,0, level_width,level_height)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c=="*" then
			set_stone( "st-wood2", i-1, line)
		elseif c=="." then
			set_stone("st-wood",i-1,line)
		elseif c=="a" then
			set_floor("fl-water",i-1,line)
		elseif c=="b" then
			set_floor("fl-sand",i-1,line)
		elseif c=="c" then
			set_floor("fl-water",i-1,line)
			set_stone("st-chameleon",i-1,line)
		elseif c=="/" then
			set_floor("fl-sand",i-1,line)
			set_item("it-trigger",i-1,line,{invisible=1,action="callback", target="pursuit"})
		elseif c=="x" then
			set_floor("fl-sand",i-1,line)
			set_item("it-wormhole",i-1,line, {invisibble=1,targetx=" 12.5",targety="30.5",strength=0,range=0})
			set_stone("st-chameleon",i-1,line)
		elseif c=="q" then
			set_floor("fl-sand",i-1,line)
			set_item("it-wormhole",i-1,line, {invisibble=1,targetx=" 30.5",targety="11.5",strength=0,range=0})
			set_stone("st-chameleon",i-1,line)
		elseif c=="y" then
			set_floor("fl-sand",i-1,line)
			set_stone("st-chameleon",i-1,line)          
		elseif c=="z" then
			set_floor("fl-sand",i-1,line)
			set_actor("ac-blackball", i-.5,line+.5, {name="maroon",player=0})
		elseif c=="o" then
			oxyd(i-1,line)
		elseif c=="p" then
			set_stone("st-fakeoxyd",i-1,line)
		end
	end	
end

anz_rotors=0

function pursuit1()
	if devil==1 and anz_rotors<6 then
		set_actor("ac-rotor",8.5,11.5,{range=40,force=20,name="nickel1",gohome=FALSE})
                anz_rotors = anz_rotors+1
	end
end

function pursuit()
	devil=1
	pursuit1()
	devil=0
end

--               012345678901234567890123456789012345678901
renderLine(00 , "####################  ####################")
renderLine(01 , "#bbbbbbbbbb#yyy#bbb#  #bbbbbbbbbb#bbb#bbb#")
renderLine(02 , "#b########b#y#b#b#b#  #b########b#b#b#b#b#")
renderLine(03 , "#bbbbbbbb#b#y#b#b#b#  #bbbbbbbb#b#b#b#b#b#")
renderLine(04 , "########b#b#x#b#b#b#  ########b#b#b#b#b#b#")
renderLine(05 , "#bbbbbbbb#b#p#bbb#b#  #bbbbbbbb#b#o#bbb#b#")
renderLine(06 , "#b########b#######b#  #b########b#######b#")
renderLine(07 , "#bbbbbbbb#b#bbbbbbb#  #bbbbbbbb#b#bbbbbbb#")
renderLine(08 , "########b#b#b#######  ########b#b#b#######")
renderLine(09 , "#bbbbbbbb#b#bbbbbbb#  #bbbbbbbb#b#bbbbbbb#")
renderLine(10 , "#b########b#######b#  #b########b#######b#")
renderLine(11 , "#/bbbbbbz#bbbbbbbbb#  #bbbbbbbb#bbbbbbbbb#")
renderLine(12 , "##########################################")
renderLine(13 , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
renderLine(14 , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
renderLine(15 , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
renderLine(16 , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
renderLine(17 , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
renderLine(18 , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
renderLine(19 , "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
renderLine(20 , "cccccccccccccccccccaybybybybybybybybybybyb")
renderLine(21 , "cccccccccccccccccccabybybybybybybybybybyby")
renderLine(22 , "cccccccccccccccccccayyybybybybyyybybybyqyy")
renderLine(23 , "cccccccccccccccccccayybybybybybybybybybyyy")
renderLine(24 , "cccccccccccccccccccayyyyyyyyyyyyyyyyyyyyyy")
renderLine(25 , "*******************.**********************")
renderLine(26 , "                               b          ")
renderLine(27 , "  bbb                 aaaaaaa bpb aaaaaa  ")
renderLine(28 , "  bob                 a        b       a  ")
renderLine(29 , "  bbb                 a aaaaaa  aaaaaa a  ")
renderLine(30 , "                      a                a  ")
renderLine(31 , "                      aaaaaaa    aaaaaaa  ")
renderLine(32 , "                               o          ")
renderLine(33 , "   bbbbbbbbb          aaaaaaa    aaaaaaa  ")
renderLine(34 , "   baaaaaaab          a                a  ")
renderLine(35 , "   baaapaaab          a aaaaaa  aaaaaa a  ")
renderLine(36 , "   baaaaaaab          a        b       a  ")
renderLine(37 , "   bbbbbbbbb          aaaaaaa bob aaaaaa  ")
renderLine(38 , "                               b          ")
renderLine(39 , "                                          ")
--               012345678901234567890123456789012345678901

devil=0
oxyd_shuffle()



