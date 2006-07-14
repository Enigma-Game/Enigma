-- Copyright (c) 2003 Jacob Scott
-- License: GPL v2.0 or above
-- Enigma Level: Drunk

levelw = 20
levelh = 13

create_world(levelw, levelh)

fill_floor("fl-hay")

set_stone("st-greenbrown", 3, 5)

set_stone("st-brownie", 1, 0)
set_stone("st-brownie", 3, 0)
set_stone("st-brownie", 4, 0)
set_stone("st-brownie", 5, 0)
set_stone("st-brownie", 6, 0)
set_stone("st-brownie", 7, 0)
set_stone("st-brownie", 8, 0)
set_stone("st-brownie", 9, 0)
set_stone("st-brownie", 10, 0)
set_stone("st-brownie", 11, 0)
set_stone("st-brownie", 12, 0)
set_stone("st-brownie", 13, 0)
set_stone("st-brownie", 14, 0)
set_stone("st-brownie", 15, 0)
set_stone("st-brownie", 16, 0)
set_stone("st-brownie", 17, 0)
set_stone("st-brownie", 18, 0)
set_stone("st-brownie", 19, 0)

set_stone("st-brownie", 1, 12)
set_stone("st-brownie", 3, 12)
set_stone("st-brownie", 4, 12)
set_stone("st-brownie", 5, 12)
set_stone("st-brownie", 6, 12)
set_stone("st-brownie", 7, 12)
set_stone("st-brownie", 8, 12)
set_stone("st-brownie", 9, 12)
set_stone("st-brownie", 10, 12)
set_stone("st-brownie", 11, 12)
set_stone("st-brownie", 12, 12)
set_stone("st-brownie", 13, 12)
set_stone("st-brownie", 14, 12)
set_stone("st-brownie", 15, 12)
set_stone("st-brownie", 16, 12)
set_stone("st-brownie", 17, 12)
set_stone("st-brownie", 18, 12)
set_stone("st-brownie", 19, 12)

set_stone("st-brownie", 19, 1)
set_stone("st-brownie", 19, 2)
set_stone("st-brownie", 19, 3)
set_stone("st-brownie", 19, 4)
set_stone("st-brownie", 19, 5)
set_stone("st-brownie", 19, 6)
set_stone("st-brownie", 19, 7)
set_stone("st-brownie", 19, 8)
set_stone("st-brownie", 19, 9)
set_stone("st-brownie", 19, 10)
set_stone("st-brownie", 19, 11)
set_stone("st-brownie", 19, 12)

set_stone("st-brownie", 0, 1)
set_stone("st-brownie", 0, 2)
set_stone("st-brownie", 0, 3)
set_stone("st-brownie", 0, 4)
set_stone("st-brownie", 0, 5)
set_stone("st-brownie", 0, 6)
set_stone("st-brownie", 0, 7)
set_stone("st-brownie", 0, 8)
set_stone("st-brownie", 0, 9)
set_stone("st-brownie", 0, 10)
set_stone("st-brownie", 0, 11)
set_stone("st-brownie", 0, 12)

oxyd(1,2)
oxyd(1,4)
oxyd(1,6)
oxyd(1,7)
oxyd(1,9)
oxyd(1,11)
oxyd(1,1)
oxyd(18,1)
oxyd_shuffle()

fakeoxyd(1,3)
fakeoxyd(1,5)
fakeoxyd(1,8)
fakeoxyd(1,10)

abyss(7,7)
abyss(7,8)
abyss(7,9)
abyss(12,1)
abyss(10,11)
abyss(12,2)
abyss(13,3)
abyss(12,3)
abyss(9,5)
abyss(11,6)

abyss(7,11)
abyss(13,9)

set_stone("st-death", 10, 8)
set_stone("st-death", 10, 9)

set_stone("st-death", 19, 12)
set_stone("st-death", 18, 12)
set_stone("st-death", 17, 12)
set_stone("st-death", 19, 11)
set_stone("st-death", 19, 10)


abyss(2,0)
abyss(2,12)

set_stone("st-death", 5, 2)
set_stone("st-death", 5, 3)
set_stone("st-death", 5, 1)
set_stone("st-death", 5, 7)
set_stone("st-death", 5, 8)
set_stone("st-death", 5, 9)
set_stone("st-death", 5, 10)
set_stone("st-death", 5, 11)
set_stone("st-death", 5, 12)
set_stone("st-greenbrown", 5, 6)
set_stone("st-greenbrown", 5, 4)
set_stone("st-door", 5, 5 , {name="door2", type="v"})

mytimer = 0
--x=-20
--y=0
function timer_handler()
    mytimer=mytimer+1
    if mytimer==1 then
	x=20
	y=20
    end
    if mytimer==2 then
	x=-20
	y=-20
    end
    if mytimer==3 then
	x=20
	y=-20    
    end
    if mytimer==4 then
	x=0
	y=0
	x=-20
	y=20
	SendMessage("door2", "openclose")
	mytimer=0
    end
    enigma.AddConstantForce(x,y)
end
  

set_stone( "st-timer", 0, 0, {action="callback", target="timer_handler"})

set_actor("ac-blackball" ,17.5, 2)

















