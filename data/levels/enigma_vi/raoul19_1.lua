-- Zeus - The master of flashes, a level for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above

enigma.ConserveLevel = TRUE
enigma.FrictionFactor=0.5

CreateWorld(20,13)
oxyd_default_flavor = "d"

function writeLine( line, cells)
  for i=1, strlen(cells) do
    local c = strsub(cells,i,i)

    if(c =="#") then
      set_stone("st-blue-sand",i-1,line)

    elseif(c == "-") then
      set_floor("fl-rough",i-1,line)

    elseif(c == " ") then
      set_floor("fl-water",i-1,line)

    elseif(c == "e") then
      set_stone("st-white4",i-1,line)
      set_floor("fl-rough",i-1,line)

    elseif(c == "x") then
      set_stone("st-grate1",i-1,line)
      set_floor("fl-rough",i-1,line)

    elseif(c =="o") then
      oxyd(i-1,line)

    elseif(c =="F") then
      set_stone("st-flash",i-1,line)

    end
  end
end

if not difficult then
  writeLine(00,"####################")
  writeLine(01,"#---x----------x---#")
  writeLine(02,"#-o-#   #####  #-o-#")
  writeLine(03,"#---x---x---#  #---#")
  writeLine(04,"#####   #-o-#  ###x#")
  writeLine(05,"#---x---x---#     -#")
  writeLine(06,"#-o-# ############e#")
  writeLine(07,"#---x-x---#  #-----#")
  writeLine(08,"##### #-o-#  #-----#")
  writeLine(09,"#---x-x---#  #--F--#")
  writeLine(10,"#-o-# #####  #-----#")
  writeLine(11,"#---x--------e-----#")
  writeLine(12,"####################")
else
  writeLine(00,"####################")
  writeLine(01,"#------------------#")
  writeLine(02,"#-o-    ## ##   -o-#")
  writeLine(03,"#-----------#  #---#")
  writeLine(04,"## ##    -o-   ## -#")
  writeLine(05,"#-----------#     -#")
  writeLine(06,"#-o-  ############e#")
  writeLine(07,"#---------#  #-----#")
  writeLine(08,"## ##  -o-   #-----#")
  writeLine(09,"#---------#  #--F--#")
  writeLine(10,"#-o-  ## ##  #-----#")
  writeLine(11,"#------------e-----#")
  writeLine(12,"####################")
end

set_actor("ac-blackball",18.5,11.5,{player="0"})
set_actor("ac-whiteball",14.5,7.5)

oxyd_shuffle()

