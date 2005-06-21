-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Shrinking Pits

levelw=50
levelh=30

create_world( levelw, levelh)

fill_floor("fl-bluegreen", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "!" then
            --			abyss(i-1,line)
            fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
        elseif c == "r" then
            set_stone("st-bolder-e", i-1,line)
        elseif c=="g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c=="l" then
            oneway(i-1,line, enigma.WEST)
        elseif c=="w" then
            set_actor("ac-whiteball-small", i-1,line, {player=0})
        elseif c=="H" then
            hollow(i-1,line)

        elseif c=="$" then
            hollow(i-1,line)
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"#######################################")
renderLine(01,"# gggg#gggggg#gggg l                  #")
renderLine(02,"# gHg +  $Hg +  Hg l  w   w   w   w   #") 
renderLine(03,"# g gg#ggg gg#gg g l                  #")
renderLine(04,"###+######+#####+###                  #")
renderLine(05,"#gg gg#ggg gg#gg g l    w   w   w     #")
renderLine(06,"# g$g +  $H  + gHg l                  #")
renderLine(07,"# gggg#g gggg#gg g l                  #")
renderLine(08,"#+######+#######+###      w   w       #")
renderLine(09,"#     #g gggg#gg g l                  #")
renderLine(10,"#g $gg#ggH$  + gHg l w              w #")
renderLine(11,"#g g  + ggggg#gggg l        w         #")
renderLine(12,"#######################################")

oxyd_shuffle()