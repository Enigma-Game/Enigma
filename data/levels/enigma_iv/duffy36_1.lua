-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: It Never Stops

levelw=50
levelh=30

create_world( levelw, levelh)

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
            --	           	fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5)
        elseif c=="s" then
            set_stone( "st-stoneimpulse", i-1, line)
        elseif c=="S" then
            set_stone( "st-stoneimpulse-hollow", i-1,line)
	elseif c=="A" then
            set_stone( "st-stoneimpulse-hollow", i-1,line,{name="ss1"})
	elseif c=="B" then
            set_stone( "st-stoneimpulse-hollow", i-1,line,{name="ss2"})
	elseif c=="C" then
            set_stone( "st-stoneimpulse-hollow", i-1,line,{name="ss3"})
	elseif c=="D" then
            set_stone( "st-stoneimpulse-hollow", i-1,line,{name="ss4"})
        elseif c=="a" then
            set_stone("st-stoneimpulse",i-1,line,{name="s1"})
        elseif c=="b" then
            set_stone("st-stoneimpulse",i-1,line,{name="s2"})
        elseif c=="c" then
            set_stone("st-stoneimpulse",i-1,line,{name="s3"})
        elseif c=="d" then
            set_stone("st-stoneimpulse",i-1,line,{name="s4"})
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"!!!!!###########!!!!")
renderLine(01,"!!!!##o       o##!!!")
renderLine(02,"!!!!#o         o#!!!")
renderLine(03,"!!!!#  c sB sb  #!!!")
renderLine(04,"!!!!#  s#o o#   #!!!")
renderLine(05,"!!!!#   o   os  #!!!")
renderLine(06,"!!!!#  C     A  #!!!")
renderLine(07,"!!!!#  so   o   #!!!")
renderLine(08,"!!!!#   #o o#s  #!!!")
renderLine(09,"!!!!#  ds Ds a  #!!!")
renderLine(10,"!!!!#o         o#!!!")
renderLine(11,"!!!!##o   z   o##!!!")
renderLine(12,"!!!!!###########!!!!")

oxyd_shuffle()

SendMessage("s1", "trigger")
SendMessage("s2", "trigger")
SendMessage("s3", "trigger")
SendMessage("s4", "trigger")

SendMessage("ss1", "trigger")
SendMessage("ss2", "trigger")
SendMessage("ss3", "trigger")
SendMessage("ss4", "trigger")

mytimer = 0
function timer_handler()
    if mytimer==0 then
        set_stone("st-stoneimpulse", 8,3)
        set_stone("st-stoneimpulse", 13,4)
        set_stone("st-stoneimpulse", 7,8)
        set_stone("st-stoneimpulse", 12,9)
        set_stone("st-stoneimpulse", 11,3)
        set_stone("st-stoneimpulse", 9,9)
        set_stone("st-stoneimpulse", 13,7)
        set_stone("st-stoneimpulse", 7,5)
        mytimer=1
    end
end

set_stone("st-timer", 21, 0, {action="callback", target="timer_handler",interval=.3})















