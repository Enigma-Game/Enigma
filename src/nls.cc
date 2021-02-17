#include "nls.hh"
#include "main.hh"
#include "ecl_system.hh"
#include "tinygettext/include/tinygettext/file_system.hpp"

#include <locale.h>
#include <fstream>
#include <string>
#include <cstdlib>

#include <config.h>

using namespace std;
using namespace nls;

unique_ptr<tinygettext::DictionaryManager> nls::theDictionaryManager = nullptr;

#if !defined (HAVE_SETENV) && defined (HAVE_PUTENV)
static char lang_env[256];
#endif

static char localeutf[256];

static void my_setenv (const char* var, const char* val)
{
#if defined (HAVE_SETENV)
    setenv(var, val, 1);
#elif defined (HAVE_PUTENV)
    snprintf (lang_env, sizeof(lang_env), "%s=%s", var, val);
    putenv(lang_env);
#endif
}

void nls::SetMessageLocale (const std::string &language)
{
    if (language != "") {
        theDictionaryManager->set_language(tinygettext::Language::from_name(language));
#if defined (WIN32) || defined (MACOSX)
        my_setenv ("LANG", language.c_str());
#else // not WIN32 or MACOSX
        if (setlocale (LC_MESSAGES, language.c_str()) != NULL) {
            my_setenv ("LANG", language.c_str());
        } else {
            // Second attempt: lang + .UTF-8
            snprintf(localeutf, sizeof(localeutf), "%s.UTF-8", language.c_str());
            if (setlocale (LC_MESSAGES, localeutf) != NULL) {
                my_setenv ("LANG", localeutf);
            }
        }
#endif
    }

#if defined(ENABLE_NLS) && defined(HAVE_LC_MESSAGES)
    // Hack to fool libintl into changing the message locale more than
    // once
    setlocale (LC_MESSAGES, "C");
    setlocale (LC_MESSAGES, ""); //language.c_str());
#endif

    std::string li = ecl::SysMessageLocaleName();
    enigma::Log << "locale name: " << li << endl;
    enigma::Log << "language code: " << ecl::GetLanguageCode (li) << endl;
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
