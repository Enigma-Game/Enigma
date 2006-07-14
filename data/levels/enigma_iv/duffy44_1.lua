-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Speed Jump

levelw=100
levelh=30

create_world( levelw, levelh)

fill_floor("fl-hay",0,0,levelw,levelh)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
        local c = strsub( pattern, i, i)
        if c =="#" then
            set_stone( "st-greenbrown", i-1, line)
        elseif c =="r" then
            set_stone( "st-invisible", i-1, line)
            abyss(i-1,line)
        elseif c == "o" then
            oxyd( i-1, line)
        elseif c=="z" then
            set_actor("ac-blackball", i,line+.5)
        elseif c == "." then
            set_floor("fl-hay",  i-1,  line)
        elseif c == " " then
            abyss(i-1,line)
	elseif c=="a" then
            set_item("it-trigger", i-1,line, {action="callback", target="funcca"})
	elseif c=="b" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccb"})
	elseif c=="c" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccc"})
	elseif c=="d" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccd"})
	elseif c=="e" then
            set_item("it-trigger", i-1,line, {action="callback", target="funcce"})
	elseif c=="f" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccf"})
	elseif c=="g" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccg"})
	elseif c=="h" then
            set_item("it-trigger", i-1,line, {action="callback", target="funcch"})
	elseif c=="i" then
            set_item("it-trigger", i-1,line, {action="callback", target="funcci"})
	elseif c=="j" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccj"})
	elseif c=="k" then
            set_item("it-trigger", i-1,line, {action="callback", target="funcck"})
	elseif c=="l" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccl"})
	elseif c=="m" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccm"})
	elseif c=="n" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccn"})
	elseif c=="p" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccp"})
	elseif c=="q" then
            set_item("it-trigger", i-1,line, {action="callback", target="funccq"})
        elseif c=="A" then
            set_item( "it-magnet",i-1 ,line , {name="m1"})
        elseif c=="B" then
            set_item( "it-magnet",i-1 ,line , {name="m2"})
        elseif c=="C" then
            set_item( "it-magnet",i-1 ,line , {name="m3"})
        elseif c=="D" then
            set_item( "it-magnet",i-1 ,line , {name="m4"})
        elseif c=="E" then
            set_item( "it-magnet",i-1 ,line , {name="m5"})
        elseif c=="F" then
            set_item( "it-magnet",i-1 ,line , {name="m6"})
        elseif c=="G" then
            set_item( "it-magnet",i-1 ,line , {name="m7"})
        elseif c=="H" then
            set_item( "it-magnet",i-1 ,line , {name="m8"})
        elseif c=="I" then
            set_item( "it-magnet",i-1 ,line , {name="m9"})
        elseif c=="J" then
            set_item( "it-magnet",i-1 ,line , {name="m10"})
        elseif c=="K" then
            set_item( "it-magnet",i-1 ,line , {name="m11"})
        elseif c=="L" then
            set_item( "it-magnet",i-1 ,line , {name="m12"})
        elseif c=="M" then
            set_item( "it-magnet",i-1 ,line , {name="m13"})
        elseif c=="N" then
            set_item( "it-magnet",i-1 ,line , {name="m14"})
        elseif c=="P" then
            set_item( "it-magnet",i-1 ,line , {name="m15"})
        elseif c=="W" then
            oneway(i-1,line, enigma.WEST)
        elseif c=="O" then
            set_item("it-extralife",i-1,line)
        elseif c == "S" then
            set_item("it-spring1",i-1,line)
        elseif c == "R" then --3-r
            set_floor("fl-gradient",  i-1,  line, {type=3})
        end
    end	
end

-- Floor:  " "
-- Border: "#"
-- Oxyd:   "o"

renderLine(00,"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr")
renderLine(01,"####                                                                     ####")
renderLine(02,"o..W.....................................................................W..o")
renderLine(03,"o..#                                                                     #..o")
renderLine(04,"o..#                                                                     #..o")
renderLine(05,"o..#######################################################################..o")
renderLine(06,"#z.S.a.Ab.Bc.Cd.De.Ef.Fg.Gh.Hi.Ij.Jk.Kl.Lm.Mn.Np.Pq..                RRRRRO.#")
renderLine(07,"o..#######################################################################..o")
renderLine(08,"o..#                                                                     #..o")
renderLine(09,"o..#                                                                     #..o")
renderLine(10,"o..W.....................................................................W..o")
renderLine(11,"####                                                                     ####")
renderLine(12,"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr")

oxyd_shuffle()

function funcca()
    SendMessage("m1","on")
end

function funccb()
    SendMessage("m1","off")
    SendMessage("m2","on")
end

function funccc()
    SendMessage("m2","off")
    SendMessage("m3","on")
end

function funccd()
    SendMessage("m3","off")
    SendMessage("m4","on")
end

function funcce()
    SendMessage("m4","off")
    SendMessage("m5","on")
end

function funccf()
    SendMessage("m5","off")
    SendMessage("m6","on")
end

function funccg()
   SendMessage("m6","off")
   SendMessage("m7","on")
end

function funcch()
   SendMessage("m7","off")
   SendMessage("m8","on")
end

function funcci()
   SendMessage("m8","off")
   SendMessage("m9","on")
end

function funccj()
   SendMessage("m9","off")
   SendMessage("m10","on")
end

function funcck()
   SendMessage("m10","off")
   SendMessage("m11","on")
end

function funccl()
   SendMessage("m11","off")
   SendMessage("m12","on")
end

function funccm()
   SendMessage("m12","off")
   SendMessage("m13","on")
end

function funccn()
   SendMessage("m13","off")
   SendMessage("m14","on")
end

function funccp()
   SendMessage("m14","off")
   SendMessage("m15","on")
end

function funccq()
   SendMessage("m15","off")
end















