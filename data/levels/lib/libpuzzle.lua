-- libpuzzle, a library for enigma
-- Copyright (C) 2005, 2006 Raoul Bourquin
-- Licensed under GPL v2.0 or above
-- Version 0.9

-- This is a lua-library to make it very easy to set up random puzzles.

-- Use this lib this as showed:
-- Save the .lua in /PFAD/TO/ENIGMA/data/levels/lib/libpuzzle.lua
-- On the beginning of your level, just include:
-- Require("levels/lib/libpuzzle.lua")
-- Now you can use every function here, but usually you would just call "puzzle(YOUR OPTIONS)"

------------
--Reference:
------------
--Example: to generate a Ring with 8 Stones (the red ones) at the Position x=4/y=5:
--puzzle({{1,1,1},{1,0,1},{1,1,1}},2,4,"2")
--in [], this are the values from the example above.

--Matrix: this is the abstract definition of the puzzle:
--###
--# # this ring has the matrix: {{1,1,1},{1,0,1},{1,1,1}}
--###
--The format is: {row1, row2, row3, ...}
--where row1 is:{stone1, stone2, ston3, ...}
--[{{1,1,1},{1,0,1},{1,1,1}}]

--anz_stones: this in the number stones we need. 
--[8]

--stone_coordinates: this 2D Array keeps the coordinates of the stones.
--Format: {{X-Values},{Y-Values}}
--{{x-first-stone, xsecond-stone, ...},{y-first-stone, y-second-stone, ...}}
--[{{2,3,4,2,4,2,3,4},{4,4,4,5,5,6,6,6}}]

--xtopleftcorner/ytopleftcorner or xcorner/ycorner: the absolute coordinates of the top left corner of your puzzle-matrix.
--It's not required that this is really a stone. 
--[2 and 4]

--teile: this array keeps all strings that describes the different stones used. 
--[{"es","ew","sw","ns","ns","ne","ew","nw"}]

--shuffled_pieces: this array keeps the mix descriptions. 
--[no values, it's random!]

--art: this string describes if we use the blue or the red puzzle stones
--the values are: "" for blue and "2" for red ! 
--["2"]

--now, the syntax for a puzzle is:
--puzzle(matrix, xtopleftcorner, ytopleftcorner, art)

-- it's easy, isn't it ?

---------------------------------------------------------------
--WORLD:
--This are global variables. They determine the exact behavior of the puzzle function.

--must we shuffle the pieces or not ?
--1 means true, 0 means false.
--overwrite this in your level to get already solved puzzles.
must_shuffle=1

--which method to shuffle:
--"random" or "permutation"
shuffle_method="random"

--with how many permutations we shuffle:
--for bigger puzzles, take bigger values!
--not yet used
anz_permutations=10


---------------------------------------------------------------
--HELPER:

--Determine the length of an array:
function arraydim(array)
 local i=1

 while array[i]~=nil do
  i=i+1
 end

 array_length=i-1
 return array_length
end

--Matrix is rectangular ?
--not yet used

function test_matrix(matrix)

 local rows=arraydim(matrix)
 local okrows=0
 local good_length=arraydim(mastone_coordinatestrix[i])

 for i=1,rows do
  local_row=arraydim(matrix[i])
  if local_row~=good_length then
   error="Matrix is not rectangular !"
  end
 end
 return error
end

---------------------------------------------------------------
--WRAPPER:
--The "normal" User of libpuzzle would call this function only.
function puzzle(matrix, xtopleftcorner, ytopleftcorner, art)

 --call the matrix2places:
 matrix2places(matrix, xtopleftcorner, ytopleftcorner)

 --call the which_piece ti determine the pieces we need
 which_piece(matrix)

 --shuffle the pieces
 if must_shuffle==1 then
   puzzle_shuffle(teile)
 elseif must_shuffle==0 then
  --to produce a shuffled_pieces array. But the pieces are NOT shuffled.
  shuffled_pieces=teile
 end

 --draw the puzzle
 draw_pieces(stone_coordinates, shuffled_pieces, art)

 return 0
end

---------------------------------------------------------------
--INPUT_PARSER:

--TODO a function that checks the inputvalues!

--Determine the real coordinates of the stones
function matrix2places(matrix,xcorner,ycorner)

 stone_coordinates={{},{}}

 local i,j
 local counter=1

 local xdim=arraydim(matrix[1])
 local ydim=arraydim(matrix)

  for i=1,ydim do
   for j=1,xdim do
    if matrix[i][j]==1 then
     stone_coordinates[1][counter]=xcorner+j-1
     stone_coordinates[2][counter]=ycorner+i-1
     counter=counter+1
    end
   end
  end
 --number of stones:
 anz_stones=arraydim(stone_coordinates[1])

 return stone_coordinates,anz_stones
end


--Determine the kind of the stones
function which_piece(matrix)

 teile={}

 local xdim=arraydim(matrix[1])
 local ydim=arraydim(matrix)

 local i,j

 --these should be local:
 oben=""
 links=""
 unten=""
 rechts=""
 
 local counter=1

 for i=1,ydim do
  for j=1,xdim do
   if matrix[i][j]==1 then
      if i==1 then
       oben=""
       if ydim>1 then
        tests(matrix,j,i)
       end 
      elseif i==ydim then
       unten=""
       if ydim>1 then
        testn(matrix,j,i) 
       end
      else
       testn(matrix,j,i) 
       tests(matrix,j,i) 
      end 
 
      if j==1 then
       links=""
       if xdim>1 then
        teste(matrix,j,i) 
       end
      elseif j==xdim then
       rechts=""
       if xdim>1 then
        testw(matrix,j,i) 
       end
      else
       teste(matrix,j,i) 
       testw(matrix,j,i) 
      end  

      -- To get a valid Stone if no neigbours were present, we define it as the cross ("nesw")
      if oben=="" and rechts=="" and unten=="" and links=="" then
       oben="n"
       rechts="e"
       unten="s"
       links="w"
      end

      teile[counter]=oben..rechts..unten..links
      counter=counter+1
   end
  end
 end

 return teile

end

--Helperfunction for testing the required connection of a puzzlestone:
function testn(matrix,posx,posy)
 if matrix[posy-1][posx]==1 then
  oben="n"
 else
  oben=""
 end
end

function teste(matrix,posx,posy)
 if matrix[posy][posx+1]==1 then
  rechts="e"
 else
  rechts=""
 end
end

function tests(matrix,posx,posy)
 if matrix[posy+1][posx]==1 then
  unten="s"
 else
  unten=""
 end
end

function testw(matrix,posx,posy)
 if matrix[posy][posx-1]==1 then
  links="w"
 else
  links=""
 end
end

---------------------------------------------------------------
--RANDOM SHUFFLE

--shuffle the array teile, this is the classical method.
--its not guaranted to get a solvable puzzle every time!
function puzzle_shuffle(teile)
 
 shuffled_pieces={}

 local restteile={}
 local anz=anz_stones
 local zyklen=anz
 local i,j,k
 local counter=1

 for i=1,zyklen do

  --shuffle pieces:
  t=random(1,anz)
  
  aktteil=teile[t]
  shuffled_pieces[counter]=aktteil
  counter=counter+1

  --prepare the teile array for next cycle
  local restteile={}
  local schogse=0

  --copy the teile array, mark the piece we just have used with "0"
  for k=1,anz do
   if teile[k]==aktteil and schogse==0 then
    restteile[k]="0"
    schogse=1
   else
    restteile[k]=teile[k]
   end
  end

  --clear teile array:
  teile={}
  local t=1

  for j=1,anz do
   if restteile[j]~="0" then
    teile[t]=restteile[j]
    t=t+1
   end 
  end
  
  --we have used one piece:
  anz=anz-1
 end

 return shuffled_pieces

end

---------------------------------------------------------------
--OUTPUT:

--Draw the Puzzlestones:
function draw_pieces(stone_coordinates,shuffled_pieces,art)

 local i
 local anz=anz_stones

  for i=1,anz do
  set_stone("st-puzzle"..art.."-"..shuffled_pieces[i], stone_coordinates[1][i], stone_coordinates[2][i])
 end
end

---------------------------------------------------------------
--END OF THE LIBPUZZLE
