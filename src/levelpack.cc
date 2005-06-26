/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "lua.hh"
#include "world.hh"
#include "levels.hh"
#include "main.hh"

#include "ecl_util.hh"

#include "zipios++/zipfile.h"

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cassert>

using namespace enigma;
using namespace levels;
using namespace std;
using namespace ecl;
using zipios::ZipFile;

namespace
{

    /*! A traditional Enigma level pack. */
    class LevelPack_Enigma : public LevelPack {
    public:
        LevelPack_Enigma (const string &initfile, const string &name);

        // LevelPack interface
        void reinit();
        string get_name() const { return m_name; }
        size_t size() const { return m_levels.size(); }
        void load_level (size_t index);

        const LevelInfo &get_info (size_t index) const;

        int    get_revision_number(size_t index) const;
        int    get_default_SoundSet() const { return 1; }
        bool   needs_twoplayers() const { return false; }
        bool   may_have_previews() const { return true; }

    protected:
        LevelPack_Enigma() {}
        void set_name(const string &name) { m_name = name; }

        void clear_index () { 
            m_levels.clear(); 
        }
        void append_level_info (const LevelInfo &info) {
            m_levels.push_back (info);
        }

        // Load list of levels from stream
        void load_index (istream &is);

        void load_level (istream &is);
        void load_level_xml (istream &is);

    private:
        // Variables
        string            m_initfile;
        string            m_name;
        vector<LevelInfo> m_levels;
    };

    class LevelPack_CommandLine : public LevelPack_Enigma {
    public:
        LevelPack_CommandLine (const vector<string> &levelfiles,
                               const string &name);
    private:
        void load_level (size_t index);
    };


    /*! An Enigma level pack inside a ZIP file. */
    class LevelPack_Zipped : public LevelPack_Enigma {
    public:
        LevelPack_Zipped (const string &zipfile);
        ~LevelPack_Zipped();

        // LevelPack interface
        void        reinit();
        string      get_name() const { return m_name; }
        void        load_level(size_t idx);

        // never DECREASE the revision number!
        int         get_revision_number(size_t /*idx*/) const { return 1; }

    private:
        // Variables
        string            m_filename; // Name of .zip file
        auto_ptr<ZipFile> m_zip;
        string            m_name; // Name of level pack
    };
}


namespace 
{
    void parsePar(const string& par, int& par_value, string& par_text) 
    {
        // 'par' is in format "value,text"
        // The value is stored in 'par_value' and the text in 'par_text'.

        size_t comma = par.find(',');
        Assert<XLevelPackInit> (comma!=string::npos, "Comma expected in par");

        string value = par.substr(0, comma);
        par_text     = par.substr(comma+1);
        par_value    = atoi(value.c_str());
    }

    bool parse_levelinfo (const std::string &line, LevelInfo &info)
    {
        const char *wspace = " \t";
        size_t      p      = line.find_first_not_of(wspace);

        if (p == string::npos || line.at(p) != '{')
            return false;

        // found a level description

        int    par_time        = -1;
        string par_time_by;
        string par_moves_by;

        ++p;
        while (1) {
            string tag;
            string content;

            // ugly parser code starts here.
            // - it parses 'tag = content' or 'tag = "content"'
            // - '\"' is allowed in '"content"'
            // - whitespace is ignored
            // - sets message and breaks while loop in case of error
            size_t tag_start = line.find_first_not_of(wspace, p);
            Assert <XLevelPackInit> (tag_start != string::npos, "Expected tag or '}'");
            if (line.at(tag_start) == '}') break; // line done

            size_t equal = line.find('=', tag_start);
            Assert <XLevelPackInit> (equal != string::npos, "'=' expected");
            Assert <XLevelPackInit> (equal != tag_start, "empty tag");

            size_t tag_end = line.find_last_not_of(wspace, equal-1);
            tag = line.substr(tag_start, tag_end-tag_start+1);

            size_t content_start = line.find_first_not_of(wspace, equal+1);
            if (line.at(content_start) == '\"') { // content in ""
                size_t oquote       = line.find('\"', content_start+1);
                bool   have_escapes = false;
                while (oquote != string::npos && line.at(oquote-1) == '\\') { // step over \"
                    oquote       = line.find('\"', oquote+1);
                    have_escapes = true;
                }
                Assert <XLevelPackInit> (oquote != string::npos, "unmatched quote");
                content = line.substr(content_start+1, oquote-content_start-1);
                if (have_escapes) {
                    size_t esc;
                    while ((esc = content.find("\\\"")) != string::npos)
                        content.replace(esc, 2, "\"");
                }
                p = oquote+1;
            }
            else { // content w/o ""
                size_t content_end = line.find_first_of(" \t}", content_start);
                Assert <XLevelPackInit> (content_end != string::npos, "expected space or } behind content");
                content = line.substr(content_start, content_end-content_start);
                p = content_end;
            }

            if      (tag == "file")     info.filename     = content;
            else if (tag == "indexname")info.indexname    = content;
            else if (tag == "name")     info.name         = content;
            else if (tag == "author")   info.author       = content;
            else if (tag == "revision") info.revision     = atoi(content.c_str());
            else if (tag == "easymode") info.has_easymode = (content == "1");
            else if (tag == "par_time") parsePar(content, par_time, par_time_by);
            else if (tag == "par_time_easy") parsePar(content, info.par_time_easy, info.par_time_easy_by);
            else if (tag == "par_time_normal") parsePar(content, info.par_time_normal, info.par_time_normal_by);
            else if (tag == "par_moves") parsePar(content, info.par_moves, par_moves_by);
//                 else if (tag == "hint1") hint1           = content;
//                 else if (tag == "hint2") hint2           = content;
            else
                throw XLevelPackInit(strf("unknown tag '%s'", tag.c_str()));
        }

        Assert <XLevelPackInit> (info.filename.length() != 0, 
                                 "mandatory tag 'file' missing");

        if (info.has_easymode) {
            Assert <XLevelPackInit> (par_time == -1,
                                     "'par_time' not allowed when easymode=1 "
                                     "(use 'par_time_easy' and 'par_time_normal')");
        }
        else {
            Assert <XLevelPackInit> (info.par_time_normal==-1 && info.par_time_easy==-1,
                                     "'par_time_easy' and 'par_time_normal' are not allowed when easymode=0 (use 'par_time')");
            info.par_time_easy    = info.par_time_normal    = par_time;
            info.par_time_easy_by = info.par_time_normal_by = par_time_by;
        }
        return true;
    }
}

/* -------------------- LevelPack_Enigma -------------------- */

LevelPack_Enigma::LevelPack_Enigma (const string &initfile, const string &n)
: m_initfile(initfile), m_name(n)
{
    reinit();
}


const LevelInfo &
LevelPack_Enigma::get_info (size_t index) const
{
    assert(index < m_levels.size() || "invalid level index");
    return m_levels[index];
}

int LevelPack_Enigma::get_revision_number(size_t index) const {
    return get_info(index).revision;
}

void LevelPack_Enigma::load_index (istream &is)
{
    int linenumber = 0;

    try {
        m_levels.clear();

        string line;
        while (getline(is, line)) {
            ++linenumber;
            LevelInfo info;
            if (parse_levelinfo (line, info))
                m_levels.push_back(info);
        }
    }
    catch (const XLevelPackInit &e) {
        string xerror = strf("in line %i: %s", linenumber, e.what());
        throw XLevelPackInit (xerror);
    }
}

void LevelPack_Enigma::reinit()
{
    if (m_initfile == "") 
        return;

    string      filename = enigma::FindDataFile(m_initfile);
    ifstream    is(filename.c_str());

    if (!is)
        throw XLevelPackInit ("Cannot open index file");
        
    load_index(is);
}

void LevelPack_Enigma::load_level (istream &is) 
{
    world::PrepareLevel (20,13);
    file::ByteVec luacode;
    file::readfile (is, luacode);

    lua_State *L = lua::LevelState();
    if (lua::Dobuffer(L, luacode) != 0) {
//        clear_world();
        throw XLevelLoading (lua::LastError(L));
    }
    world::InitWorld();
}

void LevelPack_Enigma::load_level_xml (istream &is)
{
    world::PrepareLevel (20,13);
    file::ByteVec xmlcode;
    file::readfile (is, xmlcode);
    lua_State *L = lua::LevelState();
    if (lua::CallFunc (L, "LoadLevelXML", xmlcode)) {
        throw XLevelLoading (lua::LastError(L));
    }
    world::InitWorld();
}

void LevelPack_Enigma::load_level (size_t index)
{
    const LevelInfo &info = get_info(index);
    string filename;
    if (file::FindFile ("levels/" + info.filename + ".xml", filename)) {
        ifstream ifs(filename.c_str());
        load_level_xml (ifs);
    }
    else if (file::FindFile ("levels/" + info.filename + ".lua", filename)) {
        ifstream ifs(filename.c_str());
        load_level (ifs);
    }
    else {
        throw XLevelLoading("Could not find level `" + info.filename );
    }
}


/* -------------------- LevelPack_CommandLine -------------------- */


LevelPack_CommandLine::LevelPack_CommandLine (
    const vector<string> &levelfiles,
    const string &name)
{
    set_name (name);
    char buffer[256];
    for (unsigned i=0; i<levelfiles.size(); i++) {
        LevelInfo info;
        info.filename = levelfiles[i];
        snprintf (buffer, sizeof(buffer)-1, "Level %d", int(i));
        buffer[sizeof(buffer)] = '\0';
        info.name = buffer;
        append_level_info (info);
    }
}

void LevelPack_CommandLine::load_level (size_t index)
{
    const LevelInfo &info = get_info(index);
    string filename = info.filename;

    if (!ecl::FileExists ( filename )) { // Do not modify filename, use as is
        throw XLevelLoading("Could not find level `" + info.filename );
    }

    size_t extbegin = filename.rfind ('.');
    if (extbegin != string::npos) {
        string ext = filename.substr (extbegin);
        
        if (ext == ".lua" || ext == ".ell") {
            ifstream ifs(filename.c_str());
            LevelPack_Enigma::load_level (ifs);
        }
        else if (ext == ".xml" || ext == ".elx") {
            ifstream ifs(filename.c_str());
            load_level_xml (ifs);
        }
        else{
            throw XLevelLoading ("Unknown file extension in " + info.filename);
        }
    }
}


/* -------------------- LevelPack_Zipped -------------------- */

LevelPack_Zipped::LevelPack_Zipped (const string &zipfile)
: m_filename (zipfile), m_zip()
{
    reinit();
}

LevelPack_Zipped::~LevelPack_Zipped()
{
}

void LevelPack_Zipped::reinit()
{
    try {
        m_zip.reset (new ZipFile (m_filename));

        auto_ptr<istream> isptr (m_zip->getInputStream ("index.txt"));
        istream &is = *isptr;

        string line;
        if (getline(is, line)) {
            m_name = line;
            load_index (is);
        }
        else {
            clear_index();
            throw XLevelPackInit ("Invalid level pack: " + m_filename);
        }
    }
    catch (std::exception &x) {
        throw XLevelPackInit ("Error reading from .zip file: " + m_filename);
    }
}

void LevelPack_Zipped::load_level (size_t index)
{
    const LevelInfo &levelinfo = get_info(index);
    string filename = levelinfo.filename + ".lua";
    try {
        auto_ptr<istream> isptr (m_zip->getInputStream (filename));
        Assert<XLevelLoading> (isptr.get() != 0, 
                               "File not found in ZIP file: " + 
                               m_filename + "/" + filename);
        
        LevelPack_Enigma::load_level (*isptr);
    }
    catch (XLevelLoading &) {
        throw;                  // rethrow
    }
    catch (std::exception &x) {
        throw XLevelLoading("Unknown error in loading from ZIP file: " + 
                            m_filename + "/" + filename);
    }
}


/* -------------------- Functions -------------------- */

// contains all levels packs added by AddLevelPack and AddZippedLevelPack
static set<string> addedLevelPacks;

void levels::AddLevelPack (const char *init_file, const char *name)
{
    if (addedLevelPacks.find(init_file) == addedLevelPacks.end()) {
        string filename;
        if (FindFile(init_file, filename)) {
            try {
                RegisterLevelPack (new LevelPack_Enigma (init_file, name));
                addedLevelPacks.insert(init_file);
            }
            catch (const XLevelPackInit &e)
            {
                cerr << e.get_string() << endl;
            }
        } else {
            enigma::Log << "Could not find level index file `" << init_file << "'\n";
        }
    }
}

void levels::AddSimpleLevelPack (const std::vector<std::string> &levels, 
                                 const char *name)
{
    try {
        RegisterLevelPack (new LevelPack_CommandLine (levels, name));
    }
    catch (const XLevelPackInit &e)
    {
        cerr << e.get_string() << endl;
    }
}


void levels::AddZippedLevelPack (const char *zipfile)
{
    if (addedLevelPacks.find(zipfile) == addedLevelPacks.end()) {
        string filename;
        if (FindFile (zipfile, filename)) {
            RegisterLevelPack (new LevelPack_Zipped (filename));
            addedLevelPacks.insert(zipfile);
        } else {
            enigma::Log << "Could not find zip file `" << zipfile << "'\n";
        }
    }
}
