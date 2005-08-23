-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Just Moving Blocks?

levelw=80
levelh=20

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)
enigma.ConserveLevel=FALSE
enigma.SetCompatibility("oxyd1")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-glass", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
            --			fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
        elseif c=="W" then
            set_attrib(laser(i-1,line, FALSE, WEST), "name", "laser")
        elseif c=="L" then
            set_stone("st-key_a", i-1,line, {action="on", target="laser"})
        elseif c=="k" then
            set_item("it-key_a", i-1,line)
        elseif c=="`" then
            mirrorp(i-1,line,FALSE,TRUE, 2)
        elseif c=="/" then
            mirrorp(i-1,line,FALSE,TRUE, 4)
        elseif c=="B" then
            set_stone("st-bombs", i-1,line)
        elseif c=="d" then
            set_stone("st-oneway-s", i-1,line)	
        elseif c=="l" then
            set_stone("st-oneway-w", i-1,line)
        elseif c=="F" then	
            set_item("it-floppy", i-1,line)
        elseif c=="c" then	
            set_item("it-coin2", i-1,line)
        elseif c=="5" then	
            set_item("it-odometer", i-1,line)
        elseif c=="4" then	
            set_item("it-coin4", i-1,line,{value=3})
        elseif c=="P" then
            set_item("it-wrench",i-1,line)
        elseif c=="h" then
            hammer(i-1,line)
        elseif c=="H" then
            set_stone("st-stone_break", i-1,line)
        elseif c=="f" then
            set_stone( "st-floppy", i-1,line,{action="openclose",target="unimportantdoor"})
        elseif c=="A" then
            doorv( i-1,line, {name="door1"})
        elseif c=="b" then
            doorv( i-1,line, {name="door2"})
        elseif c=="C" then
            --			doorh( i-1,line, {name="door3"})
            set_floor("fl-bridge", i-1,line, {name="bridge1", type="yy"})
        elseif c=="D" then
            set_floor("fl-bridge", i-1,line, {name="bridge2", type="yy"})
        elseif c=="1" then
            set_stone("st-switch", i-1,line, {action="callback",target="funcc"})
        elseif c=="2" then
            doorh( i-1,line, {name="unimportantdoor"})
        elseif c=="9" then
            set_stone("st-coinslot", i-1,line, {action="openclose", target="unimportantdoor"})
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"####################B###9###fL###9###1####################")
renderLine(01,"#/                 #                  #                 `#")
renderLine(02,"#/                 #P   z        k   F#                 `#")
renderLine(03,"#/                 #                  #                 `#")
renderLine(04,"#                  ####d####CD####d####                  #")
renderLine(05,"#                  #       #  #       #       #2#        #")
renderLine(06,"#                  #+++++++#ch#+++++++#                  #")
renderLine(07,"#                  #       #  #       #                  #")
renderLine(08,"#                  #+++++++#  #+++++++#                  #")
renderLine(09,"#                  #   H   #  #   H   #                  #")
renderLine(10,"#                  ###++####  ####++###                  #")
renderLine(11,"#`                 # A 4          5 b #                //#")
renderLine(12,"####################o################o####################")
renderLine(13,"#`                                                     `W#")

oxyd_shuffle()

SendMessage("door1", "openclose")
SendMessage("door2", "openclose")

through=0
function funcc()
    through = 1-through
    if through==0 then
	SendMessage("door1", "openclose")
	SendMessage("door2", "openclose")
--	SendMessage("door3", "openclose")
	SendMessage("bridge1", "openclose")
	SendMessage("bridge2", "openclose")
    end
end