-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Rush Hour Traffic

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "&" then
            set_floor("fl-water",i-1,line)
            oxyd( i-1, line)
        elseif c == "*" then
            set_stone( "st-brownie", i-1, line)
        elseif c == "%" then
            set_stone( "st-block", i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
        elseif c == "~" then
            set_floor("fl-water",i-1,line)
        elseif c=="z" then
            set_actor("ac-blackball", i-1,line+.5)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
        elseif c=="=" then
            set_floor("fl-space",i-1,line)
        elseif c=="L" then
            set_stone( "st-rotator-right", i-1, line)
        elseif c=="R" then
            set_stone( "st-rotator-left", i-1, line)
        elseif c=="l" then
            set_stone( "st-rotator_move-right", i-1, line)
        elseif c=="r" then
            set_stone( "st-rotator_move-left", i-1, line)
        elseif c=="$" then
            set_stone( "st-rotator-left", i-1, line)
            abyss(i-1,line)
        elseif c=="B" then
            set_stone( "st-blocker", i-1, line)
        elseif c=="I" then
            set_item("it-wrench",i-1,line)
        elseif c=="J" then
            set_item("it-weight",i-1,line)
        elseif c == "N" then
            set_stone("st-bolder-n", i-1,line)
        elseif c == "E" then
            set_stone("st-bolder-e", i-1,line)
        elseif c == "S" then
            set_stone("st-bolder-s", i-1,line)
        elseif c == "W" then
            set_stone("st-bolder-w", i-1,line)
        elseif c=="1" then
            set_stone("st-oneway-w", i-1,line)
        elseif c=="2" then
            set_stone("st-oneway-e", i-1,line)
        end
    end	
end

renderLine(00,"!!!#Ro###o###o#!!!!!")
renderLine(01,"!!!# E   E   SR!!!!!")
renderLine(02,"!!!o ######## #!!!!!")
renderLine(03,"!!!# #R###### ###!!!")
renderLine(04,"!!!#N# E   E   SR!!!")
renderLine(05,"!!!# # o####oS# #!!!")
renderLine(06,"!!!o # 1  z 2 # o!!!")
renderLine(07,"!!!# #No####o # #!!!")
renderLine(08,"!!!RN   W   W #S#!!!")
renderLine(09,"!!!### ######R# #!!!")
renderLine(10,"!!!!!# ######## o!!!")
renderLine(11,"!!!!!RN   W   W #!!!")
renderLine(12,"!!!!!#o###o###oR#!!!")

oxyd_shuffle()
