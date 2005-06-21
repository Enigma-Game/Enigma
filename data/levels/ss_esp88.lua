-- A level for Enigma
-- Copyright: 	(C) 2003 Sven Siggelkow
-- License: 	GPL v2.0 or above
-- ESPRIT #088

levelw = 39
levelh = 13
create_world(levelw, levelh)

set_actor("ac-blackball", 2.5,10.5)

fill_floor("fl-bluegray")


function renderLine( line, pattern)
    for i=1, strlen(pattern) do
      local c = strsub( pattern, i, i)
      if c ==   "#" then
         set_stone("st-door_b", i-1,line, {type="v"})
      elseif c=="A" then
         abyss(i-1,line)
      elseif c=="x" then
         set_item("it-key_a", i-1,line)
      elseif c=="y" then
         set_item("it-key_b", i-1,line)
      elseif c=="z" then
         set_item("it-key_c", i-1,line)
      elseif c=="1" then
         set_stone("st-key_a", i-1, line, {action="openclose", target="a"})
      elseif c=="2" then
         set_stone("st-key_b", i-1, line, {action="openclose", target="b"})
      elseif c=="3" then
         set_stone("st-key_a", i-1, line, {action="openclose", target="c"})
      elseif c=="4" then
         set_stone("st-key_c", i-1, line, {action="openclose", target="d"})
      elseif c=="5" then
         set_stone("st-key_b", i-1, line, {action="openclose", target="e"})
      elseif c=="6" then
         set_stone("st-key_c", i-1, line, {action="openclose", target="f"})
      elseif c=="7" then
         set_stone("st-key_b", i-1, line, {action="openclose", target="g"})
      elseif c=="8" then
         set_stone("st-key_c", i-1, line, {action="openclose", target="h"})
      elseif c=="9" then
         set_stone("st-key_a", i-1, line, {action="openclose", target="i"})
      elseif c=="a" then
         set_stone("st-door_c", i-1, line, {name="a"})
      elseif c=="b" then
         set_stone("st-door_c", i-1, line, {name="b"})
      elseif c=="c" then
         set_stone("st-door_c", i-1, line, {name="c"})
      elseif c=="d" then
         set_stone("st-door_c", i-1, line, {name="d"})
      elseif c=="e" then
         set_stone("st-door_c", i-1, line, {name="e"})
      elseif c=="f" then
         set_stone("st-door_c", i-1, line, {name="f"})
      elseif c=="g" then
         set_stone("st-door_c", i-1, line, {name="g"})
      elseif c=="h" then
         set_stone("st-door_c", i-1, line, {name="h"})
      elseif c=="i" then
         set_stone("st-door_c", i-1, line, {name="i"})
      elseif c=="o" then
         set_stone("st-key_a", i-1, line)
      elseif c=="p" then
         set_stone("st-key_b", i-1, line)
      elseif c=="q" then
         set_stone("st-key_c", i-1, line)
      elseif c=="O" then 
	 oxyd(i-1,line)  
        end
    end
end
--              012345678901234567890123456789012345678 
renderLine(00, "#########AA####A#######A####AA#########")
renderLine(01, "#O  g  O#AA#x #A# y#  #A# z#AA#O  c  O#")
renderLine(02, "#   #   #AA#  #A#  #  #A#  #AA#   #   #")
renderLine(03, "#   #   #AA#  #A#  #  #A#  #AA#   #   #")
renderLine(04, "##e######AA#  #A#  #  #A#  #AA##h##bd##")  
renderLine(05, "#O  a  O#AA#  #A#  #  #A#  #AA#O  f  O#")
renderLine(06, "#   #   #AA#  #A#  #  #A#  #AA#   #   #")
renderLine(07, "#   #   #AA#  #A#  #  #A#  #AA#   i   #")
renderLine(08, "#p1o2qoq34p#  ###  #  ###  #5o6p7q89qp#")
renderLine(09, "#                                     #")
renderLine(10, "#                                     #")
renderLine(11, "#                                     #")
renderLine(12, "#######################################")
--              012345678901234567890123456789012345678

set_actor("ac-whiteball-small", 2.5, 2.5, {controllers=1})
set_actor("ac-whiteball-small",36.5, 2.5, {controllers=1})

oxyd_shuffle()
