-- A test level to test implementation of impulses

dofile(enigma.FindDataFile("levels/ralf.lua"))

debug_mode()

floortile = "fl-tigris"
walltile = "st-rock3"

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

level={
   "########################################",
   "#                                      #",
   "#   r     l              E@I@W         #",
   "#   ^     ^                       r    #",
   "# r +>r l<+ l        O          O      #",
   "#             O        #W E#      @    #",
   "#   r  1  l   @         i i       N    #",
   "#                 aw                   #",
   "#                 %%    @E@ PPPPPP @   #",
   "#             X   %%                   #",
   "#             x              l         #",
   "#      O@  Xx   XxEW@O      #  E@ #    #",
   "#                                l     #",
   "# #                           S        #",
   "# S           X               @        #",
   "# @           x              l         #",
   "#             S                l       #",
   "#             N             l          #",
   "#             @               l        #",
   "#             O      l                 #",
   "# @                  EEEEEEEEEESl      #",
   "# N                  @         @       #",
   "# #                 lNWWWWWWWWWW       #",
   "#                              l       #",
   "#                                      #",
   "########################################",
}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

function trigger_test()
   set_stone("st-blocker",2,17,{name="b1"})
   set_stone("st-blocker",2,18,{name="b2"})

   set_item("it-trigger", 4,17,{action="trigger", target="b1"});
   set_stone("st-switch", 5,18,{action="trigger", target="b2"});
end

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

cells={}

-- documentation

cells["1"]=cell{{{document, "Sometimes the turnstiles rotate two steps"}}}

-- basics

cells[" "] = cell{floor=floortile}
cells["#"] = cell{parent=cells[" "],stone=walltile}
cells["@"] = cell{parent=cells[" "],stone="st-brownie"}

cells["a"] = cell{parent=cells[" "],actor={"ac-blackball", {player=0}}}
cells["O"] = cell{oxyd}

-- turnstiles:
--cells["+"] = cell{parent=cells[" "],stone="st-turnstile-green"}
cells["+"] = cell{parent=cells[" "],stone="st-turnstile"}

cells["%"] = cell{parent=cells[" "],stone="st-brake"}

cells["^"] = cell{parent=cells[" "],stone="st-turnstile-n"}
cells["v"] = cell{parent=cells[" "],stone="st-turnstile-s"}
cells["<"] = cell{parent=cells[" "],stone="st-turnstile-w"}
cells[">"] = cell{parent=cells[" "],stone="st-turnstile-e"}

-- rotators:

cells["r"] = cell{parent=cells[" "],stone="st-rotator-right"}
cells["l"] = cell{parent=cells[" "],stone="st-rotator-left"}

-- impulse stones

cells["i"] = cell{parent=cells[" "],stone="st-stoneimpulse"}
cells["I"] = cell{parent=cells[" "],stone="st-stoneimpulse_movable"}

-- bolders and blockers

cells["E"] = cell{parent=cells[" "], stone="st-bolder-e"}
cells["W"] = cell{parent=cells[" "], stone="st-bolder-w"}
cells["N"] = cell{parent=cells[" "], stone="st-bolder-n"}
cells["S"] = cell{parent=cells[" "], stone="st-bolder-s"}

cells["X"] = cell{parent=cells[" "], stone="st-blocker"}
cells["x"] = cell{parent=cells[" "], item="it-blocker"}

cells["w"] = cell{parent=cells[" "], item="it-magicwand"}

cells["P"] = cell{parent=cells[" "], stone="st-pull"}

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

rs_create_world(level,cells)
display.SetFollowMode(display.FOLLOW_SCROLLING)
oxyd_shuffle()

trigger_test()
