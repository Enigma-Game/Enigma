#include "nls.hh"
#include "main.hh"
#include "ecl_system.hh"
#include "tinygettext/include/tinygettext/file_system.hpp"
#include "enigma.hh"
#include "options.hh"

#include <fstream>
#include <string>
#include <cstdlib>

#include <config.h>

using namespace std;
using namespace nls;

unique_ptr<tinygettext::DictionaryManager> nls::theDictionaryManager = nullptr;

void nls::SetMessageLocale(const std::string &language)
{
    if (language != "")
        theDictionaryManager->set_language(tinygettext::Language::from_name(language));
}

void nls::CycleLocale(bool direction)
{
    int locnr = GetCurrentLocaleNr();
    locnr = ((locnr + (direction ? 0 : -2)) % (NUMENTRIES(nls::languages) - 1)) + 1;
    std::string localename = nls::languages[locnr].localename;
    enigma::Log << "cycle to: " << localename << endl;
    enigma::app.setLanguage(localename);
    enigma::options::SetOption("Language", localename);
}

int nls::GetCurrentLocaleNr() {
    std::string localename;
    enigma::options::GetOption("Language", localename);
    for (size_t i = 0; i < NUMENTRIES(nls::languages); ++i) {
        if (localename == nls::languages[i].localename)
            return int(i);
    }
    return 0; // unknown or default language
}

std::vector<std::string> TinyGetTextFileSystem::open_directory(const std::string& pathname) {
    std::vector<std::string> result;
    enigma::DirIter * dirIter = enigma::DirIter::instance(pathname);
    enigma::DirEntry dirEntry;
    while (dirIter->get_next(dirEntry))
        if (!dirEntry.is_dir)
            result.push_back(dirEntry.name);
    delete dirIter;
    return result;
}

std::unique_ptr<std::istream> TinyGetTextFileSystem::open_file(const std::string& filename) {
    std::basic_ifstream<char> ifs(filename, ios::binary | ios::in);
    unique_ptr<std::basic_ifstream<char>> isptr = make_unique<std::basic_ifstream<char>>(move(ifs));
    return isptr;
}

void nls::tinygettext_log_callback(const std::string& str) {
    enigma::Log << "tinygettext: " << str;
}

void nls::tinygettext_error_callback(const std::string& str) {
    fputs("tinygettext: ", stderr);
    fputs(str.c_str(), stderr);
}

// The use of unicode character U+2019 as apostrophe AND right single quotation
// mark is heavily disputed. As right single quotation mark, Chinese typography
// demands an additional empty space behind it; using it in other languages
// as apostrophe makes this look very ugly. We therefore have to replace each
// occurence of U+2019 by a similar character; we choose the straight (ASCII)
// apostrophe.
std::string nls::replaceApostrophe(std::string text) {
    std::string s = text;
    if (not nls::languages[GetCurrentLocaleNr()].replaceApostrophe)
        return s;
    while(s.find("’") != string::npos) {
        s.replace(s.find("’"), 3, "'");
    }
    return s;
}
