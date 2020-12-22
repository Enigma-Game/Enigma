------------------------------------------------------------------------
-- Copyright (C) 2008,2009,2010,2011 Ronald Lamprecht
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

-----------------------------------------------
-- Semi strict Lua - global typo detection   --
-----------------------------------------------
-- Limit Lua code read access to prior 'declared' global variables,
-- that have been registered by a regular assignment (even assigning nil will
-- do). C code will read access globals without check.
-- This section is a slightly modified version of Lua's etc/strict.lua.
-- We call it semi strict, as we still allow level authors to declare new
-- globals inside a function. We just want to catch typos in global constants.
do
    local mt = getmetatable(_G)
    if mt == nil then
        mt = {}
        setmetatable(_G, mt)
    end

    mt.__declared = {}

    local function what ()
      local d = debug.getinfo(3, "S")
      return d and d.what or "C"
    end

    mt.__newindex = function (t, n, v)
        if not mt.__declared[n] then
            mt.__declared[n] = true
        end
        rawset(t, n, v)
    end

    mt.__index = function (t, n)
        if not mt.__declared[n]  and what() ~= "C"  then
            error("variable '"..n.."' is not declared", 2)
        end
        return rawget(t, n)
    end
end
-----------------------------
-- Helper functions        --
-----------------------------

function enigma.settile(key, pos)
    wo[pos] = en.ti[key]
end

---------------
-- Constants --
----------------

-- state
OFF       = 0
ON        = 1
CLOSED    = 0
OPEN      = 1
IDLE      = 0
ACTIVE    = 1
INACTIVE  = 2
OXYDPAIR  = 2
LIGHT     = 0
MEDIUM    = 1
DARK      = 2
FLOODING  = 1
INVISIBLE = -1
SMALL     = 0
LARGE     = 2
YIN       = 0
YANG      = 1
EMPTY     = 0
FULL      = 2
BROKEN    = 1
BREAKING  = 2
CW  = 0
CCW = 1

-- color
BLACK  = 0
WHITE  = 1
GLASS  = 2
BLUE   = 3
YELLOW = 4
NOCOLOR = nil

-- inkwell state
INKWELL_BLACK = 0
INKWELL_WHITE = 1
INKWELL_EMPTY = 2

-- controllers
CTRL_NONE    = 0
CTRL_YIN     = 1
CTRL_YANG    = 2
CTRL_YINYANG = 3

-- oxyd color
OXYD_BLUE   =  0
OXYD_RED    =  1
OXYD_GREEN  =  2
OXYD_YELLOW =  3
OXYD_CYAN   =  4
OXYD_PURPLE =  5
OXYD_WHITE  =  6
OXYD_BLACK  =  7
OXYD_GRAY   =  8
OXYD_ORANGE =  9
OXYD_PINE   = 10
OXYD_BROWN  = 11
OXYD_AUTO   = -1
OXYD_FAKE   = -2
OXYD_QUAKE  = -3
OXYD_BOLD   = -4

-- direction/orientations
RANDOMDIR = -2
NODIR     = -1
WEST      =  0
SOUTH     =  1
EAST      =  2
NORTH     =  3
NORTHWEST =  4
SOUTHWEST =  5
SOUTHEAST =  6
NORTHEAST =  7

-- alternative orientation notation for mirrors
BACKSLASH  = 0
HORIZONTAL = 1
SLASH      = 2
VERTICAL   = 3

-- direction offsets
N = po(0, -1)
E = po(1, 0)
S = po(0, 1)
W = po(-1, 0)
NE = N + E
NW = N + W
SE = S + E
SW = S + W
NNE = N + NE
ENE = E + NE
ESE = E + SE
SSE = S + SE
SSW = S + SW
WSW = W + SW
WNW = W + NW
NNW = N + NW

-- neighbors

NEIGHBORS_4     = W .. S .. E .. N
NEIGHBORS_8     = NEIGHBORS_4 .. NW .. SW ..SE .. NE
NEIGHBORS_CHESS = NNE .. ENE .. ESE .. SSE .. SSW .. WSW .. WNW .. NNW

-- convertion orientation numbers to direction vectors
ORI2DIR = {[WEST]=W, [SOUTH]=S, [EAST]=E, [NORTH]=N, [NODIR]=po(0,0),
           [NORTHWEST]=NW, [SOUTHWEST]=SW, [SOUTHEAST]=SE, [NORTHEAST]=NE}

-- essential
DISPENSABLE   = 0
INDISPENSABLE = 1
PERKIND       = 2

-- meditation
MEDITATION_CALDERA = -3
MEDITATION_HOLLOW  = -2
MEDITATION_DENT    = -1
MEDITATION_BUMP    =  1
MEDITATION_HILL    =  2
MEDITATION_VOLCANO =  3

-- glasses
SPOT_NOTHING        =   0
SPOT_DEATH          =   1
SPOT_HOLLOW         =   2
SPOT_ACTORIMPULSE   =   4
SPOT_SENSOR         =   8
SPOT_LIGHTPASSENGER =  16
SPOT_TRAP           =  32

-- coinslot
COIN_IGNORE = -1
COIN_REJECT = -2

-- rubberbands
AUTOLENGTH = -1

-- subsoil
SUBSOIL_ABYSS = 0
SUBSOIL_WATER = 1
SUBSOIL_AUTO  = 2

-- follower
FOLLOW_NO     = 0
FOLLOW_SCROLL = 1
FOLLOW_FLIP   = 2

FOLLOW_FULLSCREEN = po(19, 12)
FOLLOW_HALFSCREEN = po(9.5, 6)

-- map transformations, aka. read directions
MAP_IDENT            = {index = 1}
MAP_ROT_CW           = {index = 2}
MAP_ROT_180          = {index = 3}
MAP_ROT_CCW          = {index = 4}
MAP_FLIP_BACKSLASH   = {index = 5}
MAP_FLIP_HORIZONTAL  = {index = 6}
MAP_FLIP_SLASH       = {index = 7}
MAP_FLIP_VERTICAL    = {index = 8}
MAP_COUNT = 8
-- Note: The sequence is of importance for lib.map.transformation_compose.
MAP_ALL = {MAP_IDENT, MAP_ROT_CW, MAP_ROT_180, MAP_ROT_CCW, MAP_FLIP_BACKSLASH,
           MAP_FLIP_HORIZONTAL, MAP_FLIP_SLASH, MAP_FLIP_VERTICAL}

for _, transformation in ipairs(MAP_ALL) do
    setmetatable(transformation, {
        __mul = (function() error("You have to load libmap to use \"*\" on map transformations.", 2) end),
        __pow = (function() error("You have to load libmap to use \"^\" on map transformations.", 2) end),
        __unm = (function() error("You have to load libmap to use \"-\" on map transformations.", 2) end),
        _type = "maptransformation"})
end

---------------------
-- Utility Methods --
---------------------

wo:_register("drawMap",
    function (world, resolver, anchor, arg3, arg4, arg5)
        -- world, resolver, (position|object|table), string, (table|map), [int]
        -- world, resolver, (position|object|table), map, [int]
        assert_type(world, "wo:drawMap self (world)", 2, "world")
        assert_bool(is_resolver(resolver), "wo:drawMap first argument is no valid resolver", 2)
        assert_type(anchor, "wo:drawMap second argument (anchor)", 2, "position", "valid object", "table")
        assert_type(arg3, "wo:drawMap third argument (ignorekey or map)", 2, "string", "map")
        local origin = po(anchor)   -- either convert or make a working position copy
        -- Analyse arguments 4 to 6
        local ignore, map, readdir
        if (etype(arg3) == "map") then
            assert_type(arg4, "wo:drawMap fourth argument (read direction)", 2, "nil", "maptransformation")
            map = arg3
            ignore = map.defaultkey
            readdir = arg4 or MAP_IDENT
        else
            assert_type(arg4, "wo:drawMap fourth argument (map)", 2, "table", "map")
            assert_type(arg5, "wo:drawMap fifth argument (read direction)", 2, "nil", "maptransformation")
            ignore = arg3
            map = arg4
            readdir = arg5 or MAP_IDENT
            assert_bool((etype(map) ~= "map") or (string.len(map.defaultkey) == string.len(ignore)),
                "wo:drawMap: Ignore key and default key differ in length.", 2)
        end
        local len = string.len(ignore)
        -- Prepare read direction rotation
        local w, h = 0, 0
        local function rot(x, y)
            -- The difference of this function to the one in libmap
            -- results among others from different coordinate origins
            -- and different application of rot.
            return ({[MAP_IDENT]           = {x,         y},
                     [MAP_ROT_CW]          = {h + 1 - y, x},
                     [MAP_ROT_180]         = {w + 1 - x, h + 1 - y},
                     [MAP_ROT_CCW]         = {y,         w + 1 - x},
                     [MAP_FLIP_HORIZONTAL] = {w + 1 - x, y},
                     [MAP_FLIP_VERTICAL]   = {x,         h + 1 - y},
                     [MAP_FLIP_SLASH]      = {y,         x},
                     [MAP_FLIP_BACKSLASH]  = {h + 1 - y, w + 1 - x} })[readdir]
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
            local line = map[y]
            local linelen = string.len(line)
            assert_bool(math.fmod(linelen, len) == 0, "wo:drawMap: Map line ".. y .. " with odd length.", 2)
            for x = 1, linelen/len do
                local key = string.sub(line, len*(x-1)+1, len*x)
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
                        error("wo:drawMap: undefined tile '" .. key .. "' at "
                              .. p[1] .. ", " .. p[2] .. "(in submap at "
                              .. x .. ", ".. y .. ").")
                    end
                end
            end
        end
        -- Call resolver finalizers from bottom to top
        local function finalizeResolvers(context)
            if type(context) == "table" then
                finalizeResolvers(context[3])
                local finalizer = context[2]
                if type(finalizer) == "function" then
                     finalizer(context)
                end
            end
        end
        finalizeResolvers(resolver)
    end
)

wo:_register("drawBorder",
    function (world, arg1, arg2, arg3, arg4, arg5)
        -- world, (position|object|table), width, height, (tile|table|key, resolver)
        -- world, (position|object|table), (position|object|table), (tile|table|key, resolver)
        assert_type(world, "wo:drawBorder self (world)", 2, "world")
        assert_type(arg1, "wo:drawBorder first argument (upperleft edge)", 2, "position", "valid object", "table")
        assert_type(arg2, "wo:drawBorder second argument (width or lowerright edge)", 2, "position", "valid object", "table", "positive integer")
        local origin = po(arg1)
        local dest, tile, resolver
        local isTile = true
        if etype(arg2) == "number" then
            assert_type(arg3, "wo:drawBorder third argument (height)", 2, "positive integer")
            dest = po(origin.x + arg2 - 1, origin.y + arg3 - 1)
            tile = arg4
            resolver = arg5
        else
            dest = po(arg2)
            tile = arg3
            resolver = arg4
        end
        assert_type(tile, "wo:drawBorder tile/key argument error", 2, "tile", "table", "string")
        if etype(tile) == "string" then
            assert_bool(is_resolver(resolver), "wo:drawBorder resolver argument error", 2)
            isTile = false
        end
        for x = origin.x, dest.x do
            wo[{x, origin.y}] = tile
            if origin.y ~= dest.y then
                if isTile then
                    wo[{x, dest.y}] = tile
                else
                    wo[{x, dest.y}] = world:_evaluate(resolver, tile, x, dest.y)
                end
            end
        end
        for y = origin.y + 1, dest.y -1 do
            wo[{origin.x, y}] = tile
            if origin.x ~= dest.x then
                if isTile then
                    wo[{dest.x, y}] = tile
                else
                    wo[{dest.x, y}] = world:_evaluate(resolver, tile, dest.x, y)
                end
            end
        end
    end
)

wo:_register("drawRect",
    function (world, arg1, arg2, arg3, arg4, arg5)
        -- world, (position|object|table), width, height, (tile|table|key, resolver)
        -- world, (position|object|table), (position|object|table), (tile|table|key, resolver)
        assert_type(world, "wo:drawRect self (world)", 2, "world")
        assert_type(arg1, "wo:drawRect first argument (upperleft edge)", 2, "position", "valid object", "table")
        assert_type(arg2, "wo:drawRect second argument (width or lowerright edge)", 2, "position", "valid object", "table", "positive integer")
        local origin = po(arg1)
        local dest, tile, resolver
        local isTile = true
        if etype(arg2) == "number" then
            assert_type(arg3, "wo:drawRect third argument (height)", 2, "positive integer")
            dest = po(origin.x + arg2 - 1, origin.y + arg3 - 1)
            tile = arg4
            resolver = arg5
        else
            dest = po(arg2)
            tile = arg3
            resolver = arg4
        end
        assert_type(tile, "wo:drawRect tile/key argument error", 2, "tile", "table", "string")
        if etype(tile) == "string" then
            assert_bool(is_resolver(resolver), "wo:drawRect resolver argument error", 2)
            isTile = false
        end

        for x = origin.x, dest.x do
            for y = origin.y, dest.y do
                if isTile then
                    wo[{x, y}] = tile
                else
                    wo[{x, y}] = world:_evaluate(resolver, tile, x, y)
                end
            end
        end
    end
)

------------------------------
-- Global utility functions --
------------------------------

-- A wrapper of "if" to resemble the ternary ?:-function.
-- Note that this function evaluates both IFTRUE as well as IFFALSE, e.g.
--     cond(t == 0, 1/t, error("Division by zero"))
-- will evaluate the error-function and thus halt for any T.
-- Hence: Make sure there are no sideeffects in IFTRUE and IFFALSE!
function cond(condition, iftrue, iffalse)
    if condition then
        return iftrue
    else
        return iffalse
    end
end

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
res.metatable = {_type="resolver", _resolver=true}

function res.random_implementation(context, evaluator, key, x, y)
    for hit_itr, hit_pair in ipairs(context[4]) do
        if key == hit_pair[1] then
            local super = nil
            if hit_pair[2] ~= nil then
                super = evaluator(context[3], hit_pair[2], x, y)
            end
            local repl_key
            local r = math.random() * context[6]
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
    assert_bool(is_resolver(subresolver), "res.random first argument (subresolver)", 2)
    assert_type(hits, "res.random second argument (hits)", 2, "non-empty string", "table")
    assert_type(replacements, "res.random third argument (replacements)", 2, "string", "table")
    local hit_table = {}
    if type(hits) == "string" then
        hit_table[1] = {hits, nil}
    else  -- table
        for i, v in ipairs(hits) do
            assert_type(v, "res.random second argument (hits), entry " .. i, 2, "non-empty string", "table")
            if type(v) == "string" then
                hit_table[i] = {v, nil}
            elseif type(v) == "table" then
                assert_type(v[1], "res.random second argument (hits), key in entry " .. i, 2, "non-empty string")
                assert_type(v[2], "res.random second argument (hits), superkey in entry " .. i, 2, "nil", "string")
                hit_table[i] = v
            end
        end
    end
    local repl_table = {}
    if type(replacements) == "string" then
        repl_table[1] = {replacements, 1}
    else  -- table
        for i, v in ipairs(replacements) do
            assert_type(v, "res.random third argument (replacements), entry " .. i, 2, "string", "table")
            if type(v) == "string" then
                repl_table[i] = {v, 1}
            else  -- table of key and frequency
                assert_type(v[1], "res.random third argument (replacements), key in entry " .. i, 2, "string")
                assert_type(v[2], "res.random third argument (replacements), frequency in entry " .. i, 2, "non-negative")
                repl_table[i] = v
            end
        end
    end
    local repl_sum = 0
    for i, v in ipairs(repl_table) do
        repl_sum = repl_sum + v[2]
    end
    local context = {res.random_implementation, nil, subresolver, hit_table,
                     repl_table, repl_sum}
    setmetatable(context, res.metatable)
    return context
end

function res.autotile_newtile(context, evaluator, key, x, y, template, substitution)
    local decl = template:_declaration()  -- get a deep copy
    local result
    for i, tile in ipairs(decl) do
        if type(tile) == "string" then
            local subkey = string.gsub(tile, "%%%%", substitution)
            if result == nil then
                result = evaluator(context[3], subkey, x, y)
            else
                result = result .. evaluator(context[3], subkey, x, y)
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
        if #rule >= 3 then   -- from, to substitution
            --
            local first = string.byte(rule[1], #rule[1])
            local last = string.byte(rule[2], #rule[2])
            local candidate = string.byte(key, #key)
            local offset = 1
            if #rule == 4 then
                offset = rule[4]
            end

            if #rule[1] == #key and string.sub(key, 1, -2) == string.sub(rule[1], 1, -2)
                    and first <= candidate and candidate <= last then
                local tile = evaluator(context[3], key, x, y)
                if tile == nil then
                    res.autotile_newtile(context, evaluator, key, x, y, ti[rule[3]], candidate - first + offset)
                    return ti[key]
                else
                    return tile
                end
            end
        elseif string.find(key, rule[1], 1, true) == 1 then
            -- prefix based substitution
            local tile = evaluator(context[3], key, x, y)
            if tile == nil then
                res.autotile_newtile(context, evaluator, key, x, y, ti[rule[2]], string.sub(key, #(rule[1]) + 1))
                return ti[key]
            else
                return tile
            end
        end
    end
    return evaluator(context[3], key, x, y)
end

function res.autotile(subresolver, ...)
    -- syntax: ... = <{prefixkey, template} | {firstkey, lastkey, template[, offset]}>
    -- context: [4] = table with unmodified rule tables
    assert_bool(is_resolver(subresolver), "res.autotile first argument (subresolver)", 2)
    local args = {...}
    for i, rule in ipairs(args) do
        assert_type(rule, "res.autotile argument " .. (i+1) .. " (rule " .. i .. ")", 2, "table")
        assert_bool((#rule >= 2) and (#rule <= 4), "res.autotile rule "..i.." has wrong number of arguments (must be 2, 3, or 4).", 2)
        local template_pos = 2
        local string_pos = {1,2}
        if #rule >= 3  then
            template_pos = 3
            string_pos = {1,2,3}
        end

        for j, num in ipairs(string_pos) do
            assert_type(rule[string_pos[num]], "res.autotile rule " .. i .. ", position " .. num, 2, "string")
        end
        if #rule == 4 then
            assert_type(rule[4], "res.autotile rule " .. i .. ", position 4 (offset)", 2, "integer")
        end
        if #rule >= 3 then
            local first = string.byte(rule[1], #rule[1])
            local last  = string.byte(rule[2], #rule[2])
            assert_bool(#rule[1] == #rule[2] and string.sub(rule[2], 1, -2) == string.sub(rule[1], 1, -2) and first <= last,
                "res.autotile: Rule " .. i .. " has bad range start-end strings.", 2)
        end
        assert_bool(ti[rule[template_pos]], "res.autotile: Missing template tile '" .. rule[template_pos] .. "'", 2)
    end
    local context = {res.autotile_implementation, nil, subresolver, args}
    setmetatable(context, res.metatable)
    return context
end

function res.composer_implementation(context, evaluator, key, x, y)
    local tile = evaluator(context[3], key, x, y)
    if tile ~= nil then
        return tile
    end
    local sequence = context[4]
    if sequence == nil then
        sequence = "123456789"
    end
    -- try to compose tile
    for i = 1, #key do
        local subkey = ""
        local ic = "" .. i
        for j = 1, #key do
            if sequence:sub(j, j) == ic then
                subkey = subkey .. key:sub(j, j)
            else
                subkey = subkey .. " "
            end
        end
        if subkey ~= string.rep(" ", #key) then
            local subtile = evaluator(context[3], subkey, x, y)
            if subtile == nil then
                return nil
            end
            if tile == nil then
                tile = subtile
            else
                tile = tile .. subtile
            end
        end
    end
    return tile
end

function res.composer(subresolver, ...)
    assert_bool(is_resolver(subresolver), "res.composer first argument (subresolver)", 2)
    local args = {...}
    local sequence = nil
    assert_bool(#args <= 1, "res.composer: Too many arguments (must be 1 or 2)." ,2)
    if #args == 1 then
        assert_type(args[1], "res.composer second argument (sequence)", 2, "non-empty string")
        sequence = args[1]
    end
    local context = {res.composer_implementation, nil, subresolver, sequence}
    setmetatable(context, res.metatable)
    return context
end

--------------------
-- Error handling --
--------------------

function etype(object)
    local t = type(object)
    if t == "userdata" then
        return usertype(object)
    elseif t == "table" then
        return (getmetatable(object) or {})._type or t
    end
    return t
end

function is_resolver(resolver)
    local t = etype(resolver)
    return (t == "tiles") or (t == "function") or (t == "resolver") or
        ((type(resolver) == "table") and getmetatable(resolver) and getmetatable(resolver)["_resolver"])
end

assert_type = function (object, objname, level, ...)
    -- assert_type throws an error if object does not fulfill one of the
    -- conditions in "...". Possible conditions are the lua- and usertype
    -- names and "map" (i.e. the results of etype), plus the strings
    -- "integer", "natural" (i.e. integer >= 0), "positive" (> 0), and
    -- "non-negative" (>= 0).
    local conditions = {...}
    local fulfilled = false
    for k = 1, #conditions do
        local condition = conditions[k]
        fulfilled = fulfilled
            or  (condition == etype(object))
            or ((condition == "integer") and (type(object) == "number") and (math.ceil(object) == object))
            or ((condition == "positive") and (type(object) == "number") and (object > 0))
            or ((condition == "non-negative") and (type(object) == "number") and (object >= 0))
            or ((condition == "natural") and (type(object) == "number") and (math.ceil(object) == object) and (object >= 0))
            or ((condition == "positive integer") and (type(object) == "number") and (math.ceil(object) == object) and (object > 0))
            or ((condition == "non-empty string") and (type(object) == "string") and (object ~= ""))
            or ((condition == "any table") and (type(object) == "table"))
            or ((condition == "valid object") and (etype(object) == "object") and (-object))
    end
    if not fulfilled then
        for k = 1, #conditions do
            if type(conditions[k]) ~= "string" then
                error("Error in type assertion: Wrong argument for assert_type, must be string.", 2)
            end
        end
        if type(objname) ~= "string" then
            error("Error in type assertion: Object name missing for assert_type.", 2)
        end
        local s = ""
        if objname ~= "" then
            s = " for " .. objname
        end
        if #conditions == 0 then
            error("Unconditional assertion" .. s .. ".", (level or 1) + 1)
        end
        s = "Wrong type" .. s ..", is " .. etype(object) ..", must be "
        if #conditions == 1 then
            s = s .. conditions[1] .. "."
        else
            s = s .. "one of "
            for k = 1, #conditions - 1 do
                s = s .. conditions[k] .. ", "
            end
            s = s .. conditions[#conditions] .. "."
        end
        error(s, (level or 1) + 1)
    end
end

assert_bool = function (bool, message, level)
    -- assert_bool throws an error with message MESSAGE if BOOL is false or nil.
    if not bool then
        if message and (type(message) == "string") and (message ~= "") then
            error(message, (level or 1) + 1)
        else
            error("Anonymous assertion failed.", (level or 1) + 1)
        end
    end
end

