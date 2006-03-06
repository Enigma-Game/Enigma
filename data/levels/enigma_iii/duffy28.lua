-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Black Diamond Course

levelw=80
levelh=40

create_world( levelw, levelh)

fill_floor("fl-abyss", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
			set_actor("ac-blackball", i-.5,line+.5)
			fill_floor("fl-leaves", i-1,line,1,1)
		elseif c==" " then
		        abyss(i-1,line)
		elseif c == "s" then
			set_item("it-spring1",i-1,line)
			fill_floor("fl-leaves", i-1,line,1,1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="*" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c=="H" then
			set_stone("st-stone_break", i-1,line)
		elseif c=="9" then
			set_stone( "st-floppy", i-1,line,{action="openclose",target="unimportantdoor"})
		elseif c=="a" then
			fill_floor("fl-leaves", i-1,line,1,1)
		elseif c=="b" then
			fill_floor("fl-sand", i-1,line,1,1)
		elseif c=="c" then
			fill_floor("fl-bluegreen", i-1,line,1,1)
		elseif c=="d" then
			fill_floor("fl-inverse", i-1,line, 1,1)
		elseif c=="e" then
			fill_floor("fl-ice_001", i-1,line, 1,1)
		elseif c=="f" then
			fill_floor("fl-marble", i-1,line, 1,1)
		elseif c=="g" then
			fill_floor("fl-wood", i-1,line, 1,1)
		elseif c=="h" then
			fill_floor("fl-hay", i-1,line, 1,1)
		elseif c=="i" then
			fill_floor("fl-dunes", i-1,line, 1,1)
		elseif c=="j" then
			fill_floor("fl-brick", i-1,line, 1,1)
		elseif c=="k" then
			fill_floor("fl-gray", i-1,line, 1,1)
		elseif c=="l" then
			fill_floor("fl-plank", i-1,line, 1,1)
		elseif c=="m" then
			fill_floor("fl-metal",i-1,line, 1,1)
		elseif c=="n" then
			fill_floor("fl-normal", i-1,line, 1,1)
		elseif c=="O" then
			fill_floor("fl-rough", i-1,line, 1,1)
		elseif c=="p" then
			fill_floor("fl-space", i-1,line, 1,1)
		elseif c=="q" then
			fill_floor("fl-bluegreenx", i-1,line, 1,1)
		elseif c=="r" then
			fill_floor("fl-leaves", i-1,line, 1,1)
			set_stone("st-chameleon", i-1,line)
		elseif c=="S" then
			fill_floor("fl-floor_001", i-1,line, 1,1)	
		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"
                   
renderLine(00,"##########################################################")
renderLine(01,"#            c   #       jjj              #           ff #")
renderLine(02,"# aaaa bb  ccccc #     jjj j   kkkk   iii #   gg     fff #")
renderLine(03,"# azsa bbb ccccc #   jjj jjj   kk k   iii # gggg    ffff #")
renderLine(04,"# aaaa     ccc c #   j jjj j   kkkk   ii  #   ggg    fff #")
renderLine(05,"#                #   jjj jjj   kkk           d        ff #")
renderLine(06,"######    ddd     #    jjj j    k      h    ddd          #")
renderLine(07,"#     ee   ddd    #      jjj           hhh ddddd  #    p #")
renderLine(08,"#fff eeee dddd    #                         ddd   #   pp #")
renderLine(09,"#fff eeee  ddd    #   ll       ##########    d    #   pp #")
renderLine(10,"#f   eee      #####   lllll    #         #        #   pp #")
renderLine(11,"#ff  eeee  ###    #   olllo   ##  OOOOOO  #####          #")
renderLine(12,"#fff      #   j    #        ## #  OOOOOO                 #")
renderLine(13,"#       ##  jjj     #######*  r * OO  OO  jjjjjjj   # dd #")
renderLine(14,"#ggg   #  jjjjj   #    kkk rrrrr* OO  OO  jjjjj     # dd #")
renderLine(15,"#ggg  #    jjjjj #   kkkkk rrrrr* OOOOOO   jjj  aa  # dd #")
renderLine(16,"#gg  # OO   jjjj #    kkkk rrrrr* OOOOOO   j    aaa # dd #")
renderLine(17,"#gg # OOOO    jj                        e      aaaa #    #")
renderLine(18,"#gg   OOOO  # jj       qqq #SSSSS#  eeeeee        a #    #")
renderLine(19,"#      OOO #    #     qqqq #SSSSS# eeeeee #              #")
renderLine(20,"#      OO  #     pppp  qqq # SSSS#  eeeee #  bbbbb   ccc #")
renderLine(21,"#mmm      # iiii pppp  qqqq#l  S# nn      #  bbbb   cccc #")
renderLine(22,"#mmmmmmm # iiiii pppp  qqq #lll nnnnnn    #    bb   cccc #")
renderLine(23,"#mmmmmm  #  iii           # lll nnnn      #    bb  ccccc #")
renderLine(24,"##########################################################")

oxyd_shuffle()

--through=0
--function funcc()
--    if through==1 then
--	through=0
--    end
--    if through==0 then
--	SendMessage(door1, "openclose")
--	SendMessage(door2, "openclose")
--	SendMessage(door3, "openclose")
--	SendMessage(bridge1, "openclose")
--	SendMessage(bridge2, "openclose")
--    end
--    through=through+1
--end














