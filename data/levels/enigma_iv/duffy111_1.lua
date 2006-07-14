-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Artillery

rooms_wide=3
rooms_high=3

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
--enigma.ConserveLevel=FALSE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		elseif c =="S" then
			set_stone( "st-spitter", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c == "U" then
		   set_item("it-extralife",i-1,line)
		elseif c == "O" then
			set_stone( "st-rock1_hole", i-1, line)
		   set_item("it-extralife",i-1,line)
		   set_item("it-extralife",57-(i-1),line)
		   set_item("it-extralife",i-1,36-line)
		   set_item("it-extralife",57-(i-1),36-line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c==" " then
			set_stone( "st-rock1_hole", i-1, line)
	     end
	end	
end

renderLine(00,"##########################################################")
renderLine(01,"#                    O                                   #")
renderLine(02,"#                                                        #")
renderLine(03,"#      O                    O                            #")
renderLine(04,"#                                                        #")
renderLine(05,"#                                                        #")
renderLine(06,"#   O        O                                           #")
renderLine(07,"#                                                        #")
renderLine(08,"#                      O                                 #")
renderLine(09,"#                                                        #")
renderLine(10,"# O              gggggggggggggggggggggggg                #")
renderLine(11,"#                gqqqqqqqqqqqqqqqqqqqqqqg                #")
renderLine(12,"#                gq#+#+#+#+#++#+#+#+#+#qg                #")
renderLine(13,"#          O     gq+qqqqqqqqqqqqqqqqqq+qg                #")
renderLine(14,"#                gq#qq###qqqqqqqq###qq#qg                #")
renderLine(15,"#                gq+qq#o#qqqqqqqq#o#qq+qg                #")
renderLine(16,"#  O             gq#qq###qqqqqqqq###qq#qg                #")
renderLine(17,"#          O     gq+q###qqqqqqqqqq###q+qg                #")
renderLine(18,"#                gq#q#o#qUqSqzSqUq#o#q#qg                #")
renderLine(19,"#                gq+q###qqqqqqqqqq###q+qg                #")
renderLine(20,"#                gq#qq###qqqqqqqq###qq#qg                #")
renderLine(21,"#                gq+qq#o#qqqqqqqq#o#qq+qg                #")
renderLine(22,"#                gq#qq###qqqqqqqq###qq#qg                #")
renderLine(23,"#                gq+qqqqqqqqqqqqqqqqqq+qg                #")
renderLine(24,"#                gq#+#+#+#+#++#+#+#+#+#qg                #")
renderLine(25,"#                gqqqqqqqqqqqqqqqqqqqqqqg                #")
renderLine(26,"#                gggggggggggggggggggggggg                #")
renderLine(27,"#                                                        #")
renderLine(28,"#                                                        #")
renderLine(29,"#                                                        #")
renderLine(30,"#                                                        #")
renderLine(31,"#                                                        #")
renderLine(32,"#                                                        #")
renderLine(31,"#                                                        #")
renderLine(32,"#                                                        #")
renderLine(33,"#                                                        #")
renderLine(34,"#                                                        #")
renderLine(35,"#                                                        #")
renderLine(36,"##########################################################")

oxyd_shuffle()
















