#! /usr/bin/env lua50
general = {}
html = {}

--
--  Enigma Lua Homepage Gluer - main program
--  (c) 2007 Enigma Team
--  The Enigma Lua Homepage Gluer is licenced under GPL v.2 or above,
--  please find a copy of it here:
--    http://www.gnu.org/licenses/gpl-2.0.html
--  This program and some of its input files are loosely based on 
--  a predecessor by Daniel Heck.
--
--  The Enigma Lua Homepage Gluer incorporates the subprograms
--  "read_news.lua" and "read_lotm.lua" and calls "output-files.lua"
--  and "lotm_archive_data.lua" for input. All of them are under the
--  GPL v.2 as stated above.
--
--   Syntax:
--
--  Call without arguments. The main input file is
--    input/output-files.lua,
--  where all input files and the overall structure of the homepage
--  as well as the meaning of the $$...$$-macros are defined.
--  This main program only replaces the $$...$$-macros according to
--  the rules specified in output-files.lua.
--
--  Each file to be created has an own structure, holding the
--  strings infile (mostly schema.html) and outfile (name of result).
--  Infile is read and written to outfile.
--  If somewhere in infile occurs the pattern "$$mything$$" then
--  this pattern is replaced, dependend on the field mything of the
--  overall structure:
--    a string -> replace directly by this string
--    a function -> start this function
--    a table -> parse each entry as a new html-file
--

----------------------------------------------------------------------
-- Tools
----------------------------------------------------------------------

function mydebug(st)
  --io.write(st)
end

function add_lang_to_filename(filename, lang)
    if lang == "" then
        return filename
    else
        return string.gsub(filename, "%.", lang..".", 1)
    end
end

-- translate_month returns a translated string of the month number
-- MONTH in the year YEAR. If year is nil, no year is appended.
-- Data for translation is provided in output-files.lua.
function translate_month(lang, date_table)
  if type(date_table) ~= "table" then
    error("Trying to translate month, but argument is "..type(date_table).."!")
  end
  local month = date_table.month
  if month == nil then
    error("Trying to translate month, but month is NIL!")
  end
  local translation_table = date_translation_field["months"..lang]
  if translation_table == nil then
    translation_table = date_translation_field.months
  end
  month = translation_table[month]
  local year = date_table.year
  if year == nil then
    return month
  else
    -- circumvent mod->fmod change in 5.1!
    --year = math.fmod(year, 100)
    if year >= 2000 then
      year = year - 2000
    else
      year = year - 1900
    end
    year = string.format("%02d", year)
    return month.." '"..year
  end
end

-- scribble writes string s to output if and only if it wasn't
-- written there before. Hence, double infos are omitted.
scribbleblock = {}
function scribble(s)
  if type(s) ~= "string" then
    error("Can't scribble that, it's type is "..type(s).."!")
  end
  for k, t in pairs(scribbleblock) do
    if t == s then
      mydebug("  repeat: ["..s.."]")
      return
    end
  end
  io.write(s)
  table.insert(scribbleblock, s)
end

----------------------------------------------------------------------
-- Parsing and Macro-Replacing - The Program's Recursive Heartbeat
----------------------------------------------------------------------
 
function parse_text(v, text, lang, context)
    return string.gsub(text, "%$%$(.-)%$%$", function (s)
        local process = v[s] or general[s]
        local process_with_lang = v[s..lang] or general[s..lang]
        local stype = type(process)
        if stype == "nil" then
            -- Assume the entry is short for an html-output-file
            mydebug(" --> "..s.."\n")
            if type(html[s]) == "table" and type(html[s].outfile) == "string" then
                return add_lang_to_filename(html[s].outfile, lang)
            else
                error("Error during evaluation of "..context
                      ..": Entry "..s.." not defined.")
            end
        end
        if stype == "string" then
            if type(process_with_lang) == "string" then
                return parse_text(v, process_with_lang, lang, context)
            else
                scribble("  Missing entry "..s..lang.." for "
                         ..context..", using fallback.\n")
                return parse_text(v, process, lang, context)
            end
        end
        if stype == "function" then
            return process(v,s,lang)
        end
        if stype == "table" then
            local addstring = ""
            for j,f in pairs(process) do
                -- remove slash for name- and id-tags
                fn = string.gsub(f, "/", "_")
                if f ~= fn then
                    scribble("  Note: changed anchors for "..f.." to "..fn..
                             ". Please correct href's.\n")
                end
                addstring = addstring.."<a name=\""..fn.."\" id=\""..fn.."\"></a>\n"
                addstring = addstring..parse_html(v, directory..f..suffix, lang)
            end
            return addstring
        end
        error("Error during evaluation of "..context..", "..s.." (type "..stype..").")
    end)
end

function parse_html(v, infilename0, lang)
    if not infilename0 then
        error("Error during creation of "..v.title..": Expected filename, got nil.")
    end

    -- Open infile dependend relative to lang, use English as fallback.

    local infilename = add_lang_to_filename(infilename0, lang)
    local infile = io.open(infilename, "r")
    if infile == nil then
        infile = io.open(infilename0, "r")
        if infile == nil then
            error("Error: Neither "..infilename.." nor "..infilename0.." exist.")
        else        
            scribble ("No file "..infilename..", using fallback "..infilename0.."...\n")
            infilename = infilename0
        end
    else
        scribble ("Reading input from "..infilename.." ...\n")
    end

    -- Read and process infile

    local body = infile:read("*a")
    infile:close()
    return parse_text(v, body, lang, infilename)
end

----------------------------------------------------------------------
-- Main Routine
----------------------------------------------------------------------

-- Get general data and LotM archive data

dofile("input/output-files.lua")
dofile("input/lotm/lotm_archive_data.lua")

-- Add functions for parsing the news and LotM archive

dofile("input/news/read_news.lua")
dofile("input/lotm/read_lotm.lua")

-- Autogenerate LotM-entries into the html-field (see read_lotm.lua)

generate_lotm_entries(html)

-- Now glue everything together

for lang_nr, lang in pairs(language_list) do
  for k,v in pairs(html) do
    if type(v.outfile) ~= "string" then
        error("Error during main.lua: Outfile not properly defined.")
    end
    local outfilename = add_lang_to_filename(v.outfile, lang)
    local outfile = io.open (outfilename, "w")
    outfile:write(parse_html(v, v.infile or general.infile, lang).."\n")
    outfile:close()
  end
end

