-- Koosnab, a level for Enigma
-- Copyright (C) 2005 Roberto Bardin
-- Licensed under GPL v2.0

levelw = 20
levelh = 13

create_world( levelw, levelh)
oxyd_default_flavor="a"
fill_floor("fl-space", 0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-rock7", i-1, line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c == "-" then
            set_stone("st-glass1", i-1, line)
        elseif c == "." then
            set_floor("fl-normal", i-1, line)
        elseif c == ":" then
            set_floor("fl-bluegray", i-1, line)
        elseif c == "s" then
            set_floor("fl-red", i-1, line)
        elseif c == "m" then
            set_floor("fl-normal", i-1, line)
            set_stone( "st-wood-growing", i-1, line)
        end
    end
end

renderLine(  0, "   o             -  ");
renderLine(  1, "  #:########    -   ");
renderLine(  2, "  #...##...#     -  ");
renderLine(  3, "  #.#.##.m.#      - ");
renderLine(  4, "  #........:o      -");
renderLine(  5, "  ###.##.###      - ");
renderLine(  6, "  ###.##.###     -  ");
renderLine(  7, "  #....ssss#    -   ");
renderLine(  8, "  #.m.##.#.#   -    ");
renderLine(  9, " o:...##...#  -     ");
renderLine( 10, "  ########:#   -    ");
renderLine( 11, "          o     -   ");
renderLine( 12, "               -    ");

set_stone("st-door", 11, 4, {name="door1", type="v"})
set_stone("st-door", 3, 1, {name="door2", type="h"})
set_stone("st-door", 2, 9, {name="door3", type="v"})
set_stone("st-door", 10, 10, {name="door4", type="h"})

set_item("it-trigger", 7,7, {action="openclose", target="door3"})
set_item("it-trigger", 8,7, {action="openclose", target="door4"})
set_item("it-trigger", 9,7, {action="openclose", target="door2"})
set_item("it-trigger",10,7, {action="openclose", target="door1"})

ac1=set_actor("ac-blackball", 3.5, 3.5)

oxyd_shuffle()























