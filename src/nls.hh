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

namespace nls
{
    void SetMessageLocale (const std::string &language);

    struct Language {
        const char *name;
        const char *localename;
        const char *flagimage;
    };
    
    const Language languages[] = {
        { "default",    "",      "par" },
        { "Deutsch",    "de_DE", "flags25x15/de" },
        { "English",    "en_EN", "flags25x15/gb" },
        { "Español",    "es_ES", "flags25x15/es" },
        { "Français",   "fr_FR", "flags25x15/fr" },
        { "Italiano",   "it_IT", "flags25x15/it" },
        { "Nederlands", "nl_NL", "flags25x15/nl" },
        { "Svenska",    "sv_SE", "flags25x15/se" },
        { "Русский",    "ru_RU", "flags25x15/ru" },
        { "Magyar",     "hu_HU", "flags25x15/hu" },
        { "Português",  "pt_BR", "flags25x15/pt" },
        { "Suomi",      "fi_FI", "flags25x15/fi" },
        { "українська", "uk_UA", "flags25x15/ua" },
        { "беларуская", "be_BY", "flags25x15/by" },
        { "Ελληνικά",   "el_GR", "flags25x15/gr" },
    };
}

#endif
