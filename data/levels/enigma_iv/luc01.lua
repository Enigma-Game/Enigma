------------------------------
-- Friction Nightmare
-- An Enigma Level
-- Copyright (C) 2005 by Lukas Schueller
-- Contact: Lucky_Luc@web.de
------------------------------
-- Dec.05: Changed floor and friction to match Jacobs Nightmare-pattern, Andreas

function writeLine( line, cells)
  for i=1, strlen(cells) do
    local c = strsub(cells,i,i)
    if(c =="a") then
      set_stone("st-death",i-1,line)
    elseif(c =="o") then
      oxyd(i-1,line)
    end
  end
end

CreateWorld(20,13)
--fill_floor("fl-leaves",0,0,20,13)
fill_floor("fl-rough-blue",0,0,20,13)
--enigma.FrictionFactor=-0.5
if difficult then
  enigma.FrictionFactor=-0.4
else
  enigma.FrictionFactor=-0.25
end
writeLine(00,"aaaaaaaaaaaaaaaaaaaa")
writeLine(01,"o                  o")
writeLine(02,"a  a               a")
writeLine(03,"a        a         a")
writeLine(04,"a              a   a")
writeLine(05,"a                  a")
writeLine(06,"a     a    a       a")
writeLine(07,"a                  a")
writeLine(08,"a       a          a")
writeLine(09,"a            a     a")
writeLine(10,"a     a            a")
writeLine(11,"o                  o")
writeLine(12,"aaaaaaaaaaaaaaaaaaaa")
set_actor("ac-blackball",1.5,1.5,{player=0})
oxyd_shuffle()
























