-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Inverted Laser Spiral

levelw = 20
levelh = 13

create_world(levelw, levelh)

draw_border("st-greenbrown")
fill_floor("fl-hay")


function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone("st-greenbrown",i-1,line)
        elseif c == "z" then	
            set_actor("ac-blackball", i-.5,line+1)
        elseif c == "o" then
            oxyd(i-1,line)
        elseif c == "@" then	
            hollow(i-1,line)
        elseif c == "/" then
            mirrorp(i-1,line,FALSE,FALSE, 4)
        elseif c == ";" then
            mirrorp(i-1,line,FALSE,FALSE, 2)
        elseif c == "?" then
            mirrorp(i-1,line,TRUE,TRUE, 1)
        elseif c == "x" then	
            set_item("it-key_a", i-1 ,line)
        elseif c == "u" then
            set_attrib(laser(i-1,line, TRUE, NORTH), "name", "laser")
        elseif c == "w" then
            draw_stones("st-glass", {i-1,line},{1,0}, 1)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        end
    end
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"


renderLine(00,"####################")
renderLine(01,"###gg/            ;#")
renderLine(02,"#ow g /          ; #")
renderLine(03,"#ow g  /        ;  #")
renderLine(04,"#ow?g   /      ;   #")
renderLine(05,"#ow g    /    ;    #")
renderLine(06,"#owgggggggw  ;     #")
renderLine(07,"#owz ?   ;   /     #")
renderLine(08,"#owggggg;     /    #")
renderLine(09,"#ow    ;       /   #")
renderLine(10,"#ow   ;         /  #")
renderLine(11,"#ow  ;           / #")
renderLine(12,"##################u#")

oxyd_shuffle()

