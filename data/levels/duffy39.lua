-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Turnstile City

levelw=20
levelh=13

create_world( levelw, levelh)
enigma.ConserveLevel=FALSE

fill_floor("fl-hay", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c =="X" then
			set_stone( "st-death", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
--			fill_floor("fl-water", i-1,line, 1,1)
                elseif c == "~" then
--			abyss(i-1,line)
			fill_floor("fl-water", i-1,line, 1,1)
		elseif c=="z" then
		        set_actor("ac-blackball", i-.5,line+.5)
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c =="A" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge1"})
		elseif c =="B" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge2"})
		elseif c =="C" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge3"})
		elseif c =="D" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge4"})
		elseif c =="E" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge5"})
		elseif c =="H" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge6"})
		elseif c =="K" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge7"})
		elseif c =="M" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge8"})
		elseif c =="N" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge9"})
		elseif c =="P" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge10"})
		elseif c =="Q" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge11"})
		elseif c =="R" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge12"})
		elseif c =="S" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge13"})
		elseif c =="T" then
			set_item("it-trigger", i-1,line, {action="openclose", target="bridge14"})
		elseif c=="=" then
		   set_stone("st-turnstile",i-1,line)
		   set_stone("st-turnstile-w",i-2,line)
		   set_stone("st-turnstile-e",i,line)
		elseif c=="i" then
		   set_stone("st-turnstile",i-1,line)
		   set_stone("st-turnstile-n",i-1,line-1)
		   set_stone("st-turnstile-s",i-1,line+1)
		elseif c=="l" then
		   set_stone("st-turnstile",i-1,line)
		   set_stone("st-turnstile-n",i-1,line-1)
		   set_stone("st-turnstile-e",i,line)
		elseif c=="j" then
		   set_stone("st-turnstile",i-1,line)
		   set_stone("st-turnstile-n",i-1,line-1)
		   set_stone("st-turnstile-w",i-2,line)
		elseif c=="f" then
		   set_stone("st-turnstile",i-1,line)
		   set_stone("st-turnstile-s",i-1,line+1)
		   set_stone("st-turnstile-e",i,line)
		elseif c=="7" then
		   set_stone("st-turnstile",i-1,line)
		   set_stone("st-turnstile-s",i-1,line+1)
		   set_stone("st-turnstile-w",i-2,line) 
		elseif c=="a" then
		   set_floor("fl-bridge",i-1,line,{name="bridge1"})
		elseif c=="b" then
		   set_floor("fl-bridge",i-1,line,{name="bridge2"})
		elseif c=="c" then
		   set_floor("fl-bridge",i-1,line,{name="bridge3"})
		elseif c=="d" then
		   set_floor("fl-bridge",i-1,line,{name="bridge4"})
		elseif c=="e" then
		   set_floor("fl-bridge",i-1,line,{name="bridge5"})
		elseif c=="h" then
		   set_floor("fl-bridge",i-1,line,{name="bridge6"})
		elseif c=="k" then
		   set_floor("fl-bridge",i-1,line,{name="bridge7"})
		elseif c=="m" then
		   set_floor("fl-bridge",i-1,line,{name="bridge8"})
		elseif c=="n" then
		   set_floor("fl-bridge",i-1,line,{name="bridge9"})
		elseif c=="p" then
		   set_floor("fl-bridge",i-1,line,{name="bridge10"})
		elseif c=="q" then
		   set_floor("fl-bridge",i-1,line,{name="bridge11"})
		elseif c=="r" then
		   set_floor("fl-bridge",i-1,line,{name="bridge12"})
		elseif c=="s" then
		   set_floor("fl-bridge",i-1,line,{name="bridge13"})
		elseif c=="t" then
		   set_floor("fl-bridge",i-1,line,{name="bridge14"})
		elseif c =="O" then
			set_item("it-extralife", i-1,line)
		elseif c =="`" then
		   set_item("it-puller-s", i-1, line)
	     end
	end	
end

function yy1( color, x, y)
       stone = format( "st-%s4", color)
        set_stone( stone, x, y)
     end

renderLine(00,"#######o#o#o#o#o#o##")
renderLine(01,"#edcba             #")
renderLine(02,"#h#### i i i i i i #")
renderLine(03,"#k#z               #")
renderLine(04,"#m#   ` ` ` ` ` `  #")
renderLine(05,"#n#  j j j j j j j #")
renderLine(06,"#p#  7A7B7C7D7E7H7K#")
renderLine(07,"#q#                o")
renderLine(08,"#r#                #")
renderLine(09,"#s#  l l l l l l l o")
renderLine(10,"#t# MfNfPfQfRfSfTf #")
renderLine(11,"o o                o")
renderLine(12,"#o##o#############o#")

oxyd_shuffle()
