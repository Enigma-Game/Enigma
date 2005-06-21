-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Crossfire

levelw=40
levelh=13

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE
enigma.TwoPlayerGame = TRUE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c =="G" then
            set_stone( "st-glass", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "*" then
            set_stone( "st-brownie", i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
            --			fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5)
            set_item("it-yinyang",i-1,line)
        elseif c=="y" then
            set_actor("ac-whiteball", i-.5,line+.5)
            set_item("it-yinyang",i-1,line)
        elseif c=="u" then
            set_item("it-yinyang",i-1,line)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
        elseif c=="=" then
            set_floor("fl-space",i-1,line)
        elseif c=="R" then
            set_stone("st-oneway-e", i-1,line)
            set_floor("fl-space",i-1,line)
        elseif c=="S" then
            set_attrib(laser(i-1,line, TRUE, SOUTH), "name", "laser")
        elseif c=="N" then
            set_attrib(laser(i-1,line, TRUE, NORTH), "name", "laser")
        elseif c=="a" then
            set_item("it-trigger",i-1,line,{action="onoff",target="laser1"})
        elseif c=="b" then
            set_item("it-trigger",i-1,line,{action="onoff",target="laser2"})
        elseif c=="A" then
            set_attrib(laser(i-1,line, FALSE, NORTH), "name", "laser1")
        elseif c=="B" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser2")
        elseif c=="6" then
            puzzle(i-1,line,PUZ_0101)
        elseif c=="4" then
            puzzle(i-1,line,PUZ_0100)
        elseif c=="T" then
            set_stone( "st-thief", i-1, line)
            set_floor("fl-space",i-1,line)
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"!!!!!!!!TTTTTTT!!!!#####SSSSSSSSSSSS#o#")
renderLine(01,"!#####!!T=====T!!!!#u                 #")
renderLine(02,"!#   #TTT=TTT=T!TTT#  46              #")
renderLine(03,"!# z =====T!T=T!T==R #46              #")
renderLine(04,"!#   #TTTTT!T=TTT=T#  46              #")
renderLine(05,"!#####!!!!!!T=====T#  4646            #")
renderLine(06,"!!!!!!!!!!!!TTTTTTT#  46#             #")
renderLine(07,"!#####!!!!!!T=====T#  4646            #")
renderLine(08,"!#   #TTTTT!T=TTT=T#  46              #")
renderLine(09,"!# y =====T!T=T!T==R #46              #")
renderLine(10,"!#   #TTT=TTT=T!TTT#  46              #")
renderLine(11,"!#####!!T=====T!!!!#u                 #")
renderLine(12,"!!!!!!!!TTTTTTT!!!!#####NNNNNNNNNNNN#o#")

oxyd_shuffle()
