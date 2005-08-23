-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Two Billiard Tables

levelw=80
levelh=20

create_world( levelw, levelh)

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
            --			fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5)
            set_item("it-spring1",i-1,line)
        elseif c=="w" then
            set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
        elseif c=="+" then
            set_stone( "st-wood",i-1,line)
        elseif c == "s" then
            set_stone("st-stoneimpulse",i-1,line)
        elseif c == "S" then
            set_stone("st-stoneimpulse-hollow",i-1,line)
        elseif c=="=" then
            fill_floor("fl-space", i-1,line, 1,1)
        elseif c=="M" then
            set_item( "it-magnet-on",i-1 ,line , {name="magnet1"})
        elseif c=="N" then
            set_item( "it-magnet-on",i-1 ,line , {name="magnet2"})
        elseif c=="$" then
            set_item( "it-hollow", i-1,line, {name="h1"})
        elseif c=="%" then
            set_item( "it-hollow", i-1,line, {name="h2"})
        elseif c=="^" then
            set_item( "it-hollow", i-1,line, {name="h3"})
        elseif c=="&" then
            set_item( "it-hollow", i-1,line, {name="h4"})
        elseif c=="*" then
            set_item( "it-hollow", i-1,line, {name="h5"})
        elseif c=="Q" then
            set_item( "it-hollow", i-1,line, {name="h6"})
        elseif c=="E" then
            set_item( "it-hollow", i-1,line, {name="h7"})
        elseif c=="R" then
            set_item( "it-hollow", i-1,line, {name="h8"})
        elseif c=="4" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh1"})
        elseif c=="5" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh2"})
        elseif c=="6" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh3"})
        elseif c=="7" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh4"})
        elseif c=="8" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh5"})
        elseif c=="q" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh6"})
        elseif c=="e" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh7"})
        elseif c=="r" then
            set_stone("st-switch", i-1,line, {action="callback",target="hh8"})
	end
    end	
end
-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"!!!!!!!!!!!!!!!!!!!!")
renderLine(01,"####4####!!####e####")
renderLine(02,"#R     *#!!#&     $#")
renderLine(03,"#       #!!#       #")
renderLine(04,"#       #!!#       #")
renderLine(05,"#   w   #!!#   w   #")
renderLine(06,"7  wMw  5!!q  wNw  r")
renderLine(07,"#   w   #!!#   w   #")
renderLine(08,"#       #!!#       #")
renderLine(09,"#       #!!#       #")
renderLine(10,"#E     Q#!!#^     %#")
renderLine(11,"####6####!!####8####")
renderLine(12,"!!!!!!!!!!!!!!!!!!!!")

oxyd_shuffle()

SendMessage("h1", "trigger")
SendMessage("h2", "trigger")
SendMessage("h3", "trigger")
SendMessage("h4", "trigger")
SendMessage("h5", "trigger")
SendMessage("h6", "trigger")
SendMessage("h7", "trigger")
SendMessage("h8", "trigger")

function hh1()
    SendMessage("h1", "trigger")
end

function hh2()
    SendMessage("h2", "trigger")
end

function hh3()
    SendMessage("h3", "trigger")
end

function hh4()
    SendMessage("h4", "trigger")
end

function hh5()
    SendMessage("h5", "trigger")
end

function hh6()
    SendMessage("h6", "trigger")
end

function hh7()
    SendMessage("h7", "trigger")
end

function hh8()
    SendMessage("h8", "trigger")
end
