------------------------------------------------------------------------
-- Copyright (C) 2008 Ronald Lamprecht
--
-- This program is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License
-- as published by the Free Software Foundation; either version 2
-- of the License, or (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License along
-- with this program; if not, write to the Free Software Foundation, Inc.,
-- 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
--
------------------------------------------------------------------------

-------------------------------------------------
-- Level API 2 as of Enigma 1.10 compatibility --
-------------------------------------------------


-----------------------------------------------
-- Use Enigma internal random implementation --
-----------------------------------------------

math.random = en.random
random = math.random
-- randseed is issued by Enigma application and must not be disturbed
math.randomseed = function () end

-----------------------------
-- old stuff to be checked --
-----------------------------

-- TODO remove Tick
function Tick (deltatime)
    -- do nothing by default
end



function PrintTable(t)
    for i,v in pairs(t) do
        if type(v)=="table" then
            print (i.." - "..v[1]..","..v[2])
        else
            print (i.." - "..v)
        end
    end
end


function Require(filename)
    enigma.LoadLib(string.sub(filename,8,string.len(filename)-4))
end

---------------
-- Constants --
----------------

-- state
OFF      = 0
ON       = 1
CLOSED   = 0
OPEN     = 1
OXYDPAIR = 2

-- color
BLACK = 0
WHITE = 1
NOCOLOR = nil

-- rotation direction
CW  = 0
CCW = 1

-- oxyd color
OXYD_BLUE   =  0
OXYD_RED    =  1
OXYD_GREEN  =  2
OXYD_YELLOW =  3
OXYD_CYAN   =  4
OXYD_PURPLE =  5
OXYD_WHITE  =  6
OXYD_BLACK  =  7
OXYD_AUTO   = -1
OXYD_FAKE   = -2
OXYD_FART   = -3
OXYD_BOLD   = -4

-- direction
WEST  = 0
SOUTH = 1
EAST  = 2
NORTH = 3

-- essential
DISPENSIBLE   = 0
INDISPENSIBLE = 1
PERKIND       = 2

---------------------
-- Utility Methods --
---------------------

function drawmap(world, resolver, origin, ignore, map)
    local len = string.len(ignore)
    for y=1, #map do
        local linelen = string.len(map[y])
        if math.fmod(linelen, len) ~= 0 then
            error("drawmap map line ".. y .. " with odd length", 2)
        end
        for x = 1, linelen/len do
            local key = string.sub(map[y], len*(x-1)+1, len*x)
            if key ~= ignore then
                world[origin + {x-1, y-1}] =  
                        world:_evaluate(resolver, key, origin.x + x - 1, 
                                origin.y + y-1)
            end
        end
    end
end

wo:_register("drawmap", drawmap)

---------------
-- Resolvers --
---------------

res = {}

function res.random_implementation(context, evaluator, key, x, y)
    for hit_itr, hit_pair in ipairs(context[3]) do
        if key == hit_pair[1] then
            local super = nil
            if hit_pair[2] ~= nil then
                super = evaluator(context[2], hit_pair[2], x, y)
            end
            local repl_key
            local r = math.random(context[5])
            local i = 1
            local s = context[4][i][2]
            while s < r do
                i = i + 1
                s = s + context[4][i][2]
            end
            repl_key = context[4][i][1]
            local replacement = evaluator(context[2], repl_key, x, y)
            if super == nil then
                return replacement
            elseif type(super) == "table" then
                return ti(super) .. replacement
            else
                return super .. replacement
            end
        end
    end
    -- key not found
    return evaluator(context[2], key, x, y)
end

function res.random(subresolver, hits, replacements)
    -- syntax: hits = key | {key, [key]*, [{key, superkey}]*}
    --         replacements = {key, [key]*, [{key, frequency}]*}
    -- TODO check args!
    
    local hit_table = {}
    if type(hits) == "string" then
        hit_table[1] = {hits, nil}
    else
        for i, v in ipairs(hits) do
            if type(v) == "string" then
                hit_table[i] = {v, nil}
            elseif type(v) == "table" then
                hit_table[i] = v
            end
        end
    end
    
    local repl_table = {}
    for i, v in ipairs(replacements) do
        if type(v) == "string" then
            repl_table[i] = {v, 1}
        elseif type(v) == "table" then
            repl_table[i] = v
        end
    end
    local repl_sum = 0
    for i, v in ipairs(repl_table) do
        repl_sum = repl_sum + v[2]
    end
    local context = {res.random_implementation, subresolver, hit_table, 
                      repl_table, repl_sum}
    return context
end


