
lua = {
    floor = floor
}

lua2xml = {
    datadir = "../data",
    errormsg = "",
    errors = {},

    w = 0,
    h = 0,
    fl = {},
    it = {},
    st = {},
    actors = {},

    namedobjs = {},
    oxyds = {},
    rubberbands = {},
    signals = {},
    objcnt = 0,

    luacode = "",

    -- options
    oxyd_shuffle = FALSE
}

function lua2xml:add_error (e)
    self.errors[e] = (self.errors[e] or 0) + 1
    self.errormsg = self.errormsg .. e .. "\n"
end

function lua2xml:has_error ()
    return self.errormsg ~= ""
end

function lua2xml:resize_world (w, h)
    self.w, self.h = w, h
    for x=0,(w-1) do
        self.fl[x] = {}
        self.it[x] = {}
        self.st[x] = {}
    end
end

function lua2xml:set_table  (table, x, y, o)
    x, y = lua.floor (tonumber (x)), lua.floor (tonumber (y))
    if x >= 0 and y >= 0 and x<self.w and y<self.h then
        table[x][y] = o
    end
end


function lua2xml:optimize_floor ()
    local floorcnt = {}
    for y = 0,(self.h-1) do
        for x = 0,(self.w-1) do
            local fl = self.fl[x][y]
            if fl == nil then
                fl = {name="fl-abyss"} -- replace missing floor tiles with fl-abyss
                self.fl[x][y] = fl
            end
            floorcnt[fl.name] = (floorcnt[fl.name] or 0) + 1
        end
    end

    local floorlist = {}
    for k,v in floorcnt do tinsert (floorlist, k) end
    sort (floorlist, function (x, y) return (%floorcnt[x] < %floorcnt[y]) end)

    -- Fill floor with most common floor tile
    self.fillfloor = floorlist[getn(floorlist)]

    -- Remove these floor tiles from the array
    for y = 0,(self.h-1) do
        for x = 0,(self.w-1) do
            local fl = self.fl[x][y].name
            if fl == self.fillfloor then
                self.fl[x][y] = nil
            end
        end
    end
end



------------------------------
-- Output level as XML file --
------------------------------

function xmlout_info (s)
    print ("\t<info>")
    print ("\t\t<name></name>")
    print ("\t\t<author></author>")
    print ("\t\t<copyright></copyright>")
    print ("\t\t<license></license>")
    print ("\t</info>")
end

function xmlout_options (s)
    function yesno(x) return x==TRUE and "YES" or "NO" end
    function noyes(x) return x==TRUE and "NO" or "YES" end
    function printopt (name, val) 
        if val then
            print ('\t<option name="'..name..'" value="'..val..'" />') 
        end
    end
    
    printopt ("reset", noyes(enigma.ConserveLevel))
    printopt ("oxydflavor", oxyd_default_flavor)
    printopt ("shuffle", yesno (lua2xml.oxyd_shuffle))
    printopt ("scrolling", lua2xml.scrollmode)
    printopt ("brittleness", enigma.Brittleness)
    printopt ("slopeforce", enigma.SlopeForce)
    printopt ("flatforce", enigma.FlatForce)
    printopt ("frictionfactor", enigma.FrictionFactor)
    printopt ("electricforce", enigma.ElectricForce)
    printopt ("bumperforce", enigma.BumperForce)
    printopt ("magnetforce", enigma.MagnetForce)
    printopt ("magnetrange", enigma.MagnetRange)
    printopt ("wormholeforce", enigma.WormholeForce)
    printopt ("wormholerange", enigma.WormholeRange)
    printopt ("holeforce", enigma.HoleForce)
end

-- Convert a table of attributes into an XML tag option.
function string_attribs (a)
    local s = ""
    if a then
        for k,v in a do
            s = s .. k .. "='" .. v .. "',"
        end
    end
    if s ~= "" then
        return ' attribs="' .. s  .. '"'
    end
    return ""
end


function xmlout_actors (s)
    print "\t<actors>"

    for i=1,getn(s.actors) do
        local ac = s.actors[i]
        local o = ac.obj
        local a =string_attribs(o.attribs)
        print (format ('\t\t<actor kind="%s" x="%f" y="%f" %s />', o.name, ac.x, ac.y, a))
    end

    print "\t</actors>"
end

function xmlout_grid (s, tag, table)
    function optimize_row (r)
        -- remove trailing " -"'s and " *"'s
        r = gsub (r, "[%- ]+$", "")

        -- compress repeated " #"'s
        r = gsub (r, "(#[ #]+ )", function (v) 
                                     local n = (strlen(v))/2
                                     return n>1 and (n .. " ") or v
                                 end)
        r = gsub (r, "(%-[ %-]+ )", function (v) 
                                     local n = (strlen(v))/2
                                     return n>1 and ("-" .. n .. " ") or v
                                 end)
        return r
    end

    local attribobjs = ""
    for y=0,(s.h-1) do
        local row = ""
        local prevname = ""
        local numempty = 0
        for x=0,(s.w-1) do
            local o = table[x][y]
            if not o then
                row = row .. '- '
                numempty = numempty + 1
            else
                local a = string_attribs (o.attribs)
                if a ~= "" then
                    attribobjs = attribobjs .. '\t\t<' .. tag .. ' x="' .. x .. '" y="' .. y 
                        .. '" kind="' .. o.name .. '"' .. a .. ' />\n'
                    row = row .. '- '
                else
                    if o.name == prevname then
                        row = row .. '# '
                    else
                        row = row .. strsub(o.name, 4) .. " "
                        prevname = o.name
                    end
                end
            end
        end
        row = optimize_row (row)
        if numempty == s.w then
--             print ('\t\t<row y="' .. y .. '"/>')
        elseif row ~= "" then 
            print ('\t\t<row y="' .. y .. '">' .. row  .. "</row>") 
        end
    end
    if attribobjs ~= "" then print (attribobjs) end
end

function xmlout_floors (s)
    print "\t<floors>"
    if s.fillfloor then
        print ('\t\t<fill kind="' .. s.fillfloor .. '" />')
    end
    xmlout_grid (s, "floor", s.fl)
    print "\t</floors>"
end

function xmlout_stones (s)
    print "\t<stones>"
    xmlout_grid (s, "stone", s.st)
    for i=1,getn(s.oxyds) do
        print ('\t\t<oxyd x="' .. s.oxyds[i].x .. '" y="' .. s.oxyds[i].y .. '" />')
    end
    print "\t</stones>"
end

function xmlout_items (s)
    print "\t<items>"
    xmlout_grid (s, "item", s.it)
    print "\t</items>"
end

function xmlout_signals (s)
    if getn (s.signals) == 0 then return end
    print "\t<signals>"
    print "\t</signals>"
end

function xmlout_rubberbands (s)
    if (getn (s.rubberbands) == 0) then return end
    print "\t<rubberbands>"
    for i=1,getn (s.rubberbands) do
        local rb = s.rubberbands[i]
        print (format ('\t\t<rubberband from="%s" to="%s" strength="%f" length="%f" minlen="%f" />', 
                        rb.o1, rb.o2, rb.force, rb.len, rb.minlen))
    end
    print "\t</rubberbands>"
end

function xmlout_lua (s)
    if (s.luacode ~= "") then
        print ("\t<lua>")
        print (s.luacode)
        print ("\t</lua>")
    end
end


function xmlout_level (s)
    print ("<level width=\"" .. s.w.. "\" height=\"" .. s.h.. "\">")
    xmlout_info (s)
    xmlout_options (s)
    xmlout_floors (s)
    xmlout_items (s)
    xmlout_stones (s)
    xmlout_actors (s)
    xmlout_rubberbands (s)
    xmlout_signals (s)
    xmlout_lua (s)
    print ("</level>")
end


-----------------------------------------------
-- Simulate the interface to the game engine --
-----------------------------------------------

options = { }
enigma = { 
    WEST  = 0,
    SOUTH = 1,
    EAST  = 2,
    NORTH = 3,
    ConserveLevel = 1,
}
world = { }
display = { 
    FOLLOW_SCROLLING = 1,
    FOLLOW_SCREEN = 2,
    FOLLOW_SCREENSCROLLING = 3,
    FOLLOW_SMOOTH = 4
}

options.Difficulty = 2

function enigma.GetAttrib (o, name)
    return o.attribs[name]
end

function enigma.SetAttrib (o, key, val)
    function escapestr (str)
        str = gsub (str, "'", "\\'")
        return str
    end

    if key == "action" and val == "callback" then
        lua2xml:add_error "- uses callbacks"
    elseif o.name == "fl-gradient" and key == "type" then
        val = tonumber (val)
        if val >= 1 and val <= 12 then
            o.name = "fl-gradient" .. val
            return
        else
            local t = {[22] = 13, [21] = 14, [24] = 15, [23] = 16}
            o.name = "fl-gradient" .. t[val]
            return
        end
    elseif o.name == "st-puzzle" and key == "connections" then
        local t = {
            "hollow", "w", "s", "sw", "e", "ew", "es", "esw", "n", 
            "nw", "ns", "nsw", "ne", "new", "nes", "nesw",
        }
        o.name = "st-puzzle-" .. t[val]
        return
    elseif o.name == "st-bolder" and key == "direction" then
        local t = { "w", "s", "e", "n" }
        o.name = "st-bolder-" .. t[val+1]
        return
    elseif o.name == "st-oneway" and key == "orientation" then
        local t = { "w", "s", "e", "n" }
        o.name = "st-oneway-" .. t[val+1]
        return
    elseif (o.name == "st-door_a" or o.name == "st-door_b") and key == "type" then
        return
    end
    o.attribs[key] = escapestr (val)
end

function enigma.GetKind (o)
    return o.name
end

function enigma.NameObject (o, name)
    o.attribs["name"] = name
    lua2xml.namedobjs[name] = o
end

function enigma.GetNamedObject (name)
    return lua2xml.namedobjs[name]
end

function world.Resize (w, h) 
    lua2xml:resize_world (w, h)
end

function enigma.SendMessage (o, msg, val)
    lua2xml:add_error "- uses enigma.SendMessage"
end

function enigma.GetTicks ()
    lua2xml:add_error "- uses enigma.GetTicks"
    return 0
end

function enigma.SetFloor (x, y, o) lua2xml:set_table (lua2xml.fl, x, y, o) end
function enigma.SetItem (x,y, o)   lua2xml:set_table (lua2xml.it, x, y, o) end
function enigma.SetStone (x,y, o)  lua2xml:set_table (lua2xml.st, x, y, o) end
function enigma.SetActor (x, y, o) tinsert (lua2xml.actors, {obj=o, x=x, y=y}) end

function enigma.KillFloor (x, y)   lua2xml:set_table (lua2xml.fl, x, y, nil) end
function enigma.KillItem (x, y)    lua2xml:set_table (lua2xml.it, x, y, nil) end
function enigma.KillStone (x, y)   lua2xml:set_table (lua2xml.st, x, y, nil) end

function enigma.GetItem (x, y)     return lua2xml.it[x][y] end

function enigma.MakeObject (name)
    if name == "fl-ice_001" then
        name = "fl-ice"
    end
    return {name=name, attribs = {}}
end

function enigma.AddRubberBand (o1, o2, force, len, minlen)
    function add_missing_name (o)
        if o.attribs.name == nil then
            enigma.NameObject (o, "namedobj" .. lua2xml.objcnt)
            lua2xml.objcnt = lua2xml.objcnt + 1
        end
    end
    add_missing_name (o1); add_missing_name (o2)
    tinsert (lua2xml.rubberbands, 
             {o1 = o1.attribs.name, o2=o2.attribs.name, 
                 force = force, len = len, minlen = minlen or 0})
end

function display.SetFollowMode (m)
    lua2xml.scrollmode = m
end

function enigma.FindDataFile (f)
    return lua2xml.datadir .. "/" .. f
end

function enigma.AddConstantForce (x, y)
    lua2xml.luacode = lua2xml.luacode .. "enigma.AddConstantForce (" .. x .. ", " .. y .. ")\n"
--    lua2xml:add_error "- uses enigma.AddConstantForce"
end

function enigma.SetCompatibility (name)
    lua2xml.luacode = lua2xml.luacode .. "enigma.SetCompatibility (\"" .. name .. "\")\n"
--    lua2xml:add_error "- uses enigma.SetCompatibility"
end

function enigma.date ()
    lua2xml:add_error "- uses enigma.date"
    return "0"
end

function enigma.GetObjectTemplate ()
    lua2xml:add_error "- uses enigma.GetObjectTemplate"
end

function enigma.AddScramble ()
    lua2xml:add_error "- uses enigma.AddScramble"
end

function enigma.SetScrambleIntensity ()
    lua2xml:add_error "- uses enigma.SetScrambleIntensity"
end

function world.DefineSimpleStoneMovable ()
    lua2xml:add_error "- uses world.DefineSimpleStoneMovable"
end

function world.DefineSimpleStone ()
    lua2xml:add_error "- uses world.DefineSimpleStone"
end

function display.DefineAlias ()
    lua2xml:add_error "- uses display.DefineAlias"
end

function enigma.IsSolved ()
    lua2xml:add_error "- uses display.IsSolved"
end


function run_level_script (fname)
    local g = {}
    for k,v in globals() do g[k] = v end
    local oldglobals = globals(g)

    -- Include standard init.lua --
    dofile (enigma.FindDataFile ("init.lua"))

    -- Override definitions from init.lua --
    function g.oxyd (x, y)
        tinsert (lua2xml.oxyds, {x=x, y=y})
    end
    
    function g.oxyd_shuffle ()
        lua2xml.oxyd_shuffle = TRUE
    end
    
    -- Run level script --
    retval = 0
    if not dofile (luafile) then
        retval = 1
    end

    globals (oldglobals)
    return 0
end



------------------
-- Main program --
------------------

if getn (arg) ~= 1 then
    print ("Usage: " .. arg[0] .. " filename.lua")
    return 1
end

luafile = arg[1]

run_level_script (luafile)

--------------------------------
-- Optimize floor description --
--------------------------------

lua2xml:optimize_floor ()


if not lua2xml:has_error() then
    xmlout_level (lua2xml)
    exit (0)
else
    write (_STDERR,  "There were errors converting '" .. luafile .. "': \n")
    for k,v in lua2xml.errors do
--         write (_STDOUT, "<!-- " .. k .. ' (' .. v .. ' times) -->\n')
        write (_STDERR, k .. ' (' .. v .. ' times) \n')
    end
    write (_STDERR, '\n')
--    xmlout_level (lua2xml)
    exit(1)
end

