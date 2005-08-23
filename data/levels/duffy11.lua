-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: The Race

levelw=58
levelh=30
enigma.ConserveLevel = FALSE		-- dh

create_world( levelw, levelh)
enigma.SetCompatibility("oxyd1")

fill_floor("fl-leaves", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "h" then
            set_item("it-trigger", i-1,line, {action="openclose",target="door1"})
        elseif c == "H" then
            set_stone("st-door", i-1, line , {name="door1", type="v"})
        elseif c == "+" then
            set_stone("st-wood", i-1, line)
        elseif c == "*" then
            set_stone("st-brownie", i-1, line)
        elseif c == "g" then
            draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
        elseif c == "a" then
            set_stone("st-bolder-n", i-1,line)
        elseif c == "b" then
            set_stone("st-bolder-e", i-1,line)
        elseif c == "c" then
            set_stone("st-bolder-w", i-1,line)
        elseif c == "d" then
            set_stone("st-bolder-s", i-1,line)
        elseif c == "1" then
            oneway(i-1,line, enigma.NORTH)
        elseif c == "4" then
            oneway(i-1,line, enigma.SOUTH)
        elseif c == "3" then
            oneway(i-1,line, enigma.WEST)
        elseif c == "2" then
            oneway(i-1,line, enigma.EAST)
        elseif c == "~" then
            fill_floor("fl-water", i-1,line, 1,1)
        elseif c == "!" then
            abyss(i-1,line)
        elseif c == "@" then
            document(i-1,line,"I'ts a race!")
        elseif c == "s" then
            set_stone("st-stoneimpulse",i-1,line)
        elseif c == "x" then
            set_item("it-blackbomb",i-1,line)
        elseif c == "y" then
            dynamite(i-1,line)
        elseif c == "W" then
            if not difficult then
                --			set_stone("st-wood", i-1, line)
            else
                set_stone("st-wood", i-1, line)
            end
        elseif c == "w" then
            if not difficult then
                --			fill_floor("fl-water", i-1,line, 1,1)
            else
                fill_floor("fl-water", i-1,line, 1,1)
            end
        end
    end
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00, "##########################################################")
renderLine(01, "#  sd ####################################################")
renderLine(02, "#b### ###############################################oo###")
renderLine(03, "#se        y                              @   z   c#    ##")
renderLine(04, "##### ############################################## #####")
renderLine(05, "##### ##################h#########################sd #####")
renderLine(06, "##### ################## ##########oooooooooooooo# g #####")
renderLine(07, "##### #####   W      w  * H +    !               2 #s#####")
renderLine(08, "##### ##### ############s######################### #c#####")
renderLine(09, "##### # ds         w                W  x+            #####")
renderLine(10, "##### # ### ##### #####################xxx######## #######")
renderLine(11, "##### #sd   as  # ################################ #######")
renderLine(12, "##### ### #####c# ################################ #######")
renderLine(13, "##### ###s#####s# ################################ #######")
renderLine(14, "##### ###b##### # ################################ #######")
renderLine(15, "##### ###    sa # ################################ #######")
renderLine(16, "##### ############################################ #######")
renderLine(17, "###                            +    ~                 s###")
renderLine(18, "###b# ############################################ ###c###")
renderLine(19, "###s# ######################                    sa #s  ###")
renderLine(20, "### # ######################b#######################c#####")
renderLine(21, "### as######################s                        #####")
renderLine(22, "##########################################################")

set_actor("ac-blackball", 47.5,3.5)	

set_stone("st-greenbrown",39,10)

oxyd_shuffle()

