-- Gods of Enigma, a level for Enigma
-- Copyright (C) 2005 Moonpearl
-- Licensed under GPL v2.0 or above 
-- Reordering of the Landscapes by Raoul in 2006

Require("levels/lib/ant.lua")

function file_oxyd(x,y,f)
    oxyd_default_flavor=f
    oxyd(x,y)
end

levelh=61
levelw=58

enigma.FlatForce=30
enigma.SlopeForce=30
enigma.ElectricForce=30

cells={}
items={}
stones={}

stones[" "]=cell{}
cells[" "]=cell{}
items[" "]=cell{}

cells["!"]=cell{floor="fl-normal"}
--------------------------------------------------------------------
-- FLOORS
cells[" "]=cell{floor="fl-abyss"}
cells["+"]=cell{floor="fl-sand"}
cells["*"]=cell{floor="fl-water"}
cells[")"]=cell{floor="fl-light"}
cells["("]=cell{floor="fl-wood"}
cells["'"]=cell{floor="fl-leavesb"}
cells["&"]=cell{floor="fl-rough-red"}
cells["%"]=cell{floor="fl-bumps"}
cells["$"]=cell{floor="fl-rough-blue"}

level={"!!!              !!!                  %%%%%%%%%%%%%%%%%%%%",
       "!$!!!!      !!!!!!$!                  %%%%%%%%%%%%%%%%%%%%",
       "!$$$$!      !$$$$$$!                  %%%%%%%%%%%%%%%%%%%%",
       "!$!!!!!!!!  !$!!!!$!                  %%%%%%%%%%%%%%%%%%%%",
       "!!!   !$$!  !$!  !!!                  &%%%%%%%%%%%%%%%%%%%",
       "      !$$!  !$!                       %%%%%%%%%%%%%%%%%%%%",
       "      !!!!  !$!                       %%%%%%%%%%%%%%%%%%%%",
       "            !$!                       %%%%%%%%%%%%%%%%%%%%",
       "!!!         !$!  !!!                  %%%%%%%%%%%%%%%%%%%%",
       "!$  !!!!!!!!!$!!!!$!                  %%%%%%%%%%%%%%%%%%%%",
       "!$ $$$$$$$$$$$$$$$$!                  %%%%%%%%%%%%%%%%%%%%",
       "!$ !!!!!!!!!!!!!!!$!                  %%%%%%%%%%%%%%%%%%%%",
       "!!               !!!                  %%%%&%%%%%%%%%%%%%%%",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "''''''''''''''''''''                   ))))               ",
       "''' '' '' ''' ''   '                   )))) ))))     )))) ",
       "'''('' '' '''(''('''                   )))) ))))     )))) ",
       "'''        (     '''                   )))) )))) ))) )))) ",
       "'(  '' ''' ''('' '''                             )))      ",
       "'''('' ''' '' ''                       )))) )))) ))) )))) ",
       "'''        '' (  '''                   )))) )))) ))) )))) ",
       "' ( '''(''    ''(''!                   )))) ))))     )))) ",
       "''' ''' '' '' '' '''                             )))      ",
       "'''  (( (  ''  (   '                   )))) )))) ))) )))) ",
       "'    '' ''     ''' '                   )))) )))) ))) )))) ",
       "'''' '' ''(''' ''' '                   )))) )))) ))) )))) ",
       "''''''''''''''''''''                                 )))) ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                      ********************",
       "                                      ***********++++*****",
       "                                      **+++***+++++++++***",
       "                                      **+++***++++***++***",
       "                                      **+++**+++****++++**",
       "                                      **+++**++*++++++++**",
       "                                      **+++*++****+***++**",
       "                                      **++**+++******+++**",
       "                                      **++**+++*****++++**",
       "                                      **++****++***++++++*",
       "                                      ***+++++++***++++++*",
       "                                      *****++++*****+++***",
       "                                      ***************+****"}
--------------------------------------------------------------------
-- STONES
stones["!"]=cell{parent={{file_oxyd,"d"}}}
stones["."]=cell{stone="st-glass"}
stones["'"]=cell{stone="st-rock3_break"}
stones["("]=cell{stone="st-oneway-e"}
stones["+"]=cell{stone="st-bombs"}
stones[")"]=cell{stone="st-stone2"}
stones[","]=cell{stone="st-block"}
stones["*"]=cell{stone="st-oneway-s"}
stones["-"]=cell{stone="st-wood"}
stones["#"]=cell{stone="st-black1"}
stones["$"]=cell{stone="st-grate1"}
stones["%"]=cell{stone="st-white1"}
stones["&"]=cell{stone="st-yinyang1"}
stones["1"]=cell{stone="st-blue-sand"}
stones["2"]=cell{stone="st-knight"}
stones["/"]=cell{stone="st-death_invisible"}
stones["0"]=cell{stone="st-laser-e"}
stones["O"]=cell{stone="st-invisible"}

--      0    5    0    5    0    5    0    5    0    5    0    5 7
stmap={"!#$              $!$O                 ))))))))))))))))))))",--0
       "# $&            $$ $O                 ) )   '  )  '  )  !)",
       "$  &            $  (O                O( )   )  '  )  '   )",
       "% $&    !       $$ $O                 )*)   ))')  )))))'))",
       "!%$              $$$O                 )     )  )))))     )",
       "                    O                 )     )  )+++)     )",--5
       "                    O                 )     )'))+ +)))))))",
       "                    O                 )$$$  )  '+ +)  )  )",
       "$$$              $$$O                 )  $  )  )+++'  )  )",
       "$!$$            '$ $O                 )  $  )))))))))'))')",
       "$  $            '  $O                 )  $  '  )   )  '  )",--10
       "$ $$            '$!$O                 )  $  )  '   '  )  )",
       "$$$              $$$O                 ))))))))))))))))))))",
       "OOOOOOOOOOOOOOOOOOOOO                                     ",
       "                                                          ",
       "                                                          ",--15
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--20
       "                                                          ",
       "                                                          ",
       ",,,,,,,,,,,,,,,,,,,,,                OOOOOOOOOOOOOOOOOOOOO",
       ",,,,,,,,,,,,,,,,,,,,,                O ....               ",
       ",    -  -          ,,                O .  . ....     .... ",--25
       ",                  ,,                O .  . .!            ",
       ",            -     ,,                O .  . .    .        ",
       ",   -   -  -  -   -,,                O           .        ",
       ",                  ,,                O .  . .    .   .  . ",
       ",           -      ,,                O .  . .    .   .  . ",--30
       ",    -             ,,                O .  . .        .  . ",
       ",                  ,,                O             .      ",
       ",                  ,,                O .    ....   . .  . ",
       ",               -  ,,                O .           . .  . ",
       ",!   -             ,,                O ....        . .  . ",--35
       ",,,,,,,,,,,,,,,,,,,,,                O               .... ",
       ",,,,,,,,,,,,,,,,,,,,,                OOOOOOOOOOOOOOOOOOOOO",
       "                                                          ",
       "                                                          ",
       "                                                          ",--40
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--45
       "                                                          ",
       "                                                          ",
       "                                      ////////////////////",
       "                                      /     /            /",
       "                                      /     /            /",--50
       "                                      /     /  ///////   /",
       "                                      /     /  /!        /",
       "                                      /0   1/  /         /",
       "                                      /     /  /  ////   /",
       "                                      /     /  /  /      /",--55
       "                                      /     /  /  /      /",
       "                                      /     /  /  /222222/",
       "                                      /           /      /",
       "                                      /           /      /",
       "                                      ////////////////////"}--60
--------------------------------------------------------------------
--ITEMS
items["!"]=cell{item="it-extralife"}
items["#"]=cell{item="it-document"}
items["$"]=cell{item="it-dynamite"}
items["%"]=cell{item="it-hammer"}
items["&"]=cell{item="it-spade"}
items["'"]=cell{item="it-crack3"}
items["("]=cell{item="it-seed"}
items[")"]=cell{item="it-spring2"}
items["*"]=cell{item="it-spring1"}
items["+"]=cell{item="it-trigger"}
items[","]=cell{item="it-yinyang"}

--      0    5    0    5    0    5    0    5    0    5    0    5 7
itmap={"                                                          ",--0
       " !                                     #                  ",
       "                                                          ",
       " ,                                                        ",
       "        !                                                 ",
       "                                                     $$$  ",--5
       "                                                 %        ",
       "                                                       &$ ",
       "                                       $$              $  ",
       "                                       $$                 ",
       " !         #      !                    $$                 ",--10
       "                                       $$                 ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--15
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--20
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--25
       "   '         '  '                        #             )  ",
       "  #        '                             )                ",
       " '           '                                     )      ",
       "   '                                                      ",
       "              '                               )       )   ",--30
       "  '    '        '                       )                 ",
       " #      (                                         )       ",
       "     '' '      '                          )           *   ",
       "                                               )          ",
       "          '                                               ",--35
       "          '                                               ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--40
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--45
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                                          ",--50
       "                                         #                ",
       "                                                          ",
       "                                                          ",
       "                                                          ",
       "                                         +                ",--55
       "                                                          ",
       "                                                          ",
       "                                                     ,    ",
       "                                                          ",
       "                                                          "}--60
--------------------------------------------------------------------
--CREATE WORLD
create_world_by_map(level)
draw_map(0,0,stmap,stones)
draw_map(0,0,itmap,items)

--OXYDS
SetAttrib(enigma.GetStone(0,0),"color","0")
SetAttrib(enigma.GetStone(48,52),"color","0")
SetAttrib(enigma.GetStone(18,11),"color","1")
SetAttrib(enigma.GetStone(56,1),"color","1")
SetAttrib(enigma.GetStone(1,9),"color","2")
SetAttrib(enigma.GetStone(1,35),"color","2")
SetAttrib(enigma.GetStone(8,3),"color","3")
SetAttrib(enigma.GetStone(45,26),"color","3")
SetAttrib(enigma.GetStone(0,4),"color","6")
SetAttrib(enigma.GetStone(18,0),"color","6")

--TEXTE
SetAttrib(enigma.GetItem(11,10),"text","You again! We won't let you pass this time! See these Oxyds? You can't reach them unless you rob our Four Holy Items! But they are well guarded! Fwahahahaha!")

SetAttrib(enigma.GetItem(39,1),"text","Fire burns and destroys... Want to remove blocking stones? Then dig your way to the Holy Hammer!")

SetAttrib(enigma.GetItem(1,32),"text","Try again! Fwahahaha!")

SetAttrib(enigma.GetItem(2,27),"text","Earth fills and lasts... Want to replace missing floors? Then build your way to the Holy Seed!")

SetAttrib(enigma.GetItem(41,26),"text","Air blows and goes by... Want to jump across the void? Then make your way to the Holy Spring!")

SetAttrib(enigma.GetItem(41,51),"text","Water waves and shifts... Want to trade minds with your partner? Then find your way to the Holy Yin-Yang!")

--LASER
SetAttrib(enigma.GetStone(39,53),"init",FALSE)
SetAttrib(enigma.GetItem(41,55),"invisible",FALSE)
Signal ("it(41 55)","st(39 53)")

--TUNNELS
set_item("it-vortex-open", 49, 7, {targetx = 13.5, targety =  10.5, name="vortex0"})
set_item("it-vortex-open", 9, 31, {targetx = 13.5, targety =  10.5, name="vortex1"})
set_item("it-vortex-open", 55, 35, {targetx = 13.5, targety =  10.5, name="vortex2"})
set_item("it-vortex-open", 54, 58, {targetx = 13.5, targety =  10.5, name="vortex3"})

set_item("it-wormhole", 19, 2, {targetx = 38.5, targety =  2.5, name="vortex4"})
set_item("it-vortex-open", 18, 9, {targetx = 1.5, targety =  25.5, name="vortex5"})
set_item("it-vortex-open", 1, 11, {targetx = 40.5, targety =  25.5, name="vortex6"})
set_item("it-vortex-open", 7, 5, {targetx = 41.5, targety =  50.5, name="vortex7"})

-- ACTORS
set_actor("ac-whiteball",1.5,3.5)
set_actor("ac-blackball",13.5,10.5,{player=0})

