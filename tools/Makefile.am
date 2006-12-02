# Build font converter only if SDL_ttf library is present
# and developer tools were explicitly requested (cf. configure.ac).

EXTRA_DIST = merge_level_i18n.lua sub_quote_gettext.lua 

if BUILDTTF2BMF
TTFTOOL = ttf2bmf
else
TTFTOOL = 
endif

noinst_PROGRAMS = tolua $(TTFTOOL)

ttf2bmf_SOURCES = ttf2bmf.cc getopt.c getopt1.c getopt.h
ttf2bmf_CXXFLAGS = @SDL_CFLAGS@ $(AM_CFLAGS)
ttf2bmf_LDADD = -lSDL_ttf $(AM_LDADD)

tolua_SOURCES = tolua.c toluabind.c toluabind.h

tolua_CFLAGS = -I$(top_srcdir)/lib-src/lua $(AM_CFLAGS)
tolua_LDADD = -L$(top_builddir)/lib-src/lua -llua $(AM_LDADD)
tolua_CPPFLAGS= $(CPP_FLAGS)

if CXXLUA
tolua_CPPFLAGS += -x c++ $(AM_CXXFLAGS) $(CXXFLAGS)
tolua_LDADD += -lstdc++
endif
