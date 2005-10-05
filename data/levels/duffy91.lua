-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: You Must Be Kidding

rooms_wide=5
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-abyss", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-invisible", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
			set_floor("fl-metal",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {player=0})
			set_floor("fl-metal",i-1,line)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="x" then
			set_floor("fl-metal",i-1,line)
		elseif c=="s" then
			set_floor("fl-swamp",i-1,line)
		elseif c=="i" then
			set_floor("fl-inverse",i-1,line)
		elseif c=="&" then
		   set_stone("st-brownie",i-1,line)
			set_floor("fl-swamp",i-1,line)
		elseif c=="J" then
		   set_item("it-springboard",i-1,line)
			set_floor("fl-metal",i-1,line)
		elseif c == "u" then --2-u
			set_floor("fl-gradient",  i-1,  line, {type=2})
		elseif c == "r" then --3-r
			set_floor("fl-gradient",  i-1,  line, {type=3})
		elseif c == "d" then --1-d
			set_floor("fl-gradient",  i-1,  line, {type=1})
		elseif c == "l" then --4-l
			set_floor("fl-gradient",  i-1,  line, {type=4})
		elseif c == "1" then --ur
			set_floor("fl-gradient",  i-1,  line, {type=11})
		elseif c == "3" then --dl
			set_floor("fl-gradient",  i-1,  line, {type=9})
		elseif c == "7" then --dr
			set_floor("fl-gradient",  i-1,  line, {type=12})
		elseif c == "9" then --ul
			set_floor("fl-gradient",  i-1,  line, {type=10})
		elseif c=="A" then
			set_floor("fl-gradient",  i-1,  line-1, {type=2})
			set_floor("fl-gradient",  i,  line, {type=3})
			set_floor("fl-gradient",  i-1,  line+1, {type=1})
			set_floor("fl-gradient",  i-2,  line, {type=4})
			set_floor("fl-gradient",  i-2,  line+1, {type=11})
			set_floor("fl-gradient",  i,  line+1, {type=9})
			set_floor("fl-gradient",  i-2,  line-1, {type=12})
			set_floor("fl-gradient",  i,  line-1, {type=10})
			set_floor("fl-metal",i-1,line)
		elseif c=="B" then
			set_floor("fl-gradient",  i-1,  line+1, {type=2})
			set_floor("fl-gradient",  i-2,  line, {type=3})
			set_floor("fl-gradient",  i-1,  line-1, {type=1})
			set_floor("fl-gradient",  i,  line, {type=4})
			set_floor("fl-gradient",  i,  line-1, {type=11})
			set_floor("fl-gradient",  i-2,  line-1, {type=9})
			set_floor("fl-gradient",  i,  line+1, {type=12})
			set_floor("fl-gradient",  i-2,  line+1, {type=10})
			set_floor("fl-metal",i-1,line)
		elseif c=="R" then
		   set_actor("ac-rotor", i-.5,line+.5, {mouseforce=0, range=5, force=20})
                   set_floor("fl-metal",i-1,line)
	     end
	end	
end

renderLine(00,"################################################################################################")
renderLine(01,"#                                                                                              #")
renderLine(02,"#             ss=ssss7u9   s==sssi=si=sssvvv vvv vvv vvv   7uuuu9  7uuuu9             vvv    o #")
renderLine(03,"#             s      lxr   s             vAv vBv vAv vBv   lxxxxrxxlssssr       vvv   vAvvvv s #")
renderLine(04,"# 3ddd1  7u9  ss=s   1d3   ss===sssss    vvv vvv vvv vvv   lxxxxr  lssssr       vBvvvvvvvvBv s #")
renderLine(05,"# rxxxlxxlxr     s     vvv        sss    x vvv vvv vvv vvv 1dddd3  1dddd3    vvvvvvvBv   vvv s #")
renderLine(06,"# rxzxl  1d33d1  s     vAvvvv     sss    x vBv vAv vBv vAvxx  x      x  RRxxxvAv   vvv vvv   s #")
renderLine(07,"# rxxxl     rxliix     vvvvBv   vvv      x vvv vvv vvv vvv 7uuuu9  7uuuu9    vvv       vAv   s #")
renderLine(08,"# 9uuu7vvv  9u7           vvvvvvvAv      vvv vvv vvv vvv   l=79=r  liiiir xo        vvvvvv vvv #")
renderLine(09,"#      vAviix                vBvvvv      vAv vBv vAv vBv   l=13=rxxliiiir i    =ss  vBv    vAv #")
renderLine(10,"#      vvv                   vvv         vvv vvv vvv vvv   1dddd3  1dddd3 xs===i iixvvvxiixvvv #")
renderLine(11,"#                                                                                              #")
renderLine(12,"################################################################################################")

oxyd_shuffle()

display.SetFollowMode(display.FOLLOW_SCROLLING)
