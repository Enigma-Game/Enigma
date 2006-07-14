-- Limited Supplies, a level for Enigma
-- Copyright (c) 2005 Jacob Scott
-- License: GPL v2.0 or above

rooms_wide=1
rooms_high=1

levelw=1+(19*rooms_wide)+1
levelh=1+(12*rooms_high)

create_world( levelw, levelh)

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine(line, pattern)
	for i=1, strlen(pattern) do
		c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
			set_floor("fl-water",i-1,line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c == "&" then
		   set_floor("fl-water",i-1,line)
		elseif c=="z" then
		   set_actor("ac-blackball", i-.5,line+.5, {name="ac1",player=0})
		   document(i-1,line,"24")
		elseif c=="y" then
		   set_actor("ac-whiteball", i-1,line+.5, {player=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="~" then
		   set_floor("fl-water",i-1,line)
		   set_item("it-trigger",i-1,line,{invisible=1,action="callback",target="funcc1"})
		elseif c=="M" then
		   set_stone("st-timer",i-1,line,{name="t1",loop=1,interval=.001,action="callback",target="funcctimer"})
	     end
	end	
end

renderLine(00,"&&&&&&&&&&&&&&&&&&&&M")
renderLine(01,"&~o~~~~~~~~~~~~ ~~~& ")
renderLine(02,"&~ ~~~ ~o~ ~ ~~~~o~& ")
renderLine(03,"&~~~ ~~~~~~~~~ ~ ~~& ")
renderLine(04,"&~~~~~~~~ ~~~~~~~~ & ")
renderLine(05,"&~ o~ ~ ~~~~~~ ~~~~& ")
renderLine(06,"&~~~~~~~~z~~o~~~~~~& ")
renderLine(07,"&~ ~~~ ~~~~~~ ~~~~~& ")
renderLine(08,"&~~~~~~~ ~~ ~~~~ ~~& ")
renderLine(09,"&~ ~~~~o~~~~~~~~~~~& ")
renderLine(10,"&~~~ ~ ~~~~~~ ~~ ~~& ")
renderLine(11,"&o ~~~~~~~ ~~~~~~o~& ")
renderLine(12,"&&&&&&&&&&&&&&&&&&&& ")

oxyd_shuffle()

ac1=enigma.GetNamedObject("ac1")
t1=enigma.GetNamedObject("t1")

enigma.SendMessage(t1,"off",nil)

num=1
posx=0
posy=0
ff1=0

bridges=24

function funcc1()
   if (num<=bridges) then
      if (ff1==0) then
	 x,y=enigma.GetPos(ac1)
	 set_floor("fl-hay",x,y)
	 num=num+1
	 posx=x
	 posy=y
	 enigma.SendMessage(t1,"on",nil)
	 ff1=1
      else
	 ff1=0
      end
   end
end

function funcctimer()
   enigma.KillItem(posx,posy)
   enigma.SendMessage(t1,"off",nil)
   ff1=0
end



























