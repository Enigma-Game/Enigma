-- Copyright (c) 2004 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Bungeeball

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)
levelh=1+(12*rooms_high)

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-himalaya", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-rock6", i-1, line)
		elseif c =="A" then
		   set_stone( "st-rock4", i-1, line,{name="A"})
		elseif c =="B" then
		   set_stone( "st-rock4", i-1, line,{name="B"})
		elseif c =="C" then
		   set_stone( "st-rock4", i-1, line,{name="C"})
		elseif c =="D" then
		   set_stone( "st-rock4", i-1, line,{name="D"})
		elseif c =="E" then
		   set_stone( "st-rock4", i-1, line,{name="E"})
		elseif c =="F" then
		   set_stone( "st-rock4", i-1, line,{name="F"})
		elseif c =="G" then
		   set_stone( "st-rock4", i-1, line,{name="G"})
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "~" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {controllers=1, player=0,name="z"})
		elseif c=="a" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="a",controllers=0, mouseforce=0, player=1})
		elseif c=="b" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="b",controllers=0, mouseforce=0, player=1})
		elseif c=="c" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="c",controllers=0, mouseforce=0, player=1})
		elseif c=="d" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="d",controllers=0, mouseforce=0, player=1})
		elseif c=="e" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="e",controllers=0, mouseforce=0, player=1})
		elseif c=="f" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="f",controllers=0, mouseforce=0, player=1})
		elseif c=="h" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="h",controllers=0, mouseforce=0, player=1})
		elseif c=="i" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="i",controllers=0, mouseforce=0, player=1})
		elseif c=="j" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {name="j",controllers=0, mouseforce=0, player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="H" then
			set_item("it-hollow",i-1,line)
	     end
	end	
end

renderLine(00,"####################")
renderLine(01,"#              f   #")
renderLine(02,"#  b   H      iFh  #")
renderLine(03,"#  B       H H     #")
renderLine(04,"#               H  #")
renderLine(05,"#   H      z       #")
renderLine(06,"#     Ea  A        #")
renderLine(07,"# H   e         H  #")
renderLine(08,"#          H       #")
renderLine(09,"#           j  D   #")
renderLine(10,"#  cC       G  d   #")
renderLine(11,"#         H        #")
renderLine(12,"####################")

oxyd_shuffle()

z=enigma.GetNamedObject("z")

a=enigma.GetNamedObject("a")
b=enigma.GetNamedObject("b")
c=enigma.GetNamedObject("c")
d=enigma.GetNamedObject("d")
e=enigma.GetNamedObject("e")
f=enigma.GetNamedObject("f")
h=enigma.GetNamedObject("h")
i=enigma.GetNamedObject("i")
j=enigma.GetNamedObject("j")

A=enigma.GetNamedObject("A")
B=enigma.GetNamedObject("B")
C=enigma.GetNamedObject("C")
D=enigma.GetNamedObject("D")
E=enigma.GetNamedObject("E")
F=enigma.GetNamedObject("F")
G=enigma.GetNamedObject("G")

AddRubberBand(z,A,5,1)

AddRubberBand(b,B,7,1.8)
AddRubberBand(c,C,3,1.7)
AddRubberBand(d,D,30,1.5)
AddRubberBand(e,E,17,1.3)
AddRubberBand(a,e,17,1.4)
AddRubberBand(f,F,18,1.1)
AddRubberBand(f,h,25,1.1)
AddRubberBand(f,i,13,1.3)
AddRubberBand(j,G,25,1.2)













