/*
 * Copyright (C) 2006 Ronald Lamprecht
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
 */

#include "lev/PersistentIndex.hh"
#include "lev/Proxy.hh"
#include "lev/RatingManager.hh"
#include "errors.hh"
#include "main.hh"
#include "file.hh"
#include "options.hh"
#include "oxyd.hh"


#include <fstream>

namespace enigma { namespace lev {

    void PersistentIndex::registerPersistentIndices() {
        // register dirs and zips with xml-indices incl auto 
        
        // register auto not yet registered new files
        PersistentIndex * autoIndex = new PersistentIndex("/levels/auto", "Auto");
        DirIter * dirIter = DirIter::instance(app.userPath + "/levels/auto");
        DirEntry dirEntry;
        while (dirIter->get_next(dirEntry) && !dirEntry.is_dir) {
            if ((dirEntry.name.rfind(".xml") == dirEntry.name.size() - 4) ||
                    (dirEntry.name.rfind(".lua") == dirEntry.name.size() - 4)) {
                Proxy * newProxy = Proxy::autoRegisterLevel("auto", 
                        dirEntry.name.substr(0, dirEntry.name.size() - 4));
                if (newProxy != NULL) {
                    // first check that the proxy is not in the index
                    //  - may occur if the level is stored as .xml and .lua in the folder
                    
                    // is it is new, add it
                    autoIndex->appendProxy(newProxy);
                }
            }
        }
        delete dirIter;
        Index::registerIndex(autoIndex);
    }

    PersistentIndex::PersistentIndex(std::string thePackPath,  
            std::string anIndexName, std::string theIndexFilename, 
            std::string aGroupName) : 
            Index(anIndexName, aGroupName), packPath (thePackPath), 
            indexFilename(theIndexFilename) {
        Log << "PersistentIndex AddLevelPack " << thePackPath << " - " << anIndexName << "\n";
        lev::RatingManager *theRatingMgr = lev::RatingManager::instance();
        
    }
        
    PersistentIndex::~PersistentIndex() {}
    
    void PersistentIndex::clear() {
        proxies.clear();
    }
    


    PersistentIndex::PersistentIndex(std::istream *legacyIndexStream, 
            std::string thePackPath, bool isZip, std::string anIndexName, 
            std::string theIndexFilename) : 
            Index(anIndexName, INDEX_DEFAULT_GROUP), 
            indexFilename(theIndexFilename) {
//        Log << "PersistentIndex convert legacy index " << thePackPath << " - " << anIndexName << "\n";
        lev::RatingManager *theRatingMgr = lev::RatingManager::instance();

        // prepare Proxy coding of pack path -        
        if (thePackPath == "levels")
            packPath = "";
        else {
            //  cut off leading "levels/" 
            packPath = thePackPath.substr(7);
        }

        
        int linenumber = 0;
        try {
            std::string line;
            while (std::getline(*legacyIndexStream, line)) {
                using namespace std;
                using namespace ecl;
    
                string   filename = "";              //< Filename of the level (exl. extension)
                string   indexname = "";             //< The name used in the options file, 
                                                //   empty string -> use filename entry
                string   name = "";                  //< Complete name of the level
                string   author = "";                //< Author of the level
                int      revision = 1;              //< Revision # of this level
                bool     has_easymode = false;          //< whether level has an easymode
                int      par_time_easy = -1 ;        //< Best time in seconds (for easy mode)
                int      par_time_normal = -1;      //< Best time in seconds (for normal mode)
                string   par_time_easy_by = "";     //< player name(s) for 'best_time_easy'
                string   par_time_normal_by = "";   //< same for 'best_time_normal'
                int      par_moves = -1;            //< Minimum moves to solve level
                int      intelligence = 0;
                int      dexterity = 0;
                int      patience = 0;
                int      knowledge = 0 ;
                int      speed = 0;
 
                ++linenumber;
   
                const char *wspace = " \t";
                size_t      p      = line.find_first_not_of(wspace);
        
                if (p == string::npos || line.at(p) != '{')
                    continue;
        
                // found a level description
        
                int    par_time        = -1;
                string par_time_by;
                string par_moves_by;
        
                ++p;
                while (true) {
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
        
                    if      (tag == "file")     filename     = content;
                    else if (tag == "indexname")indexname    = content;
                    else if (tag == "name")     name         = content;
                    else if (tag == "author")   author       = content;
                    else if (tag == "revision") revision     = atoi(content.c_str());
                    else if (tag == "easymode") has_easymode = (content == "1");
                    else if (tag == "int")      intelligence = atoi(content.c_str());
                    else if (tag == "dex")      dexterity    = atoi(content.c_str());
                    else if (tag == "pat")      patience     = atoi(content.c_str());
                    else if (tag == "kno")      knowledge    = atoi(content.c_str());
                    else if (tag == "spe")      speed        = atoi(content.c_str());
                    else if (tag == "par_time")        parsePar(content, par_time, par_time_by);
                    else if (tag == "par_time_easy")   parsePar(content, par_time_easy, par_time_easy_by);
                    else if (tag == "par_time_normal") parsePar(content, par_time_normal, par_time_normal_by);
                    else if (tag == "par_moves")       parsePar(content, par_moves, par_moves_by);
        //                 else if (tag == "hint1") hint1           = content;
        //                 else if (tag == "hint2") hint2           = content;
                    else
                        throw XLevelPackInit(strf("unknown tag '%s'", tag.c_str()));
                }
        
                Assert <XLevelPackInit> (filename.length() != 0, 
                                         "mandatory tag 'file' missing");
        
                if (has_easymode) {
                    Assert <XLevelPackInit> (par_time == -1,
                                             "'par_time' not allowed when easymode=1 "
                                             "(use 'par_time_easy' and 'par_time_normal')");
                }
                else {
                    Assert <XLevelPackInit> (par_time_normal==-1 && par_time_easy==-1,
                            "'par_time_easy' and 'par_time_normal' are not allowed when easymode=0 (use 'par_time')");
                    par_time_easy    = par_time_normal    = par_time;
                    par_time_easy_by = par_time_normal_by = par_time_by;
                }
                
                // correct filename for zip 
                if (isZip) {
                    Log << "Zip " << thePackPath << " - " <<packPath << " - " << indexname << "\n";
                    if (filename.find('/') == std::string::npos) {
                        // it is an old zip file with plain levelnames as filename
                        if (indexname.empty()) {
                            // keep id
                            indexname = filename;
                        }
                        // add the zip basename to get the complete path to the level
                        filename = packPath + "/" + filename;
                    }
                } else if (linenumber == 1 && packPath.empty()) {
                    // a std index.txt on levels dir - we need to guess location of pack
                    // we take the dir of the first level
                    std::string::size_type n = filename.rfind('/');
                    if (n != std::string::npos) {
                        packPath = filename.substr(0, n);
//                        Log << "Index convert pack '" << anIndexName << "' - guess pack directory is '" 
//                                << packPath << "'\n";
                    }
                }
                
                
                // register Proxy
                appendProxy(Proxy::registerLevel(filename, packPath,
                        (indexname.empty() ? filename : indexname), name, author, 
                        revision, revision, has_easymode, GAMET_ENIGMA));
                theRatingMgr->registerRating((indexname.empty() ? filename : indexname),
                    revision, intelligence, dexterity, patience, knowledge, speed, 
                    par_time_easy, par_time_easy_by, par_time_normal, par_time_normal_by);
                    
            }
        } catch (const XLevelPackInit &e) {
            std::string xerror = ecl::strf("in line %i: %s", linenumber, e.what());
            throw XLevelPackInit (xerror);
        }  
    }

    void PersistentIndex::parsePar(const string& par, int& par_value, std::string& par_text) 
    {
        // 'par' is in format "value,text"
        // The value is stored in 'par_value' and the text in 'par_text'.
        using namespace std;
        using namespace ecl;

        size_t comma = par.find(',');
        Assert<XLevelPackInit> (comma!=string::npos, "Comma expected in par");

        string value = par.substr(0, comma);
        par_text     = par.substr(comma+1);
        par_value    = atoi(value.c_str());
    }

    void AddLevelPack (const char *init_file, const char *indexName) {
        Log << "Index AddLevelPack " << init_file << "\n";
        if (Index::findIndex(indexName) == NULL) {
            std::string absPath;
            if (app.resourceFS->findFile(init_file, absPath)) {
                try {
                    std::string path = init_file;
                    std::string dir;
                    std::string filename;
                    ecl::split_path(path, &dir, &filename);
                    std::ifstream is(absPath.c_str());
                
                    if (!is)
                        throw XLevelPackInit ("Cannot open index file");
                    Index::registerIndex(new PersistentIndex(&is, dir, false, indexName));
                } catch (const XLevelPackInit &e) {
                    Log << e.get_string() << "\n";
                }
            } else {
                Log << "Could not find level index file `" << init_file << "'\n";
            }
        } else {
//            Log << "Ignored index file `" << init_file << "' - already converted to XML\n";
        }
    }

    void AddZippedLevelPack (const char *zipfile) {
        Log << "Index AddZippedLevelPack " << zipfile << "\n";
        using namespace std;
        using namespace ecl;
        string absPath;
        if (app.resourceFS->findFile (zipfile, absPath)) {
            // the index file as it would be for a unpacked zip
            std::string zf = zipfile;
            std::string dir = zf.substr(0, zf.rfind('.'));
            std::string indexfile = dir + "/index.txt";
            try {
                auto_ptr<istream> isptr;
                std::string dummy;
                if(!app.resourceFS->findFile(indexfile, dummy, isptr))
                    throw XLevelPackInit ("No index in level pack: ");
                
                istream &is = *isptr;
        
                string line;
                std::string indexName;
                if (getline(is, line)) {
                    // we read the index in binary mode and have to strip of the \n for
                    // windows
                    if (line[line.size()-1] = '\n') {
                        line.resize(line.size()-1);
                    }
                    indexName = line;
                    
                    // check if already loaded
                    
                    Index::registerIndex(new PersistentIndex(isptr.get(), dir, true, indexName));
                }
                else {
                    throw XLevelPackInit ("Invalid level pack: " + indexName);
                }
            }
            catch (std::exception &) {
                throw XLevelPackInit ("Error reading from .zip file: " + indexfile);
            }
        } else {
            enigma::Log << "Could not find zip file `" << zipfile << "'\n";
        }
    }

}} // namespace enigma::lev
