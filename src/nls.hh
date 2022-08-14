#ifndef NLS_HH_INCLUDED
#define NLS_HH_INCLUDED
#include "config.h"

#if defined(ENABLE_NLS)
#  include <libintl.h>
#  define _(String) nls::translate(String).c_str()
//#  define _(String) gettext(String)
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

    void SetMessageLocale(const std::string &language);
    void CycleLocale(bool direction);
    int GetCurrentLocaleNr();

    struct Language {
        const char *name;
        const char *localename;
        const char *flagimage;
        bool        replaceApostrophe;
    };
    
    const Language languages[] = {
        { "default",     "",      "par",           true },
        { "беларуская",  "be_BY", "flags25x15/by", true },
        { "Bosanski",    "bs_BA", "",              true },
        { "Česky",       "cs_CZ", "flags25x15/cz", true },
        { "Dansk",       "da_DK", "flags25x15/dk", true },
        { "Deutsch",     "de_DE", "flags25x15/de", true },
        { "Ελληνικά",    "el_GR", "flags25x15/gr", true },
        { "English",     "en_GB", "flags25x15/gb", true },
        { "Español",     "es_ES", "flags25x15/es", true },
        { "Français",    "fr_FR", "flags25x15/fr", true },
        { "Gàidhlig",    "gd_GB", "flags25x15/gb-sct", true },
        { "Hrvatski",    "hr_HR", "flags25x15/hr", true },
        { "Italiano",    "it_IT", "flags25x15/it", true },
        { "Magyar",      "hu_HU", "flags25x15/hu", true },
        { "Nederlands",  "nl_NL", "flags25x15/nl", true },
        { "Norsk",       "no_NO", "",              true },
        { "Polski",      "pl_PL", "flags25x15/pl", true },
        { "Português",   "pt_BR", "flags25x15/pt", true },
        { "Русский",     "ru_RU", "flags25x15/ru", true },
        { "Slovenčina",  "sk_SK", "flags25x15/sk", true },
        { "Slovenščina", "sl_SI", "",              true },
        { "Suomi",       "fi_FI", "flags25x15/fi", true },
        { "Svenska",     "sv_SE", "flags25x15/se", true },
        { "українська",  "uk_UA", "flags25x15/ua", true },
        { "中文",         "zh_CN", "flags25x15/zh-cn", false },
        { "日本語",       "ja_JP", "",                 false },
     };

    std::string replaceApostrophe(std::string text);

    static inline std::string translate(const std::string& msg) {
    #if defined(ENABLE_NLS)
        if (theDictionaryManager)
            return replaceApostrophe(theDictionaryManager->get_dictionary().translate(msg));
    #endif
        return msg;
    }

    static inline std::string ntranslate(const std::string& msg, const std::string& msg_plural, int num) {
    #if defined(ENABLE_NLS)
        if (theDictionaryManager) {
            std::string msgt = theDictionaryManager->get_dictionary().translate(msg);
            std::string msgt_plural = theDictionaryManager->get_dictionary().translate(msg_plural);
            return replaceApostrophe(theDictionaryManager->get_dictionary().translate_plural(msgt, msgt_plural, num));
        }
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

    void tinygettext_log_callback(const std::string& str);
    void tinygettext_error_callback(const std::string& str);
}

#endif
