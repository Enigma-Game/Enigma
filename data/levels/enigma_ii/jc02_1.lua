-- Rollercoaster
-- by Jens-Christian Korth
-- 2003-06-24: [ant]: changes in AddConstantForce

Force = 10

CurrentCheckPoint = 4
LastCheckPoint = 0

function SetForce(x,y)
   enigma.AddConstantForce(x,y)
end

function CheckPoint(CP)
   if (not (CurrentCheckPoint == CP)) then
      LastCheckPoint = CurrentCheckPoint
      CurrentCheckPoint = CP
      Force = Force + 1
   end
end

function sw1()
   CheckPoint(1)
   if (LastCheckPoint == 4) then
      SetForce(0, -Force)
   elseif (LastCheckPoint == 6) then
      SetForce(Force, 0)
   end
end

function sw2()
   CheckPoint(2)
   if (LastCheckPoint == 3) then
      SetForce(0, Force)
      SendMessage("doorL1", "open")
      SendMessage("doorL2", "close")
      SendMessage("doorL3", "open")
   else
      SetForce(Force, 0)
   end
end

function sw3()
   CheckPoint(3)
   if (LastCheckPoint == 4) then
      SetForce(-Force, 0)
   else
      SetForce(0, Force)
      SendMessage("doorR1", "open")
      SendMessage("doorR2", "close")
      SendMessage("doorR3", "open")
   end
end

function sw4()
   CheckPoint(4)
   if (LastCheckPoint == 1) then
      SetForce(0, -Force)
   elseif (LastCheckPoint == 5) then
      SetForce(-Force, 0)
   end
end

function sw5()
   CheckPoint(5)
   if (LastCheckPoint == 4) then
      SetForce(-Force, 0)
   else
      SetForce(0, -Force)
      SendMessage("doorL1", "open")
      SendMessage("doorL2", "open")
      SendMessage("doorL3", "close")
      SendMessage("doorR1", "close")
      SendMessage("doorR2", "open")
      SendMessage("doorR3", "open")
   end
end

function sw6()
   CheckPoint(6)
   if (LastCheckPoint == 5) then
      SetForce(0, -Force)
      SendMessage("doorL1", "close")
      SendMessage("doorL2", "open")
      SendMessage("doorL3", "open")
      SendMessage("doorR1", "open")
      SendMessage("doorR2", "open")
      SendMessage("doorR3", "close")
   else
      SetForce(Force, 0)
   end
end

create_world(20, 13) 
draw_border("st-rock1")
fill_floor("fl-stone")

draw_stones("st-rock1", {2,2}, {1,0}, 16)
draw_stones("st-rock1", {2,5}, {1,0}, 16)
draw_stones("st-rock1", {2,7}, {1,0}, 16)
draw_stones("st-rock1", {2,10}, {1,0}, 16)
draw_stones("st-rock1", {2,2}, {0,1}, 9)
draw_stones("st-rock1", {17,2}, {0,1}, 9)

doorh( 1, 5, {name="doorL1"})
doorv( 2, 6, {name="doorL2"})
doorh( 1, 7, {name="doorL3"})
doorh(18, 5, {name="doorR1"})
doorv(17, 6, {name="doorR2"})
doorh(18, 7, {name="doorR3"})

set_item("it-trigger",  1,  6, {action="callback", target="sw1"})
set_item("it-trigger",  1,  1, {action="callback", target="sw2"})
set_item("it-trigger", 18,  1, {action="callback", target="sw3"})
set_item("it-trigger", 18,  6, {action="callback", target="sw4"})
set_item("it-trigger", 18, 11, {action="callback", target="sw5"})
set_item("it-trigger",  1, 11, {action="callback", target="sw6"})

oxyd( 2, 2)
oxyd( 7, 2)
oxyd(12, 2)
oxyd(17, 2)
oxyd( 2, 5)
fakeoxyd( 7, 5)
fakeoxyd(12, 5)
oxyd(17, 5)
oxyd( 2, 7)
fakeoxyd( 7, 7)
fakeoxyd(12, 7)
oxyd(17, 7)
oxyd( 2,10)
oxyd( 7,10)
oxyd(12,10)
oxyd(17,10)
oxyd_shuffle() 

set_actor("ac-blackball", 10,6.5)

SendMessage("doorL1", "open")
SendMessage("doorL2", "open")
















