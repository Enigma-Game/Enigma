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
OXYD_GREY   =  8
OXYD_ORANGE =  9
OXYD_PINE   = 10
OXYD_BROWN  = 11
OXYD_AUTO   = -1
OXYD_FAKE   = -2
OXYD_FART   = -3
OXYD_BOLD   = -4

-- direction
WEST  = 0
SOUTH = 1
EAST  = 2
NORTH = 3

-- alternative orientation notation for mirrors
BACKSLASH  = 0
HORIZONTAL = 1
SLASH      = 2
VERTICAL   = 3

-- multidirections as used by st_chess
NNE = po(1, -2)
NEE = po(2, -1)
SEE = po(2, 1)
SSE = po(1, 2)
SSW = po(-1, 2)
SWW = po(-2, 1)
NWW = po(-2, -1)
NNW = po(-2, -1)

-- essential
DISPENSIBLE   = 0
INDISPENSIBLE = 1
PERKIND       = 2

-- glasses
SPOT_NOTHING        =   0
SPOT_DEATH          =   1
SPOT_HOLLOW         =   2
SPOT_ACTORIMPULSE   =   4
SPOT_SENSOR         =   8
SPOT_LIGHTPASSENGER =  16
SPOT_TRAP           =  32

-- follower
FOLLOW_NO     = 0
FOLLOW_SCROLL = 1
FOLLOW_FLIP   = 2

FOLLOW_FULLSCREEN = po(19, 12)
FOLLOW_HALFSCREEN = po(9.5, 6)

-- Read directions for maps
MAP_IDENT = 0
MAP_ROT_CW = 1
MAP_ROT_180 = 2
MAP_ROT_CCW = 3
MAP_MIRROR_BACKSLASH = 4
MAP_MIRROR_HORIZONTAL = 5
MAP_MIRROR_SLASH = 6
MAP_MIRROR_VERTICAL = 7
MAP_COUNT = 7

---------------------
-- Utility Methods --
---------------------

wo:_register("drawMap", 
    function (world, resolver, anchor, ignorearg, maparg, readarg)
        -- TODO check validity of arguments
        -- world, resolver, (position|object|table), string, (table|map), [int]
        -- world, resolver, (position|object|table), map, [int]
        -- Analyse arguments 3 to 6
        local origin = anchor
        if type(origin) == "table" then
            origin = po(origin)
        end
        local ignore = ignorearg
        local map = maparg
        local readdir = readarg or MAP_IDENT
        if (ignorearg.type == "map") then
            map = ignorearg
            ignore = map.defaultkey
            readdir = maparg or MAP_IDENT
        elseif     (type(map.defaultkey) == "string")
               and (string.len(map.defaultkey) ~= string.len(ignore)) then
            error("drawmap: Ignore key and default key differ in length.", 2)
        end
        local len = string.len(ignore)
        if    (type(readdir) ~= "number") or (readdir % 1 ~= 0)
           or (readdir < MAP_IDENT) or (readdir > MAP_COUNT) then
            error("drawmap: Unknown read direction.", 2)
        end
        -- Prepare read direction rotation
        local w, h = 0, 0
        local function rot(x, y)
            -- The difference of this function to the one in libmap
            -- results among others from different coordinate origins
            -- and different application of rot.
            return ({[MAP_IDENT]             = {x,         y},
                     [MAP_ROT_CW]            = {h + 1 - y, x},
                     [MAP_ROT_180]           = {w + 1 - x, h + 1 - y},
                     [MAP_ROT_CCW]           = {y,         w + 1 - x},
                     [MAP_MIRROR_HORIZONTAL] = {w + 1 - x, y},
                     [MAP_MIRROR_VERTICAL]   = {x,         h + 1 - y},
                     [MAP_MIRROR_SLASH]      = {y,         x},
                     [MAP_MIRROR_BACKSLASH]  = {h + 1 - y, w + 1 - x} })[readdir]
        end
        if readdir ~= MAP_IDENT then
          -- Calculate height and width for rotation if neccessary
          h = #map
          for y = 1, h do
            w = math.max(w, string.len(map[y])/len)
          end
        end
        -- Draw map
        for y=1, #map do
            local linelen = string.len(map[y])
            if math.fmod(linelen, len) ~= 0 then
                error("drawmap map line ".. y .. " with odd length", 2)
            end
            for x = 1, linelen/len do
                local key = string.sub(map[y], len*(x-1)+1, len*x)
                if key ~= ignore then
                    local p = {origin.x - 1, origin.y - 1}
                    if readdir == MAP_IDENT then
                      p = {p[1] + x, p[2] + y}
                    else
                      p = {p[1] + (rot(x,y))[1], p[2] + (rot(x,y))[2]}
                    end
                    tile = world:_evaluate(resolver, key, p[1], p[2])
                    if tile then
                        world[p] = tile
                    else
                        error("drawmap: undefined tile '" .. key .. "' at "
                              .. p[1] .. ", " .. p[2] .. "(in submap at "
                              .. x .. ", ".. y .. ")")
                    end
                end
            end
        end
    end
)

wo:_register("drawBorder", 
    function (world, origin, arg2, arg3, arg4)
        local dest = arg2
        local tile = arg3
        if type(arg2) == "number" and type(arg3) == "number" then
            dest = po(origin.x + arg2 - 1, origin.y + arg3 - 1)
            tile = arg4
        end
        -- TODO check validity of arguments
        for x = origin.x, dest.x do
            wo[{x, origin.y}] = tile
            if origin.y ~= dest.y then
                wo[{x, dest.y}] = tile
            end
        end
        for y = origin.y + 1, dest.y -1 do
            wo[{origin.x, y}] = tile
            if origin.x ~= dest.x then
                wo[{dest.x, y}] = tile
            end
        end
    end
)

wo:_register("drawRect", 
    function (world, origin, arg2, arg3, arg4)
        local dest = arg2
        local tile = arg3
        if type(arg2) == "number" and type(arg3) == "number" then
            dest = po(origin.x + arg2 - 1, origin.y + arg3 - 1)
            tile = arg4
        end
        -- TODO check validity of arguments
        for x = origin.x, dest.x do
            for y = origin.y, dest.y do
                wo[{x, y}] = tile
            end
        end
    end
)

---------------
-- Libraries --
---------------

lib = {}
setmetatable(lib, {__index = 
    function (table, key)
        if type(key) == "string" then
            error("Library function named '"..key.."' not existing. A typo or a missing library dependency may cause this fault.", 2)
        else
            error("Library function access with a key of type '"..type(key).."'. A name is mandatory.", 2)
        end
    end 
})

---------------
-- Resolvers --
---------------

res = {}
setmetatable(res, {__index = 
    function (table, key)
        if type(key) == "string" then
            error("Resolver named '"..key.."' not existing. A typo or a missing library dependency may cause this fault.", 2)
        else
            error("Resolver access with a key of type '"..type(key).."'. A name is mandatory.", 2)
        end
    end 
})

function res.random_implementation(context, evaluator, key, x, y)
    for hit_itr, hit_pair in ipairs(context[4]) do
        if key == hit_pair[1] then
            local super = nil
            if hit_pair[2] ~= nil then
                super = evaluator(context[3], hit_pair[2], x, y)
            end
            local repl_key
            local r = math.random(context[6])
            local i = 1
            local s = context[5][i][2]
            while s < r do
                i = i + 1
                s = s + context[5][i][2]
            end
            repl_key = context[5][i][1]
            local replacement = evaluator(context[3], repl_key, x, y)
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
    return evaluator(context[3], key, x, y)
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
    local context = {res.random_implementation, nil, subresolver, hit_table, 
                      repl_table, repl_sum}
    return context
end

function res.autotile_newtile(key, template, substitution)
    local decl = template:_declaration()  -- get a deep copy
    local result
    for i, tile in ipairs(decl) do
        if type(tile) == "string" then
            if result == nil then
                result = ti[tile]
            else
                result = result .. ti[tile]
            end
        else
            local at = {}   -- attribute table
            for key, val in pairs(tile) do   -- a table
                if type(val) ~= "table" then
                    if type(val) == "string" then
                        at[key] = string.gsub(val, "%%%%", "%%"..substitution)
                    else
                        at[key] = val
                    end
                else
                    local tt = {} 
                    for j, token in ipairs(val) do
                        if type(token) == "string" then
                            tt[j] = string.gsub(token, "%%%%", "%%"..substitution)
                        else
                            tt[j] = token
                        end
                    end
                    at[key] = tt
                end
            end
            if result == nil then
                result = ti(at)
            else
                result = result .. ti(at)
            end
        end
    end
    ti[key] = result
end

function res.autotile_implementation(context, evaluator, key, x, y)
    for i, rule in ipairs (context[4]) do
        if #rule == 3 then   -- from, to substitution
            --
            local first = string.byte(rule[1], #rule[1])
            local last = string.byte(rule[2], #rule[2])
            local candidate = string.byte(key, #key)
            
            if #key == #rule[1] and string.sub(key, 1, -2) == string.sub(rule[1], 1, -2)
                    and first <= candidate and candidate <= last then
                if ti[key] == nil then
                    res.autotile_newtile(key, ti[rule[3]], candidate - first + 1)
                end
                return ti[key]
            end
        elseif string.find(key, rule[1], 1, true) == 1 then
            -- prefix based substitution
            if ti[key] == nil then
                res.autotile_newtile(key, ti[rule[2]], string.sub(key, #(rule[1]) + 1))
            end
            return ti[key]
        end
    end
    return evaluator(context[3], key, x, y)
end

function res.autotile(subresolver, ...)
    -- syntax: ... = <{prefixkey, template} | {fistkey, lastkey, template}>
    -- context: [4] = table with unmodified rule tables
    local args = {...}
    for i, rule in ipairs(args) do
        if type(rule) ~= "table" then
            error("Resolver autotile rule " .. i.." is not a table", 2)
        else
            if #rule < 2 or #rule > 3 then
                error("Resolver autotile rule "..i.." wrong number of arguments", 2)
            end
            for j, str in ipairs(rule) do
                if type(str) ~= "string" then
                    error("Resolver autotile rule "..i.." has not a string in position "..j, 2)
                end
            end
            if #rule == 3 then
                local first = string.byte(rule[1], #rule[1])
                local last  = string.byte(rule[2], #rule[2])
                if #rule[2] ~= #rule[1] or string.sub(rule[2], 1, -2) ~= string.sub(rule[1], 1, -2)
                        or first > last then
                    error("Resolver autotile rule "..i.." bad range start-end strings", 2)
                end
            end
            if ti[rule[#rule]] == nil then
                error("Resolver autotile missing template tile '"..rule[#rule].."'", 2)
            end
        end
    end
    local context = {res.autotile_implementation, nil, subresolver, args}
    return context
end

function res.composer_implementation(context, evaluator, key, x, y)
    local tile = evaluator(context[3], key, x, y)
    if tile ~= nil then
        return tile
    end
    -- try to compose tile
    for i = #key, 1, -1 do
        local subkey = string.rep(" ", i-1) .. string.sub(key, i, i) .. string.rep(" ", #key - i)
        local subtile = evaluator(context[3], subkey, x, y)
        if subtile == nil then
            return nil
        end
        if tile == nil then
            tile = subtile
        else
            tile = tile .. subtile
        end
        if #key == 3 then
            subkey = string.sub(key, 1, i-1) .. " " .. string.sub(key, i+1)
            local subtile2 = evaluator(context[3], subkey, x, y)
            if subtile2 ~= nil then
                return subtile2 .. subtile
            end
        end
    end
    return tile
end

function res.composer(subresolver)
    local context = {res.composer_implementation, nil, subresolver}
    return context
end

