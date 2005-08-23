-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Bumperball

enigma.ConserveLevel=FALSE                  -- \dh\
levelw=200
levelh=60

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
        elseif c == "a" then
            set_item("it-trigger", i-1,line, {action="openclose",target="door1"})
        elseif c == "A" then
            set_stone("st-door", i-1, line , {name="door1", type="v"})
        elseif c == "S" then
            set_stone("st-swap", i-1,line)
        elseif c == "+" then
            fill_floor("fl-bluegreen", i-1,line, 1,1)
        elseif c == "D" then --1-d
            set_floor("fl-gradient",  i-1,  line, {type=1})
        elseif c == "U" then --2-u
            set_floor("fl-gradient",  i-1,  line, {type=2})
        elseif c == "R" then --3-r
            set_floor("fl-gradient",  i-1,  line, {type=3})
        elseif c == "L" then --4-l
            set_floor("fl-gradient",  i-1,  line, {type=4})
        elseif c == "1" then --ur
            set_floor("fl-gradient",  i-1,  line, {type=11})
        elseif c == "3" then --dl
            set_floor("fl-gradient",  i-1,  line, {type=9})
        elseif c == "7" then --dr
            set_floor("fl-gradient",  i-1,  line, {type=12})
        elseif c == "9" then --ul
            set_floor("fl-gradient",  i-1,  line, {type=10})
        elseif c=="0" then
            set_floor("fl-gradient", i-1,line , {type=23})
        elseif c=="5" then
            set_floor("fl-gradient", i-1,line , {type=24})
        elseif c=="b" then
            puzzle(i-1,line,PUZ_0110)
        elseif c=="c" then
            puzzle(i-1,line,PUZ_0101)
        elseif c=="d" then
            puzzle(i-1,line,PUZ_0011)
        elseif c=="e" then
            puzzle(i-1,line,PUZ_1010)
        elseif c=="f" then
            puzzle(i-1,line,PUZ_1001)
        elseif c=="g" then
            puzzle(i-1,line,PUZ_1100)

        end
    end
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"##########################################################")
renderLine(01,"#!!!!!+!!!!++++++++++++++!!!!!!!!!!!!!!!!!!!!!!!!!#!!!!!!#")
renderLine(02,"#+++++++++!+!!!!+!!!!!!!+!!++++++!!#!!+!!!!!!!!!!!!!!!!!!#")
renderLine(03,"#!+!!!+!+!!++++!!!+++++++++!+!o!+++!!!+!!++++++++++++++!!#")
renderLine(04,"#!+!+!!!+!!+!!+++!!+!+!!!!!!+!!!+!++++++++!!!+!!!!!!!!+!!#")
renderLine(05,"#!+!+++++++++!!!++++!++++++++++++!!!+#!!+!!!!+!!+++++!+!!#")
renderLine(06,"#!+!!!!!!!!!!!!!!!!!!!!!!!!!!!!!+++++!!+++!o!+!!+!!!+++!!#")
renderLine(07,"#!+!!!!!!!!!!!!!!!!!!!!!!!!!!!!!+!!!+!!+!++++++++!!!+!+!!#")
renderLine(08,"#!+!!!!!!!!!!!!!!!!!!!!!!!!!!!!!++++++++++!!!#!!+!!#+!+!!#")
renderLine(09,"#                             #!!!!!!!!!!++++++++++!+!+!!#")
renderLine(10,"#                                    !!!!!!!!!!+!!+!!!!!!#")
renderLine(11,"#                                #   #     !!!+++!+!!!!!!#")
renderLine(12,"#                          o               !!!+!!!+!!!!!!#")
renderLine(13,"#                                    o     !!!+!!!+!!!!!!#")
renderLine(14,"#                                          !!!+!#!+!!!!!!#")
renderLine(15,"#                     o                                  #")
renderLine(16,"#                                                        #")
renderLine(17,"#                        bcccd           o               #")
renderLine(18,"#                   o    e   e                           #")
renderLine(19,"#                        e   e                   #       #")
renderLine(20,"#                        e   e             o             #")
renderLine(21,"#              o         gcccf    o                   #  #")
renderLine(22,"#                                                        #")
renderLine(23,"#                                                        #")
renderLine(24,"#                    o          o                        #")
renderLine(25,"#                o                        #    #         #")
renderLine(26,"#                         o          o                   #")
renderLine(27,"#                                                        #")
renderLine(28,"#                                                  #     #")
renderLine(29,"#                                                        #")
renderLine(30,"#                                                #       #")
renderLine(31,"#                             #            #             #")
renderLine(32,"#                 ############     #                     #")
renderLine(33,"#                 ############                           #")
renderLine(34,"####S#            ############                         ###")
renderLine(35,"#    #             !      a###                         Ao#")
renderLine(36,"##########################################################")

oxyd_shuffle()

set_actor("ac-blackball", 27.5,19.5)
set_actor("ac-whiteball-small",1.5,35.5, {controllers=1})
--set_actor("ac-blackball", 50.5,17.5)
--set_actor("ac-blackball", 4.5,30.5)
