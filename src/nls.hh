#ifndef NLS_HH_INCLUDED
#define NLS_HH_INCLUDED
#include "config.h"

#if defined(ENABLE_NLS)
#  include <libintl.h>
#  define _(String) gettext(String)
#  define gettext_noop(String) (String)
#  define N_(String) gettext_noop(String)
#else // !defined(ENABLE_NLS) 
#  define _(String) (String)
#  define N_(String) (String)
#endif

#include <string>
#include "tinygettext/include/tinygettext/tinygettext.hpp"
#include "tinygettext/include/tinygettext/file_system.hpp"

namespace nls
{
    extern std::unique_ptr<tinygettext::DictionaryManager> theDictionaryManager;

    void SetMessageLocale (const std::string &language);

    struct Language {
        const char *name;
        const char *localename;
        const char *flagimage;
    };
    
    const Language languages[] = {
        { "default",    "",      "par" },
        { "беларуская", "be_BY", "flags25x15/by" },
        { "Česky",      "cs_CZ", "flags25x15/cz" },
        { "Dansk",      "da_DK", "flags25x15/dk" },
        { "Deutsch",    "de_DE", "flags25x15/de" },
        { "Ελληνικά",   "el_GR", "flags25x15/gr" },
        { "English",    "en_GB", "flags25x15/gb" },
        { "Español",    "es_ES", "flags25x15/es" },
        { "Suomi",      "fi_FI", "flags25x15/fi" },
        { "Français",   "fr_FR", "flags25x15/fr" },
        { "Gàidhlig",   "gd_GB", "flags25x15/gb-sct" },
        { "Hrvatski",   "hr_HR", "flags25x15/hr" },
        { "Magyar",     "hu_HU", "flags25x15/hu" },
        { "Italiano",   "it_IT", "flags25x15/it" },
        { "Nederlands", "nl_NL", "flags25x15/nl" },
        { "Polski",     "pl_PL", "flags25x15/pl" },
        { "Português",  "pt_BR", "flags25x15/pt" },
        { "Русский",    "ru_RU", "flags25x15/ru" },
        { "Svenska",    "sv_SE", "flags25x15/se" },
        { "Slovenčina", "sk_SK", "flags25x15/sk" },
        { "українська", "uk_UA", "flags25x15/ua" },
    };

    static inline std::string translate(const std::string& msg) {
    #if defined(ENABLE_NLS)
        if (theDictionaryManager)
            return theDictionaryManager->get_dictionary().translate(msg);
    #endif
        return msg;
    }

    static inline std::string ntranslate(const std::string& msg, const std::string& msg_plural, int num) {
    #if defined(ENABLE_NLS)
        if (theDictionaryManager)
            return theDictionaryManager->get_dictionary().translate_plural(msg, msg_plural, num);
    #endif
        if (num == 1)
            return msg;
        else
            return msg_plural;
    }

    // Interface for tinygettext
    class TinyGetTextFileSystem : public tinygettext::FileSystem {
    public:
        std::vector<std::string> open_directory(const std::string& pathname) override;
        std::unique_ptr<std::istream> open_file(const std::string& filename) override;
    };
}

#endif
