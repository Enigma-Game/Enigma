#ifndef NLS_HH_INCLUDED
#define NLS_HH_INCLUDED
#include "config.h"

/* Use N_(...) for strings that should be included in pot, but are not
   translated at that particular point in the source (but maybe later). */
#define _(String) nls::translate(String).c_str()
#define N_(String) (String)

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
        bool        replaceApostrophe;
    };
    
    const Language languages[] = {
        { "default",     "",      true },
        { "беларуская",  "be_BY", true },
        { "Bosanski",    "bs_BA", true },
        { "Česky",       "cs_CZ", true },
        { "Dansk",       "da_DK", true },
        { "Deutsch",     "de_DE", true },
        { "Ελληνικά",    "el_GR", true },
        { "English",     "en_GB", true },
        { "Español",     "es_ES", true },
        { "Français",    "fr_FR", true },
        { "Gàidhlig",    "gd_GB", true },
        { "Hrvatski",    "hr_HR", true },
        { "Italiano",    "it_IT", true },
        { "Magyar",      "hu_HU", true },
        { "Nederlands",  "nl_NL", true },
        { "Norsk",       "no_NO", true },
        { "Polski",      "pl_PL", true },
        { "Português",   "pt_BR", true },
        { "Русский",     "ru_RU", true },
        { "Slovenčina",  "sk_SK", true },
        { "Slovenščina", "sl_SI", true },
        { "Suomi",       "fi_FI", true },
        { "Svenska",     "sv_SE", true },
        { "українська",  "uk_UA", true },
        { "中文",         "zh_CN", false },
        { "日本語",       "ja_JP", false },
     };

    std::string replaceApostrophe(std::string text);

    static inline std::string translate(const std::string& msg) {
        if (theDictionaryManager)
            return replaceApostrophe(theDictionaryManager->get_dictionary().translate(msg));
        return msg;
    }

    static inline std::string ntranslate(const std::string& msg, const std::string& msg_plural, int num) {
        if (theDictionaryManager) {
            std::string msgt = theDictionaryManager->get_dictionary().translate(msg);
            std::string msgt_plural = theDictionaryManager->get_dictionary().translate(msg_plural);
            return replaceApostrophe(theDictionaryManager->get_dictionary().translate_plural(msgt, msgt_plural, num));
        }
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
