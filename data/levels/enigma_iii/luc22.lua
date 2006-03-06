-- Remote Control, a level for Enigma
-- Copyright (C) 2006 Lukas Schueller
-- Licensed under GPL v2.0 or above
------------------------------
function writeLine( line, cells)
  for i=1, strlen(cells) do
    local c = strsub(cells,i,i)
    if(c =="#") then
      set_stone("st-blue-sand",i-1,line)
    elseif(c =="-") then
      --set_floor("fl-leaves",i-1,line)
      set_floor("fl-acwhite",i-1,line)   
    elseif(c =="+") then
      set_stone("st-death",i-1,line)
    elseif(c =="o") then
      oxyd(i-1,line)
    elseif(c =="0") then
      set_actor("ac-blackball",i-0.5,line+0.5,{player="0"})
      set_item("it-yinyang",i-1,line)
      set_floor("fl-acwhite",i-1,line)
    elseif(c =="1") then
      set_actor("ac-whiteball",i-0.5,line+0.5,{player="1"})
      set_item("it-yinyang",i-1,line)
    end
  end
end

CreateWorld(20,25)
fill_floor("fl-acblack",0,0,20,25)
oxyd_default_flavor="a"
----------------------------------
--Environment
writeLine(00,"####################")
writeLine(01,"#------------------#")
writeLine(02,"#------------------#")
writeLine(03,"#------------------#")
writeLine(04,"#------------------#")
writeLine(05,"#-------###--------#")
writeLine(06,"#-------#0#--------#")
writeLine(07,"#-------###--------#")
writeLine(08,"#------------------#")
writeLine(09,"#------------------#")
writeLine(10,"#------------------#")
writeLine(11,"#------------------#")
writeLine(12,"####################")
writeLine(13,"#1      #   #      #")
writeLine(14,"#####+# # + # +### #")
writeLine(15,"#       # # # #    #")
writeLine(16,"# +###o## # # # ####")
writeLine(17,"# #       # #o+    #")
writeLine(18,"# # #####o# ###### #")
writeLine(19,"# # #    #  #      #")
writeLine(20,"# # # +# # ## ##+#o#")
writeLine(21,"# #    # # +       #")
writeLine(22,"# o##### # ####### #")
writeLine(23,"#        #         #")
writeLine(24,"########+#o#########")
oxyd_shuffle()


















