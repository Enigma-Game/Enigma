-- twisting by the pool, a meditation level for Enigma
-- Copyright (C) 2006 Alain Busser
-- Licensed under GPL v2.0 or above 

levelw = 20
levelh = 13

create_world(levelw, levelh)
oxyd_default_flavor = "a"
draw_border("st-actorimpulse")
fill_floor("fl-leaves", 0,0, level_width,level_height)
enigma.BumperForce=600
enigma.SlopeForce=8

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone("st-white4", i-1,line)
        elseif c == "o" then
            set_item("it-hollow", i-1, line)
        elseif c == "1" then
                set_floor("fl-gradient",  i-1,  line, {type=21})
        elseif c == "2" then
                set_floor("fl-gradient",  i-1,  line, {type=22})
        elseif c == "3" then
                set_stone("st-white4",i-1,line)
        elseif c == "4" then
                set_floor("fl-gradient",  i-1,  line, {type=24})
        elseif c=="*" then
            set_actor("ac-whiteball-small", i-0.5, line+0.5, {player=1, mouseforce=0})
        end
    end    
end

--               01234567890123456789
renderLine(00 , "                    ")
renderLine(01 , "               4444 ")
renderLine(02 , " 33333333333    4o4 ")
renderLine(03 , "           3    444 ")
renderLine(04 , "         * 3    444 ")
renderLine(05 , "           3    444 ")
renderLine(06 , " o*      * 3    o44 ")  
renderLine(07 , "           3    444 ")
renderLine(08 , "         * 3    444 ")
renderLine(09 , "           3    444 ")
renderLine(10 , " 33333333333    4o4 ")
renderLine(11 , "               4444 ") 
--               01234567890123456789

ac1 = set_actor("ac-blackball", 3.5, 6.5)
ac2 = set_actor("ac-whiteball", 8.5, 6.5)
AddRubberBand (ac1, ac2, 8000, 5, 5)

oxyd_shuffle()
