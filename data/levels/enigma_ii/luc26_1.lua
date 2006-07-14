-- Growing Boxes, a level for Enigma
-- Copyright (C) 2006 Lukas Schueller
-- Licensed under GPL v2.0 or above
------------------------------
function bad()
  enigma.SendMessage(enigma.GetNamedObject("bridgeb"),"openclose",nil)
  enigma.SendMessage(enigma.GetNamedObject("doord"),"openclose",nil)
end
function writeLine( line, cells)
  for i=1, strlen(cells) do
    local c = strsub(cells,i,i)
    if(c =="#") then
      set_stone("st-marble",i-1,line)
    elseif(c =="h") then
      set_stone("st-marble_hole",i-1,line)
    elseif(c =="1") then
      set_stone("st-greenbrown",i-1,line)
    elseif(c =="2") then
      set_stone("st-plain",i-1,line)
    elseif(c ==".") then
      set_floor("fl-abyss",i-1,line)
    elseif(c =="-") then
      set_floor("fl-bridge",i-1,line,{name="bridgeb"})
    elseif(c =="s") then
      set_item("it-seed",i-1,line)
    elseif(c =="n") then
      set_item("it-seed_nowood",i-1,line)
    elseif(c =="v") then
      set_item("it-seed_volcano",i-1,line)
    elseif(c =="a") then
      document(i-1,line,"You'd need a portable box!")
    elseif(c =="b") then
      document(i-1,line,"Congratulations! You found a seed! Try to use it behind the grate (and move away quickly)")
    elseif(c =="c") then
      document(i-1,line,"Remember that you can't fill a closed bridge with a box...                                           but maybe you can use the 3rd seed differently!")
    elseif(c =="d") then
      document(i-1,line,"There are different kinds of seeds... Try to use them!")
    elseif(c =="e") then
      document(i-1,line,"Noitcerid etisoppo eht yrt! Tuo syaw owt? Rorrim, rorrim...")
    elseif(c =="+") then
      set_stone("st-wood",i-1,line)
    elseif(c =="x") then
      set_stone("st-grate1",i-1,line)
      set_item("it-tinyhill",i-1,line)
    elseif(c =="=") then
      set_stone("st-door_a",i-1,line,{name="doord"})
    elseif(c =="*") then
      set_stone("st-switch",i-1,line,{action="callback",target="bad"})
    elseif(c =="o") then
      oxyd(i-1,line)
    elseif(c =="0") then
      set_actor("ac-blackball",i-0.5,line+0.5,{player="0"})
    end
  end
end

CreateWorld(58,13)
fill_floor("fl-tigris",0,0,58,13)
----------------------------------
--Environment
writeLine(00,"...................#########################+12###########")
writeLine(01,"...................#..................#     snv        #o#")
writeLine(02,"...................h.     #..       ..#                # #")
writeLine(03,"...................h. + + #..       ..#                # #")
writeLine(04,"...................h.     #..       ..#                # #")
writeLine(05,"...................h.     #..s####  ..#                # #")
writeLine(06,"..................sh. 0e+ xa.s=c*#  ..-d               . #")
writeLine(07,"..................bh.     #..s####  ..#                # #")
writeLine(08,"...................h.     #..       ..#                # #")
writeLine(09,"...................h. + + #..       ..#                # #")
writeLine(10,"...................h.     #..       ..#                # #")
writeLine(11,"...................#..................#                #o#")
writeLine(12,"...................#######################################")
enigma.SendMessage(enigma.GetNamedObject("doord"),"openclose",nil)
enigma.ConserveLevel = FALSE





