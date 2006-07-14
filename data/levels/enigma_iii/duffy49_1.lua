-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Sacrifice

levelw=80
levelh=20

create_world( levelw, levelh)

fill_floor("fl-leaves")

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "w" then
            set_stone( "st-glass", i-1, line)
        elseif c == "!" then
            abyss(i-1,line)
            fill_floor("fl-water", i-1,line, 1,1)
        elseif c=="z" then
            set_actor("ac-blackball", i-.5,line+.5)
        elseif c=="+" then
            set_stone( "st-wood",i-1,line)
        elseif c=="=" then
            fill_floor("fl-space", i-1,line, 1,1)
        elseif c=="B" then
            set_stone("st-bombs", i-1,line)
        elseif c=="d" then
            set_stone("st-oneway-s", i-1,line)	
        elseif c=="l" then
            set_stone("st-oneway-w", i-1,line)
        elseif c=="s" then
            set_stone("st-stoneimpulse",i-1,line)
        elseif c=="S" then
            set_stone( "st-stoneimpulse-hollow", i-1,line)
        elseif c=="M" then
            mirrorp(i-1,line,FALSE,TRUE, 4)
        elseif c=="N" then
            mirrorp(i-1,line,TRUE,FALSE, 3)
        elseif c=="L" then
            set_attrib(laser(i-1,line, TRUE, EAST), "name", "laser")
        elseif c == "R" then
            set_stone("st-bolder-e", i-1,line)
        elseif c == "a" then
            set_stone( "st-switch", i-1, line, 
                      {target="start_ferry", action="callback"})
        end
    end	
end
-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"####################################################ooo###")
renderLine(01,"#a         w  w        ====== = = =   w   w  w   w w   wo#")
renderLine(02,"#      w   w   ww ww   ======= = = =  w w  w   w w w   ww#")
renderLine(03,"#      ww ww w     w   ====== = = =      w   w w   w     o")
renderLine(04,"#z           ww  w     ======= = = =  w    w   w w w     o")
renderLine(05,"#     w   w      w w   ====== = = =   w w w  w   w N     o")
renderLine(06,"#         w  ww  w     ======= = = =  w    w   w w w     o")
renderLine(07,"#               w  w   ====== = = =      w   w w   w     o")
renderLine(08,"#      ww    w    ww   ======= = = =  w w  w   w w w     o")
renderLine(09,"#       w    w     w   ====== = = =   w   w  w   w w     o")
renderLine(10,"#wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww###o #")
renderLine(11,"#L  MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMw #")
renderLine(12,"##########################################################")

oxyd_shuffle()

ferry_started = nil
ferry_x = 2
ferry_y = 10
ferry_width = 1
ferry_end = 54

function move_ferry()
set_stone("st-greenbrown",ferry_x+ferry_width-2,ferry_y)
set_stone("st-greenbrown",ferry_x+ferry_width-1,ferry_y)
set_stone("st-greenbrown",ferry_x+ferry_width,ferry_y)
if ferry_x>2 then
set_stone("st-glass",ferry_x+ferry_width-3, ferry_y)
end
    ferry_x = ferry_x + 1
end

function timer_callback()
   if ferry_x==ferry_end then
ferry_x=2
end
    if ferry_started and ferry_x < ferry_end then
        move_ferry()
    end
end

function start_ferry()
    if not ferry_started then
        move_ferry()
        ferry_started = 1
    end
 end

set_stone( "st-timer", 58,0 ,{target="timer_callback", action="callback",name="timer", interval=.6})

enigma.ConserveLevel=FALSE















