-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
--Esprit #022

levelw = 58
levelh = 37

create_world(levelw, levelh)

oxyd_default_flavor = "b"
fill_floor("fl-rough-blue", 0, 0, levelw,levelh)
set_actor("ac-blackball", 29,6.5)

function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c == "#" then
         set_stone("st-rock3",i-1,line)
      elseif c == "b" then                      
	 set_stone("st-rock3_break", i-1,line)
      elseif c == "g" then                      
	 set_stone("st-grate1", i-1,line)
      elseif c == "^" then                      
	 set_stone("st-oneway_black-n", i-1,line)
      elseif c=="1" then	
	 set_item("it-coin1", i-1,line)
      elseif c == "h" then
	 hammer(i-1,line)
      elseif c=="d" then	
	 set_item("it-dynamite", i-1,line)
      elseif c == "K" then                      
	 set_stone("st-rock3_hole", i-1,line)
	 set_actor("ac-rotor", i-.5,line+.5, {range=8, force=40})

      elseif c == "O" then
         oxyd( i-1, line)
        end
    end
end

--              0123456789012345678901234567890123456789012345678901234567
renderLine(00, "##########################################################")
renderLine(01, "#                bbb                  #bb                #")
renderLine(02, "#                bbb 1                #bb                #")
renderLine(03, "#                bbb                  #bb                #")
renderLine(04, "#                bbb                  #b#                #")
renderLine(05, "#                bbb                  bb#                #")
renderLine(06, "#        O       bbb                  b##      O         #")
renderLine(07, "#                bbb                  bb#                #")
renderLine(08, "#                bbb                  #b#bbbbbbbbbbbbbbbb#")
renderLine(09, "#                bbb     1            #b#bbbbbbbbbbbbbbbb#")
renderLine(10, "#                bbb   1        1   1 #bbbbbbbbbbbbbbbbbb#")
renderLine(11, "#                bbb               1  #bbbbbbbbbbbbbbbbbb#")
renderLine(12, "#bbbbbbbbbbbbbbbbbb#########gg#################^^#########")
                                                                         
renderLine(13, "#                  #                ddKbbbb##bbb##bbbbbbb#")
renderLine(14, "#                  #                ddbbb###bb#######bbbb#")
renderLine(15, "#                  #                ddb#bb#bb#########bbb#")
renderLine(16, "#                  #                ddbb##bb#bbbbbbbbbbbb#")
renderLine(17, "#                  #                  bb#                #")
renderLine(18, "#        O         #        O         bb#      O         #")
renderLine(19, "#                  #                  bb#                #")
renderLine(20, "#         h        #                  bbb                #")
renderLine(21, "#                  #                  bbbbb#bbbbb#bbbbb#b#")
renderLine(22, "#                  #                  bbbbbbbbbbbbbbb#bbb#")
renderLine(23, "#                  #                  bbbbbbbbbb#b#bbbbbb#")
renderLine(24, "#########gg#################^^###########bb#bbb#bbb#b#b#b#")
                                                                         
renderLine(25, "#                  #bbbbbbbbbbbbbbbbbbbbbbbbb#bbbbbb#bbbb#")
renderLine(26, "#                  #bbbbbbbbbbbbbbbbbbbbbbbbbbb###bbbb#bb#")
renderLine(27, "#                  #                  bbbb#bbb#bb#bb#b#bb#")
renderLine(28, "#                  #                  bbbb#bb#bb#bb#bbb#b#")
renderLine(29, "#                  g                  bbbbbbbbbbbbbbbbbbb#")
renderLine(30, "#        O         g        O         bbb      O         #")
renderLine(31, "#                  g                  bbb                #")
renderLine(32, "#                  #                  b#b                #")
renderLine(33, "#                  #                  b#b                #")
renderLine(34, "#                  #                  b#b                #")
renderLine(35, "#                  #                  bbb                #")
renderLine(36, "##########################################################")
--              0123456789012345678901234567890123456789012345678901234567


oxyd_shuffle()











