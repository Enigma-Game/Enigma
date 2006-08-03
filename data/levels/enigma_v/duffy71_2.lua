-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Don't Be Greedy

levelw=40
levelh=40

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone("st-greenbrown", i-1, line)
        elseif c =="G" then
            set_stone("st-glass", i-1, line)
        elseif c =="T" then
            set_stone("st-thief", i-1, line)
            set_floor("fl-space",i-1,line)
        elseif c =="M" then
            set_stone("st-knight", i-1, line)
        elseif c =="&" then
            set_stone("st-glass", i-1, line)
            set_item("it-tinyhollow",i-1,line)
        elseif c =="^" then
            set_stone("st-greenbrown", i-1, line)
            set_item("it-tinyhollow",i-1,line)
        elseif c =="$" then
            abyss(i-1,line)
            set_stone("st-greenbrown", i-1, line)
            set_item("it-tinyhollow",i-1,line)
        elseif c =="i" then
            set_item("it-tinyhollow",i-1,line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "A" then
            set_stone("st-oxyd",i-1,line,{color="0"})
        elseif c == "B" then
            set_stone("st-oxyd",i-1,line,{color="1"})
        elseif c == "C" then
            set_stone("st-oxyd",i-1,line,{color="2"})
        elseif c == "D" then
            set_stone("st-oxyd",i-1,line,{color="3"})
        elseif c == "E" then
            set_stone("st-oxyd",i-1,line,{color="4"})
        elseif c == "*" then
            set_stone("st-brownie", i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
            --			fill_floor("fl-water", i-1,line, 1,1)
        elseif c == "~" then
            --			abyss(i-1,line)
            fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="z" then
            set_actor("ac-blackball", i-1,line+.5)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c=="+" then
            set_stone("st-wood", i-1, line)
        elseif c=="=" then
            set_floor("fl-space",i-1,line)
        elseif c =="H" then
            set_stone("st-rock3_break", i-1, line)
        elseif c =="W" then
            set_stone("st-break_acwhite",i-1, line)
        elseif c =="O" then
            set_item("it-extralife",i-1,line)
        elseif c =="L" then
            set_stone("st-mail-w", i-1, line)
        elseif c =="R" then
            set_stone("st-mail-e", i-1, line)
        elseif c =="h" then
            set_item("it-pipe-h",i-1,line)
        elseif c =="I" then
            set_item("it-pipe-v",i-1,line)
        elseif c =="1" then
            set_item("it-pipe-ne",i-1,line)
        elseif c =="3" then
            set_item("it-pipe-wn",i-1,line)
        elseif c =="7" then
            set_item("it-pipe-es",i-1,line)
        elseif c =="9" then
            set_item("it-pipe-sw",i-1,line)
        elseif c =="@" then
            set_item("it-pipe-h",i-1,line)
            set_floor("fl-space",i-1,line)
        elseif c == "Q" then
            hammer(i-1,line)
        elseif c == "d" then
            dynamite(i-1,line)
        elseif c == "p" then
            set_attrib(laser(i-1,line, TRUE, SOUTH), "name", "laser1")
        elseif c == "q" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser2")
        elseif c == "r" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser3")
        elseif c == "s" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser4")
        elseif c == "t" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser5")
        elseif c == "u" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser6")
        elseif c == "v" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser7")
        elseif c == "w" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser8")
        elseif c == "x" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser9")
        elseif c == "`" then
            set_attrib(laser(i-1,line, FALSE, SOUTH), "name", "laser10")
        elseif c=="N" then
            set_stone("st-oneway-n", i-1,line)
        elseif c=="S" then
            set_stone("st-oneway-s", i-1,line)
        elseif c == "%" then
            abyss(i-1,line)
            set_item("it-blackbomb",i-1,line)
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
renderLine(01,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(02,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(03,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(04,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(05,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(06,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(07,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(08,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(09,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(10,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(11,"$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!$")
renderLine(12,"^#####################################^")
renderLine(13,"^d                 M                 Q^")
renderLine(14,"^                  #                  ^")
renderLine(15,"^                  #L                 ^")
renderLine(16,"^                  #                  ^")
renderLine(17,"^       T==T      R#                  ^")
renderLine(18,"^       T==T       #     TTTTTTT=T    ^")
renderLine(19,"^       T@@T       #     T=====T=T    ^")
renderLine(20,"^ TTTTTTT==TTTTTTT TTTTTTT=TTT=T=T    ^")
renderLine(21,"^ T==============T ========T T===T    ^")
renderLine(22,"^ T=TTTTTTTTTTTT=T TTTTTTTTT TTTTT    ^")
renderLine(23,"^ T=T!!!!!!!!!!T=TN#                  ^")
renderLine(24,"^HT=T##########T=TM#WWWWWWWWWWWWWWWWWW^")
renderLine(25,"^ST=Txw`vpqurtsT=T TTTTTTT            ^")
renderLine(26,"^                  ======T            ^")
renderLine(27,"^                  TTTTT=T            ^")
renderLine(28,"^                  #!!!T=T            ^")
renderLine(29,"GGGGGGGGGGGGGGGGGGG#!TTT=T            ^")
renderLine(30,"GGGGGGGGGABGGGGGGGG#!T===T   z       O^")
renderLine(31,"GGGGGGGEGGGGCGGGGGG#!T=TTTT=T         ^")
renderLine(32,"GGGGGGGGGGGGGGGGGGG#!T=T!!T=T         ^")
renderLine(33,"GGGGGDGGGGGGGGDGGGG#!T=TTTT=T         ^")
renderLine(34,"GGGGGGGGGGGGGGGGGGG#!T======T         ^")
renderLine(35,"GGGGGGCGGGGGGEGGGGG#!TTTTTTTT         ^")
renderLine(36,"GGGGGGGGBGGAGGGGGGG^^^^^^^^^^^^^^^^^^^^")

tt=0
function timer_callback()
    l1 = mod(tt+1, 10) + 1
    l2 = mod(tt, 10) + 1
    SendMessage("laser"..l1, "on")
    SendMessage("laser"..l2, "off")
    tt = mod (tt+1, 10)
end

set_stone("st-timer", 39,0, {loop=1, action="callback", target="timer_callback", interval=0.5})

















