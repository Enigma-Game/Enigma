-- Water Castle, a level for Enigma
-- Copyright (C) 2006 Lukas Schueller
-- Licensed under GPL v2.0 or above
------------------------------

box = 0
function setBox()
  if(box == 0) then
    set_stone("st-wood",12,2)
    box = 1
  else
    box = 0
  end
end

function writeLine( line, cells)
  for i=1, strlen(cells) do
    local c = strsub(cells,i,i)
    if(c =="#") then
      set_stone("st-rock6",i-1,line)
    elseif(c =="~") then
      set_floor("fl-water",i-1,line)
    elseif(c =="+") then
      set_stone("st-wood",i-1,line)
    elseif(c =="c") then
      set_item("it-coin1",i-1,line)
    elseif(c =="b") then
      set_stone("st-coinslot",i-1,line,{action="callback",target="setBox"})
    elseif(c =="o") then
      oxyd(i-1,line)
    elseif(c =="0") then
      set_actor("ac-blackball",i-0.5,line+0.5,{player="0"})
      set_item("it-yinyang",i-1,line)
    elseif(c =="1") then
      set_actor("ac-whiteball",i-0.5,line+0.5,{player="1"})
      set_item("it-yinyang",i-1,line)
    end
  end
end

CreateWorld(20,13)
fill_floor("fl-bluegray",0,0,20,13)
writeLine(00,"~~~########b#####~~~")
writeLine(01,"~~~#~    ~#  c ~#~~~")
writeLine(02,"~~~#  +~  # 1c+ #~~~")
writeLine(03,"~~~# ~  + # +c~ #~~~")
writeLine(04,"~~~# +0 ~ # +c+~#~~~")
writeLine(05,"~~~#  ~+  #~+c  #~~~")
writeLine(06,"~~~#~    ~#~cc~ #~~~")
writeLine(07,"~~~##### ### ####~~~")
writeLine(08,"~~~o ~~~~~  ~  ~#~~~")
writeLine(09,"~~~#~~~~~ ~  ~~ #~~~")
writeLine(10,"~~~#~~~~#########~~~")
writeLine(11,"~~~o~~~~     #~~#~~~")
writeLine(12,"~~~#######o#o####~~~")
oxyd_shuffle()


















