-- Copyright (c) 2005 Xerxes M. Dynatos
-- License: GPL v2.0 or above
-- Enigma Level: KillStones/Items

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-metal2", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-bigbrick", i-1, line)

		elseif c == "o" then
			--oxyd( i-1, line)
			oxyd1=set_stone("st-oxyd", i-1, line, {name="oxyd1"})

		elseif c == "O" then
			--oxyd( i-1, line)
			oxyd2=set_stone("st-oxyd", i-1, line, {name="oxyd2"})

		elseif c == "z" then
			set_actor("ac-blackball", i-.5,line+.5)

		elseif c == "G" then
			set_stone("st-glass", i-1, line)

		elseif c == "1" then
			sw1=set_stone("st-switch", i-1, line, {action="callback",target="func1",name="s1"})

		elseif c == "2" then
			sw2=set_stone("st-switch", i-1, line, {action="callback",target="func2",name="s2"})

		elseif c == "3" then
			sw3=set_stone("st-switch", i-1, line, {action="callback",target="func3",name="s3"})

		elseif c == "4" then
			sw4=set_stone("st-switch", i-1, line, {action="callback",target="func4",name="s4"})

		elseif c == "M" then
			set_item("it-magicwand",i-.5,line+.5)

		end
	end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o" and "O"
--             12345678901234567890
renderLine(00,"####################")
renderLine(01,"#                  #")
renderLine(02,"#                  #")
renderLine(03,"#        z         #")
renderLine(04,"#     2#####1      #")
renderLine(05,"#      #GGo#       #")
renderLine(06,"#      #GMG#       #")
renderLine(07,"#      #OGG#       #")
renderLine(08,"#     3#####4      #")
renderLine(09,"#                  #")
renderLine(10,"#                  #")
renderLine(11,"#                  #")
renderLine(12,"####################")

oxyd_shuffle()
enigma.KillStone(10,5)
enigma.KillItem(9,6)

function func1()
local a = GetAttrib(sw1,"on")
if a==1 then
enigma.KillStone(10,5)
elseif a==0 then
--oxyd(10,5)
oxyd1=set_stone("st-oxyd", 10, 5, {name="oxyd1"})
local a = GetAttrib(sw4,"on")
if a==1 then
SendMessage(oxyd1,"trigger")
end
end
end

function func2()
local a = GetAttrib(sw2,"on")
if a==1 then
enigma.KillItem(9,6)
elseif a==0 then
set_item("it-magicwand",9,6)
end
end

function func3()
local a = GetAttrib(sw3,"on")
if a==0 then
SendMessage(oxyd2,"trigger")
elseif a==1 then
enigma.KillStone(8,7)
oxyd2=set_stone("st-oxyd", 8, 7, {name="oxyd2"})
end
end

function func4()
local a = GetAttrib(sw1,"on")
local b = GetAttrib(sw4,"on")
if b==0 then
if a==1 then
SendMessage(oxyd1,"trigger")
elseif a==0 then
end
elseif b==1 then
if a==1 then
enigma.KillStone(10,5)
oxyd1=set_stone("st-oxyd", 10, 5, {name="oxyd1"})
end
end
end

















