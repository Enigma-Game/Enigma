-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Family Business

levelw=25
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
        elseif c == "!" then
            abyss(i-1,line)
            --			fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="+" then
            set_stone( "st-wood", i-1, line)
        elseif c=="g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c=="l" then
            oneway(i-1,line, enigma.WEST)
        elseif c=="u" then
            oneway(i-1,line, enigma.NORTH)
        elseif c=="r" then
            oneway(i-1,line, enigma.EAST)
        elseif c=="d" then
            oneway(i-1,line, enigma.SOUTH)
        elseif c=="h" then
            set_item("it-tinyhollow", i-1, line)
        elseif c=="H" then
            set_item("it-hollow", i-1, line)
        elseif c=="J" then
            set_item("it-hill", i-1, line)
        elseif c=="j" then
            set_item("it-tinyhill", i-1, line)
        elseif c == "1" then --ur
            set_floor("fl-gradient",  i-1,  line, {type=11})
        elseif c == "3" then --dl
            set_floor("fl-gradient",  i-1,  line, {type=9})
        elseif c == "7" then --dr
            set_floor("fl-gradient",  i-1,  line, {type=12})
        elseif c == "9" then --ul
            set_floor("fl-gradient",  i-1,  line, {type=10})
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"####################")
renderLine(01,"#o                o#")
renderLine(02,"# J              J #")
renderLine(03,"#  ##############  #")
renderLine(04,"#  #H   o  o   H#  #")
renderLine(05,"#  #            #  #")
renderLine(06,"#  #  o  31  o  #  #")
renderLine(07,"#  #     97     #  #")
renderLine(08,"#  #            #  #")
renderLine(09,"#  #   ######   #  #")
renderLine(10,"#  #   #h  h#   #  #")
renderLine(11,"#JJ#   # oo #   #JJ#")
renderLine(12,"#JJ#   # jj #   #JJ#")
renderLine(13,"#JJ#   # oo #   #JJ#")
renderLine(14,"#  #   #h  h#   #  #")
renderLine(15,"#  #   ######   #  #")
renderLine(16,"#  #            #  #")
renderLine(17,"#  #     31     #  #")
renderLine(18,"#  #  o  97  o  #  #")
renderLine(19,"#  #            #  #")
renderLine(20,"#  #H   o  o   H#  #")
renderLine(21,"#  ##############  #")
renderLine(22,"# J              J #")
renderLine(23,"#o                o#")
renderLine(24,"####################")

oxyd_shuffle()


local actor1=set_actor("ac-blackball", 10,2)
local actor2=set_actor("ac-whiteball", 10,5)
local actor3=set_actor("ac-whiteball-small", 10,10.5,{player=1,mouseforce=0})

AddRubberBand(actor1, actor2, 8,0)
AddRubberBand(actor2, actor3, 8,0)