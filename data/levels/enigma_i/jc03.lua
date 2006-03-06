-- Why so slow?
-- by Jens-Christian Korth

Force = 40
ForceX = 0
ForceY = 0

function SetForce(x,y)
   enigma.AddConstantForce(x, y)
   ForceX = x
   ForceY = y
end

function swT()
   SetForce(ForceX, Force)
end

function swB()
   SetForce(ForceX, -Force)
end

function swL()
   SetForce(Force, ForceY)
end

function swR()
   SetForce(-Force, ForceY)
end


create_world(20, 13) 
oxyd_default_flavor = "d"
draw_border("st-rock5")
fill_floor("fl-bluegray")
set_floor("fl-abyss",  1,  1)
set_floor("fl-abyss", 18,  1)
set_floor("fl-abyss",  1, 11)
set_floor("fl-abyss", 18, 11)

for i = 2, 17 do
   set_item("it-trigger",  i,  1, {action="callback", target="swT"})
   set_item("it-trigger",  i, 11, {action="callback", target="swB"})
end

for i = 2, 10 do
   set_item("it-trigger",  1,  i, {action="callback", target="swL"})
   set_item("it-trigger", 18,  i, {action="callback", target="swR"})
end

oxyd( 2, 0)
oxyd( 7, 0)
oxyd(12, 0)
oxyd(17, 0)
oxyd( 2,12)
oxyd( 7,12)
oxyd(12,12)
oxyd(17,12)
oxyd( 0, 3)
oxyd( 0, 9)
oxyd(19, 3)
oxyd(19, 9)
oxyd_shuffle() 

set_actor("ac-blackball", 10,6.5)

SetForce(2*Force, -2*Force)















