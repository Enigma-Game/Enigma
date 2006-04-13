-- Puzzle Demo, a tech-demonstration for Enigma
-- Copyright (C) 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above 
-- use with libpuzzle >0.95

-- GENERAL --
Require("levels/lib/libpuzzle.lua")
levelw = 39
levelh = 25


create_world(levelw, levelh)

function renderLine( line, pattern)
	for i=1, strlen(pattern) do
		local c = strsub( pattern, i, i)
  		if c=="x" then
			--oxyd(i-1,line)
                elseif c=="#" then
                   set_stone("st-glass",i-1,line)
                   set_floor("fl-sahara",i-1,line)
                elseif c==" " then
                   set_floor("fl-sahara",i-1,line)
		end
	end	
end

renderLine(00 , "#######################################")
renderLine(01 , "#                  #                  #")
renderLine(02 , "#                  #                  #")
renderLine(03 , "#                  #                  #")
renderLine(04 , "#                                     #")
renderLine(05 , "#                                     #")
renderLine(06 , "#                                     #")
renderLine(07 , "#                                     #")
renderLine(08 , "#                                     #")
renderLine(09 , "#                  #                  #")
renderLine(10 , "#                  #                  #")
renderLine(11 , "#                  #                  #")
renderLine(12 , "######        ###########        ######")
renderLine(13 , "#                  #                  #")
renderLine(14 , "#                  #                  #")
renderLine(15 , "#                  #                  #")
renderLine(16 , "#                                     #")
renderLine(17 , "#                                     #")
renderLine(18 , "#                                     #")
renderLine(19 , "#                                     #")
renderLine(20 , "#                                     #")
renderLine(21 , "#                  #                  #")
renderLine(22 , "#                  #                  #")
renderLine(23 , "#                  #                  #")
renderLine(24 , "#######################################")

oxyd_shuffle()

-- Special
set_item("it-document", 3, 8, {text="Welcome to the demonstration of libpuzzle version 0.96"})
set_item("it-document", 3, 9, {text="Please also look at the source code...it's very easy :)"})
set_actor("ac-blackball", 2.5,7.5, {player=0}) 
set_item("it-magicwand", 2, 7) 

set_item("it-document", 11, 7, {text="Room 1: Some general things you can do with libpuzzle"})
set_item("it-document", 8, 18, {text="The red puzzles, generated with permutations, are ALL solvable."})
set_item("it-document", 11, 18, {text="The blue ones, generated with random, aren't!"})
set_item("it-document", 28, 8, {text="Room 2: Every figure in here, the +/T/L, all are solvable because they are generated  from the solved puzzle with permutations."})
set_item("it-document", 23, 16, {text="Room 4: Some things, you can do with pseude pieces: open puzzle clusters..."})


--first room:
--rings, treppe, figure1...
--shows some general things

  --ring1
  must_shuffle=0
  puzzle({{1,1,1},{1,0,1},{1,1,1}},2,2,"2")

  puzzle({{1,1,1,0},{1,0,0,1},{1,0,1,1},{1,1,1,0}},10,6,"2")

  must_shuffle=1
  puzzle({{1,0,0},{1,1,0},{0,1,1}},10,2,"2")

  must_shuffle=0
  puzzle({{1,1,1},{1,1,1},{1,1,1}},14,2,"2")

  puzzle({{0,0,1,0,0},{0,1,1,1,0},{1,1,0,1,1}},14,9,"")

  puzzle({{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,1,1,1},{0,0,0,1},{1,1,1,1}},5, 4,"")

--second room:
--every puzzle here is solvable, generated with permutations
  must_shuffle=1
  puzzle({{0,1,0},{1,1,1},{0,1,0}},21,1,"2")
  puzzle({{0,1,0},{1,1,1},{0,1,0}},25,1,"2")
  puzzle({{0,1,0},{1,1,1},{0,1,0}},30,1,"2")
  puzzle({{0,1,0},{1,1,1},{0,1,0}},34,1,"2")

  puzzle({{1,1,1},{0,1,0},{0,1,0}},21,5,"2")
  puzzle({{1,1,1},{0,1,0},{0,1,0}},25,5,"2")
  puzzle({{1,1,1},{0,1,0},{0,1,0}},30,5,"2")
  puzzle({{1,1,1},{0,1,0},{0,1,0}},34,5,"2")

  puzzle({{1,0,0},{1,0,0},{1,1,1}},21,9,"2")
  puzzle({{1,0,0},{1,0,0},{1,1,1}},25,9,"2")
  puzzle({{1,0,0},{1,0,0},{1,1,1}},30,9,"2")
  puzzle({{1,0,0},{1,0,0},{1,1,1}},34,9,"2")


--third room:
--shows the difference between random and permutation shuffle.

  must_shuffle=1
  shuffle_method="random"
  puzzle({{1},{1},{1}},11,20,"")
  puzzle({{1},{1},{1},{1}},13,19,"")
  puzzle({{1},{1},{1},{1},{1}},15,18,"")
  puzzle({{1},{1},{1},{1},{1},{1}},17,17,"")

  shuffle_method="permutation"
  puzzle({{1},{1},{1}},8,14,"2")
  puzzle({{1},{1},{1},{1}},6,14,"2")
  puzzle({{1},{1},{1},{1},{1}},4,14,"2")
  puzzle({{1},{1},{1},{1},{1},{1}},2,14,"2")

--4. room:
--shows the wonderfull possibilities of pseudo_pieces.

  must_shuffle=0
  puzzle({{0,2,0,2,0},{2,1,1,1,2},{0,1,0,1,0},{2,1,1,1,2},{0,2,0,2,0}},21,14,"")

  puzzle({{1,1,2,1},{1,0,0,1},{1,2,1,1}},26,20,"")

  puzzle({{1,1,1},{1,2,1},{1,1,1}},27,14,"2")

  puzzle({{1,1,1,1},{2,0,0,1},{0,2,0,1},{1,1,1,1},{0,1,0,0},{0,1,0,2},{2,1,1,1}},32,15,"")
