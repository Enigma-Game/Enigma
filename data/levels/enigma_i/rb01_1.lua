-- HostileWorld
-- A level for Enigma
--
-- Copyright (c) 2005 Roberto Bardin
-- Licensed under the GPL version 2.

rooms_wide=1
rooms_high=2

levelw=1+(19*rooms_wide)+1
levelh=1+(12*rooms_high)

create_world(levelw, levelh)

oxyd_default_flavor="a"

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock1", i-1, line)
		elseif c =="Z" then
			set_stone( "st-rock1", i-1, line)
		   set_floor("fl-hay",i-1,line)
		elseif c =="0" then
			set_stone( "st-knight", i-1, line)
		elseif c =="r" then
			if difficult then
			   set_actor("ac-rotor", i-.5,line+.5,
			{controllers=0,gohome=FALSE,force=20,range=8})
			 else
			   set_actor("ac-rotor", i-.5,line+.5,
			{controllers=0,gohome=FALSE,force=10,range=8})
			 end
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-1,line+.5, {player=0})
                elseif c == "'" then
			abyss(i-1,line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
                end
        end
end

renderLine(00,"####################")
renderLine(01,"#  z               #")
renderLine(02,"#                  #")
renderLine(03,"#   ''        ''   #")
renderLine(04,"#   ''        ''   #")
renderLine(05,"#                  #")
renderLine(06,"#       ''''       #")
renderLine(07,"#                  #")
renderLine(08,"#   ####====####   #")
renderLine(09,"#      #====#      #")
renderLine(10,"#    o #====# o    #")
renderLine(11,"#     r#====#r     #")
renderLine(12,"########====########")
renderLine(13,"#     r#====#r     #")
renderLine(14,"#    o #====# o    #")
renderLine(15,"#      #====#      #")
renderLine(16,"#   ####====####   #")
renderLine(17,"#                  #")
renderLine(18,"#       ''''       #")
renderLine(19,"#                  #")
renderLine(20,"#   ..        ..   #")
renderLine(21,"#   ..        ..   #")
renderLine(22,"#                  #")
renderLine(23,"#                  #")
renderLine(24,"####################")

oxyd_shuffle()






























