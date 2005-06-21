-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Plan Ahead

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-rough-red", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-rock1", i-1, line)
        elseif c =="%" then
            set_stone( "st-rock1", i-1, line)
            set_floor("fl-water",i-1,line)
        elseif c == "O" then
            set_stone("st-oxyd", i-1, line,{color="0"})
        elseif c == "P" then
            set_stone("st-oxyd", i-1, line,{color="1"})
        elseif c == "Q" then
            set_stone("st-oxyd", i-1, line,{color="2"})
        elseif c == "`" then
            set_stone("st-oxyd", i-1, line,{color="3"})
        elseif c == "^" then
            set_stone("st-oxyd", i-1, line,{color="4"})
        elseif c == "." then
            if not difficult then
                set_stone("st-oxyd", i-1, line,{color="2"})
            else
                set_stone("st-oxyd", i, line,{color="2"})
                set_stone( "st-rock1", i-1, line)
            end
        elseif c == "*" then
            set_stone( "st-brownie", i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
        elseif c == "~" then
            set_floor("fl-water",i-1,line)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5, {player=0})
            set_item("it-yinyang",i-1,line)
        elseif c=="y" then
            if not difficult then
                set_actor("ac-whiteball", i-1,line+.5, {player=1})
            else
                set_actor("ac-whiteball", i-.5,line+.5, {player=1})
            end
        elseif c=="C" then
            set_item("it-crack3",i-1,line)
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c=="S" then
            set_item("it-seed",i-1,line)
        elseif c=="B" then
            set_item("it-bag",i-1,line)
        elseif c=="D" then
            set_item("it-dynamite",i-1,line)
        elseif c=="h" then
            set_item("it-hollow",i-1,line)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c == "G" then
            if difficult==false then
                --			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
            else
                draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
            end
        elseif c == "H" then
            set_stone("st-rock3_break",i-1,line)
        elseif c == "$" then
            set_stone("st-rock3",i-1,line)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
        elseif c=="=" then
            set_floor("fl-space",i-1,line)
        elseif c=="U" then
            set_stone("st-oneway", i-1,line, {orientation=enigma.NORTH})	
        elseif c=="R" then
            set_stone("st-oneway", i-1,line, {orientation=enigma.EAST})	
        elseif c=="D" then
            set_stone("st-oneway", i-1,line, {orientation=enigma.SOUTH})	
        elseif c=="L" then
            set_stone("st-oneway", i-1,line, {orientation=enigma.WEST})
        elseif c=="u" then
            set_stone("st-oneway_black", i-1,line, {orientation=enigma.NORTH})
        elseif c=="r" then
            set_stone("st-oneway_black", i-1,line, {orientation=enigma.EAST})
        elseif c=="d" then
            set_stone("st-oneway_black", i-1,line, {orientation=enigma.SOUTH})
        elseif c=="l" then
            set_stone("st-oneway_black", i-1,line, {orientation=enigma.WEST})
        elseif c=="w" then
            set_stone("st-oneway_white", i-1,line, {orientation=enigma.WEST})
        elseif c=="Z" then
            yy1( "black",  i-1, line)
        elseif c=="Y" then
            yy1( "white",  i-1, line)
            if not difficult then
                set_item("it-yinyang",i-1,line)
            end
        elseif c=="q" then
            yy1( "white",  i-1, line)
        end
    end	
end

function yy1( color, x, y)
    stone = format( "st-%s4", color)
    set_stone( stone, x, y)
end

renderLine(00,"####################")
renderLine(01,"###    ######   SZO#")
renderLine(02,"#Dw    lCCCCl   SZZ#")
renderLine(03,"### qqq######    ZZ#")
renderLine(04,"#O  qhH######Hh  ZP#")
renderLine(05,"#P  qhH~~~~~ZHh  ZZ#")
renderLine(06,"#. yqhH~~~~~ZHhBzSYQ")
renderLine(07,"#`  qhH~~~~~ZHh  ZZ#")
renderLine(08,"#^  qhH######Hh  Z`#")
renderLine(09,"### qqq######    ZZ#")
renderLine(10,"#Dl    rCCCCr   SZZ#")
renderLine(11,"###    ######   SZ^#")
renderLine(12,"####################")
