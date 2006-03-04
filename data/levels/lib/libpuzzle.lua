-- libpuzzle
-- Copyright (C) 2005 Raoul
-- Licensed under GPL v2.0 or above
-- Version 0.4

--Dies ist eine im Aufbau begriffene Lua-Lib die alle allgemein definierten Funktionen enthaelt, die mit Puzzlestones zu tun haben

-- benutzen kann man diese lib so:
-- Das .lua File unter /PFAD/ZU/ENIGMA/levels/lib/libpuzzle.lua speichern.
-- Am Anfang des Levels dann folgendes einfuegen:
-- Require("levels/lib/libpuzzle.lua")
-- Danach kann man im Levels selbst auf alle hier definierten Funktionen und Variablen zugreifen.

-- Die Funktion der libpuzzle sind in der Lage, puzzlesteine einer Figur bei jedem aufruf neu und zufaellig zu verteilen.
-- Es kann vorkommen, dass alle Steine schon richtig zu liegen kommen, aber die Wahrscheinlichkeit nimmt mit der Groesse der Figur stark ab:
-- sie betraegt: 1/((anz!)/(anzgleichedersortea!*anzgleichedersorteb!*...))
--fuer einen 8er Ring sieht die Berechnung so aus: 1/((8!)/(2!*2!)) = 1/10080

--Da die Schalter keine Uebergabe von Argumenten erlauben, sind dann jeweils wrapperfunktionen noetig.

------------------------------------------------------------------------
-- TEIL()
-- Die Funktion teil() verteilt Puzzlesteine zufaellig
-- Input Daten:
-- welche Teile duerfen vorkommen: (Werte: alle Substrings von "nesw", achtung: "en" ist ungueltig, es muss "ne" heissen!)
-- bsp: teile={"sw","ns","nw","ew","ne","ns","es","ew"}
-- Die x und y Koordinaten aller Steine: (Format: {{x1,x2,x3...},{y1,y2,y3...}})
-- bsp: orte={{17,17,17,16,15,15,15,16},{8,9,10,10,10,9,8,8}}
-- Anzahl Steine:
-- bsp: anz=8
-- Art der Steine: (Wert: "" oder "2")
-- bsp: art="2"

function teil(anz,orte,teile,art)
 restteile={}
 zyklen=anz
 for i=1,zyklen do
  t=random(1,anz)
  aktteil=teile[t]

  set_stone("st-puzzle"..art.."-"..aktteil, orte[1][i],orte[2][i])

  restteile={}
  schogse=0

  for k=1,anz do
   if teile[k]==aktteil and schogse==0 then
    restteile[k]="0"
    schogse=1
   else
    restteile[k]=teile[k]
   end
  end

  teile={}
 
  z2=1
  for j=1,anz do
   if restteile[j]~="0" then
    teile[z2]=restteile[j]
    z2=z2+1
   end 
  end

  anz=anz-1
 end
end

------------------------------------------------------------------------
-- MATRIX2PLACES()
-- ermittelt aus einer matrix die absoluten Orte der Teile
-- Input Daten:
-- Verteilung im Sinne von wo hat es einen Stein
-- 1=Stein, 0=kein Stein
-- Beispiel:
-- |  #|
-- | ##|
-- |## |
-- ergibt diese Matrix:
-- matrix={{0,0,1},{0,1,1},{1,1,0}}
-- Die Anzahl Spalten der Matrix
-- xdim=3
-- Die Anzahlt Reihen der Matrix:
-- ydim=3
-- Die Absolute X-Koordinate des Linken Oberen Feldes Der Matrix:
-- xstart=8
-- Die Absolute Y-Koordinate des Linken Oberen Feldes Der Matrix:
-- ystart=2

-- Output Daten:
-- Die x und y Koordinaten aller Steine: (Format: {{x1,x2,x3...},{y1,y2,y3...}})
-- diese Variable kann als Input fuer die Funktion teil() gebraucht werden.
-- bsp: orte={{17,17,17,16,15,15,15,16},{8,9,10,10,10,9,8,8}}
-- locs{}

-- Die Anzahl der wirklich vorhandenen Steine
-- anzteile

function matrix2places(matrix,xdim,ydim,xstart,ystart)
locs={{},{}}
zahler=1
anzteile=0

 for i=1,ydim do
  for j=1,xdim do
   if matrix[i][j]==1 then
    locs[1][zahler]=xstart+j-1
    locs[2][zahler]=ystart+i-1
    zahler=zahler+1
   end
  end
 end

 anzteile=zahler-1
end

------------------------------------------------------------------------
-- WHICH_PIECE()
-- ermittelt aus einer matrix die benoetigten Teile.
-- die testen-w() sind lediglich hilfsfunktionen.

-- Input Daten:
-- Verteilung im Sinne von wo hat es einen Stein
-- 1=Stein, 0=kein Stein
-- matrix={{0,0,1},{0,1,1},{1,1,0},{1,0,0}}
-- Die Anzahl Spalten der Matrix
-- xdim=3
-- Die Anzahlt Reihen der Matrix:
-- ydim=4

-- Output Daten:
-- liefert die Teile, die noetig sind um die Figur bauen zu koennen
-- teile={"n","ne","sw"}

function which_piece(matrix,xdim,ydim)
teile={}
oben=""
links=""
unten=""
rechts=""
zahler=1

 for i=1,ydim do
  for j=1,xdim do
   if matrix[i][j]==1 then
      if i==1 then
       if j==1 then
        oben=""
        links=""
        teste(matrix,j,i) 
        tests(matrix,j,i) 
       elseif j==xdim then
        oben=""
        rechts=""
        tests(matrix,j,i) 
        testw(matrix,j,i) 
       else
        oben=""
        teste(matrix,j,i) 
        tests(matrix,j,i) 
        testw(matrix,j,i) 
       end  
      elseif i==ydim then
       if j==1 then
        unten=""
        links=""
        testn(matrix,j,i) 
        teste(matrix,j,i) 
       elseif j==xdim then
        unten=""
        rechts=""
        testn(matrix,j,i) 
        testw(matrix,j,i) 
       else
        unten=""
        testn(matrix,j,i) 
        teste(matrix,j,i) 
        testw(matrix,j,i) 
       end  
      else
       if j==1 then
        links=""
        testn(matrix,j,i) 
        teste(matrix,j,i)
        tests(matrix,j,i)  
       elseif j==xdim then
        rechts=""
        testn(matrix,j,i) 
        tests(matrix,j,i) 
        testw(matrix,j,i) 
       else
        testn(matrix,j,i) 
        teste(matrix,j,i) 
        tests(matrix,j,i) 
        testw(matrix,j,i) 
       end  
      end

      -- Damit auch bei einem Stein ohne Nachabren ein valider Stein entsteht, wird er als Kreuz definiert
      if oben=="" and rechts=="" and unten=="" and links=="" then
       oben="n"
       rechts="e"
       unten="s"
       links="w"
      end

      teile[zahler]=oben..rechts..unten..links
      zahler=zahler+1
   end
  end
 end
end

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

------------------------------------------------------------------------
-- DRAW_DIRECT()
-- Diese Funktion ist zum direkten Zeichnen von Steinen ohne vorherige durchmischung.
-- Input Daten:
-- welche Teile duerfen vorkommen: (Werte: alle Substrings von "nesw", achtung: "en" ist ungueltig, es muss "ne" heissen!)
-- bsp: teile={"sw","ns","nw","ew","ne","ns","es","ew"}
-- Die x und y Koordinaten aller Steine: (Format: {{x1,x2,x3...},{y1,y2,y3...}})
-- bsp: orte={{17,17,17,16,15,15,15,16},{8,9,10,10,10,9,8,8}}
-- Anzahl Steine:
-- bsp: anz=8
-- Art der Steine: (Wert: "" oder "2")
-- bsp: art="2"

-- Werden die Werte anz, orte und teile direkt von matrix2places() und which_piece() uebernommen, so ergibt sich aus einer Matrix ein schon geloestes Puzzle.

function draw_direct(anz,orte,teile,art)
 for i=1,anz do
  set_stone("st-puzzle"..art.."-"..teile[i], orte[1][i],orte[2][i])
 end
end

------------------------------------------------------------------------












