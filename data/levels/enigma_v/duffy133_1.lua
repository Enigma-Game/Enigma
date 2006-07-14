-- Humid Maze, a level for Enigma
-- Copyright (C) 2005 Jacob Scott
-- Licensed under GPL v2.0 or above 

rooms_wide=4
rooms_high=3

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.WaterSinkSpeed=0.15
enigma.SwampSinkSpeed=0.5

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
		   set_stone("st-invisible",i-1,line)
			abyss(i-1,line)
		elseif c == "o" then
			oxyd( i-1, line)
		   set_floor("fl-water",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
                elseif c == " " then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c == "S" then
		   set_floor("fl-swamp",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {player=0})
		   set_floor("fl-swamp",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
                elseif c == "F" then
		   set_item("it-flagblack",i-1,line)
		   set_floor("fl-swamp",i-1,line)
                elseif c == "s" then
		   if not difficult then
		      set_item("it-seed",i-1,line)
		   end
		   set_floor("fl-swamp",i-1,line)
                elseif c == "O" then
		   if not difficult then
		      set_item("it-extralife",i-1,line)
		   end
		   set_floor("fl-swamp",i-1,line)
	     end
	end	
end

renderLine(00,"#############################################################################")
renderLine(01,"#                                                                           #")
renderLine(02,"# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~    ~~~~~   ~~~~ #")
renderLine(03,"#           ~                ~                ~           ~   SSS  ~~~~~  ~ #")
renderLine(04,"# ~~~~~~~~  ~  ~~~~~~~~~~~~  ~~~~~~~~~~~~ ~~~ ~ ~~~~~~~   ~  SSSS         ~ #")
renderLine(05,"# ~      ~~~~  ~          ~  ~          ~ ~   ~ ~     ~   ~ SSSSS ~~~ ~~~~~ #")
renderLine(06,"# ~  SSS       ~   ~~~~~~ ~ SS ~~~~~~~~ ~ ~~~~~ ~ ~~~ ~   ~ SSSSS ~   ~     #")
renderLine(07,"# ~ SSSSS~~~~~~~   ~    ~ ~ SS ~      ~ ~ ~     ~ ~ ~ ~   ~     ~ ~ ~ ~ ~ ~ #")
renderLine(08,"# ~ SSSS       ~   ~    ~ ~ SS ~  ~~~ ~ ~ ~ ~~~~~ ~ ~ ~   ~ ~~~ ~ ~ ~ ~ ~ ~ #")
renderLine(09,"# ~ SSSS ~~~~~ ~   ~ ~~~~ ~ SS ~  ~ ~ ~ ~ ~ ~     ~ ~ ~   ~~~ ~ ~ ~ ~ ~ ~ ~ #")
renderLine(10,"# ~      ~   ~ ~   ~ ~    ~    ~  ~ ~ ~ ~ ~ ~~~~~ ~ ~ ~       ~ ~ ~ ~ ~ ~ ~ #")
renderLine(11,"# ~~~~~~~~ ~ ~ ~   ~ ~ ~~~~~~~~~  ~ ~ ~ ~ ~       ~ ~ ~   ~~~ ~ ~ ~ ~ ~ ~ ~ #")
renderLine(12,"# ~        ~ ~ ~   ~ ~ ~       ~  ~ ~ ~ ~ ~~~~~~~~~ ~ ~   ~ ~~~ ~ ~ ~ ~ ~ ~ #")
renderLine(13,"# ~~~~~~~~~~ ~ ~~~~~ ~ ~ ~~~~~ ~  ~   ~ ~    ~    ~ ~ ~   ~     ~ ~ ~ ~ ~ ~ #")
renderLine(14,"#            ~     ~ ~ ~ ~   ~ ~~ ~ ~~~ ~   ~~  ~ ~ ~ ~~~~~~~~~~~~~ ~ ~ ~ ~ #")
renderLine(15,"# SS~~~~~~~~ ~~~~~ ~ ~ ~ ~~~ ~  ~ ~     ~   ~   ~ ~ ~               ~ ~ ~ ~ #")
renderLine(16,"# SSS      ~ ~   ~ ~ ~ ~   ~ ~  ~ ~~~~~~~~  ~~  ~ ~ ~~~~~~~~~ ~~~~~~~ ~ ~ ~ #")
renderLine(17,"# SSSS ~~~ ~ ~~~ ~ ~ ~ ~ ~~~ ~ ~~        ~~  ~  ~ ~           ~       ~ ~ ~ #")
renderLine(18,"# SSSS ~ ~ ~ ~ ~ ~   ~ ~ ~   ~ ~    SSOS  ~~ ~~~~ ~~~~~~~~~~~ ~       ~ ~ ~ #")
renderLine(19,"#  SSS ~ ~ ~ ~ ~ ~~~ ~ ~ ~~~ ~ ~~~~~SSSFS  ~            ~   ~ ~ ~~~~~~~ ~ ~ #")
renderLine(20,"#    ~~~ ~ ~ ~ ~   ~ ~ ~   ~ ~      SSSSzS ~ ~~~~~~~~~~~~ ~~~ ~ ~       ~ ~ #")
renderLine(21,"#        ~ ~ ~ ~~~ ~ ~ ~ ~~~ ~ ~~~~  SsSSS ~ ~ooo~            ~ ~ ~~~~~~~ ~ #")
renderLine(22,"# ~~~~~~~~ ~ ~   ~ ~ ~ ~ ~   ~    ~     ~  ~ ~ooo~ ~~~~~~~~~~~~ ~ ~ ~ ~ ~ ~ #")
renderLine(23,"# ~      ~ ~ ~~~ ~ ~ ~ ~ ~~~ ~~~~~~~~~~~~  ~ ~~~~~ ~            ~ ~ ~ ~ ~ ~ #")
renderLine(24,"# ~~~~~~ ~ ~ ~ ~ ~ ~ ~     ~               ~       ~   ~~~~~~~~ ~ ~ ~ ~ ~ ~ #")
renderLine(25,"# ~      ~ ~ ~ ~ ~ ~ ~~~~~ ~   ~~~~~~~~~~~~~~~~~~~~~   ~      ~ ~ ~ ~ ~ ~ ~ #")
renderLine(26,"# ~ ~~~~~~ ~ ~ ~ ~ ~     ~     ~                       ~  SSS ~ ~ ~ ~ ~ ~ ~ #")
renderLine(27,"# ~        ~ ~ ~ ~ ~ ~ SSSSSSS ~   ~~~~~~~~~~~~~~~~~~  ~ SSSS ~ ~ ~ ~ ~ ~ ~ #")
renderLine(28,"# ~~~~~~~~ ~ ~ ~ ~ ~ ~ SSSSSSS ~~~~~                ~  ~ SSSS ~ ~ ~ ~ ~ ~~~ #")
renderLine(29,"#        ~ ~ ~ ~ ~ ~ ~  SSS            ~~~~~~~~~~~  ~  ~ SSSS ~ ~ ~ ~     ~ #")
renderLine(30,"# ~~~~~~~~ ~     ~   ~    ~~~~~~~~~~~~~~         ~~~~  ~   ~  ~ ~ ~ ~~~~~ ~ #")
renderLine(31,"# ~        ~~~~~~~~~~~              ~    ~~~~~~~       ~   ~  ~ ~       ~ ~ #")
renderLine(32,"# ~~~~~~~~                 ~~~~~~~  ~~~~~~     ~~~~~~~~~   ~  ~ ~~~~~~~~~ ~ #")
renderLine(33,"# ~        ~~~   ~~~~~     ~     ~         ~~~             ~              ~ #")
renderLine(34,"# ~~~~~~~~~~ ~~~~~   ~~~~~~~     ~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #")
renderLine(35,"#                                                                           #")
renderLine(36,"#############################################################################")

oxyd_shuffle()

display.SetFollowMode(display.FOLLOW_SCROLLING)






















