-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: If They'd Just Hold Still

levelw=50
levelh=30

create_world( levelw, levelh)

fill_floor("fl-metal", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
	elseif c == "D" then
            set_stone( "st-death", i-1, line)
        elseif c == "!" then
            --			abyss(i-1,line)
            fill_floor("fl-water", i-1,line, 1,1)
        elseif c == " " then
            abyss(i-1,line)
        elseif c=="w" then
            set_actor("ac-whiteball-small", i-.5,line+.5, {player=0, mouseforce=1})
        elseif c=="H" then
            hollow(i-1,line)
        elseif c=="*" then
            set_stone("st-greenbrown",i-1,line)
        elseif c=="r" then
            set_floor("fl-gradient", i-1,line , {type=23})
        elseif c=="l" then
            set_floor("fl-gradient", i-1,line , {type=24})

        elseif c=="R" then
            set_floor("fl-gradient", i-1,line , {type=23})
            set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
        elseif c=="L" then
            set_floor("fl-gradient", i-1,line , {type=24})
            set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
        elseif c=="x" then
            set_floor("fl-metal", i-1,line)
        elseif c=="X" then
            set_floor("fl-metal", i-1,line)
            set_actor("ac-whiteball-small", i-.5,line+.5, {player=0})
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"
 
renderLine(00,"   ###    ###       ")
renderLine(01," ###r#o####r#o##### ")
renderLine(02," DrrrrrrrrRrrrrrrr# ")
renderLine(03," ##r##o######o##r## ")
renderLine(04,"  ###    ###   ###  ")
renderLine(05," ##x##o###x##o##### ")
renderLine(06," DxxxxxxxxxxxxxxxxD ")
renderLine(07," #####o###x##o##x## ")
renderLine(08,"  ###    ###   ###  ")
renderLine(09," ##l##o###l##o##### ")
renderLine(10," #llllllllLlllllllD ")
renderLine(11," #####o#l####o##l## ")
renderLine(12,"       ###     ###  ")

oxyd_shuffle()

set_actor("ac-whiteball-small", 10.5,6.5, {player=0})
