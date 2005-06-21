-- Hurry!
-- by Jens-Christian Korth


c = 0
inner = 0
total = 0

function sw1()
   if (inner == 0) then
      if (c == 0) then
         SendMessage("door", "open")
         c = 5
         total = total + 1
         if (total == 10) then
            total = 0
            SendMessage("fart", "trigger")
         end
      end
   else
      c = 1
   end
   inner = 0
end

function sw2()
   inner = 1
end

function nextsec()
   if (c > 0) then
      SendMessage("fakeox", "blink")
      c = c - 1
      if (c == 0) then
         SendMessage("door", "close")
      end
   end
end


create_world(20, 13) 
draw_border("st-invisible")
fill_floor("fl-stone")
fill_floor("fl-normal", 13,0,6,level_height)

draw_stones("st-rock1", {12,1}, {0,1}, 11)
draw_stones("st-rock1", {19,1}, {0,1}, 11)
draw_stones("st-rock1", {12,0}, {1,0}, 8)
draw_stones("st-rock1", {12,12}, {1,0}, 8)
set_stone("st-rock1", 11,5)
set_stone("st-rock1", 13,5)
set_stone("st-rock1", 11,7)
set_stone("st-rock1", 13,7)

set_stone("st-door", 12, 6, {name="door", type="v"})
set_floor("fl-gradient", 12, 6, {type=3})

set_item("it-trigger", 11, 6, {action="callback", target="sw1"})
set_item("it-trigger", 13, 6, {action="callback", target="sw2"})

set_stone("st-timer", 12,5, 
          {loop=0, action="callback", target="nextsec", interval=1})
set_stone("st-fart", 17,10, {name="fart"})
oxyd(14,2)
oxyd(17,2)
oxyd(14,10)
oxyd(17,6)
oxyd(2,2)
oxyd(6,2)
oxyd(10,2)
oxyd(6,6)
oxyd(2,10)
oxyd(2,6)
oxyd(6,10)
oxyd(10,10)
oxyd_shuffle() 

set_actor("ac-blackball", 10,6.5)