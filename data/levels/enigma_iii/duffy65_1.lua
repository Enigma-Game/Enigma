-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Overwhelming

levelw=20
levelh=13

create_world( levelw, levelh)

fill_floor("fl-mortar", 0,0,levelw,levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
		if c =="#" then
			set_stone( "st-greenbrown", i-1, line)
		elseif c == "o" then
			oxyd( i-1, line)
		elseif c == "*" then
			set_stone( "st-brownie", i-1, line)
                elseif c == "!" then
			abyss(i-1,line)
		elseif c=="w" then
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		elseif c == "g" then
			draw_stones("st-grate1",{i-1,line}, {1,1}, 1)
		elseif c=="+" then
			set_stone( "st-wood", i-1, line)
		elseif c=="=" then
			set_floor("fl-space",i-1,line)
		elseif c=="i" then
			set_floor("fl-ice_001",i-1,line)
		elseif c=="h" then
		   set_item("it-hill",i-1,line)
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
		elseif c=="H" then
		   set_item("it-hollow",i-1,line)
		elseif c=="I" then
		   set_item("it-hollow",i-1,line)
		   set_actor("ac-whiteball-small", i-.5,line+.5, {player=0,mouseforce=1})
	elseif c=="A" then
	   if difficult==false then
	      xx=i-.5
	      yy=line-4.5
	   else
	      xx=i-.5
	      yy=line-2.5
	   end
	   set_item("it-wormhole", i-1, line, {strength=2,targetx=xx,targety=yy})
	elseif c=="B" then
	   if difficult==false then
	      xx=i+5.5
	      yy=line+.5
	   else
	      xx=i+2.5
	      yy=line+.5
	   end
	   set_item("it-wormhole", i-1, line, {strength=2,targetx=xx,targety=yy})
	elseif c=="C" then
	   if difficult==false then
	      xx=i-.5
	      yy=line+5.5
	   else
	      xx=i-.5
	      yy=line+3.5
	   end
	   set_item("it-wormhole", i-1, line, {strength=2,targetx=xx,targety=yy})
	elseif c=="D" then
	   if difficult==false then
	      xx=i-6.5
	      yy=line+.5
	   else
	      xx=i-3.5
	      yy=line+.5
	   end
	   set_item("it-wormhole", i-1, line, {strength=2,targetx=xx,targety=yy})
	     end
	end	
     end

if not difficult then

renderLine(00,"!!!!!!!!!!!!!!!!!!!!")
renderLine(01,"!!###!!!!###!!!!###!")
renderLine(02,"!##H##!!##H##!!##H##")
renderLine(03,"!#IhB#!!#DhB#!!#DhI#")
renderLine(04,"!##C##!!##C##!!##C##")
renderLine(05,"!!###!!!!###!!!!###!")
renderLine(06,"!!!!!!!!!!!!!!!!!!!!")
renderLine(07,"!!###!!!!###!!!!###!")
renderLine(08,"!##A##!!##A##!!##A##")
renderLine(09,"!#IhB#!!#DhB#!!#DhI#")
renderLine(10,"!##H##!!##H##!!##H##")
renderLine(11,"!!###!!!!###!!!!###!")
renderLine(12,"!!!!!!!!!!!!!!!!!!!!")

else

renderLine(00,"!!!###!###!###!###!!")
renderLine(01,"!!##H###H###H###H##!")
renderLine(02,"!!#HhB#DhB#DhB#DhH#!")
renderLine(03,"!!##C###C###C###C##!")
renderLine(04,"!!!###!###!###!###!!")
renderLine(05,"!!##A###A###A###A##!")
renderLine(06,"!!#IhB#DhB#DhB#DhI#!")
renderLine(07,"!!##C###C###C###C##!")
renderLine(08,"!!!###!###!###!###!!")
renderLine(09,"!!##A###A###A###A##!")
renderLine(10,"!!#HhB#DhB#DhB#DhH#!")
renderLine(11,"!!##H###H###H###H##!")
renderLine(12,"!!!###!###!###!###!!")

end














