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
#include "ecl_font.hh"
#include "ecl_geom.hh"
#include "ecl_video.hh"
#include <vector>
#include <string>
#include <memory>               // for auto_ptr
#include <stdio.h>

#include <config.h>

using namespace ecl;
using namespace std;

int Font::get_width(const char *str) 
{
    int w=0;
    for (const char *p = str; *p; ++p)
	w += get_width(*p);
    return w;
}

std::string::size_type Font::breakString(const std::string &theString,
        const std::string &breakChars, const int width) {
    std::string::size_type lastpos = std::string::npos;
    std::string::size_type pos = 0;
    do {
        std::string::size_type nextpos = theString.find_first_of(breakChars, pos);
        if (nextpos == std::string::npos) {
            // there is no further break pos
            if (lastpos == std::string::npos) {
                // there was no break at all
                return theString.size();
            } else {
                if (get_width(theString.c_str()) <= width) { 
                    // the complete string fits into a line
                    return theString.size();
                } else {
                    // just the last chunk was too much
                    return lastpos;
                }
            }
        } else {
            if (get_width(theString.substr(0, nextpos+1).c_str()) <= width) { 
                lastpos = nextpos+1;
                pos = lastpos;
            } else {
                // now the string is too long
                if (lastpos == std::string::npos) {
                    // but there is no other break before
                    return nextpos+1;
                } else {
                    return lastpos;
                }
            }
        }
    } while (true);
}

//
// Bitmap fonts
//

namespace
{
    class InvalidFont {};

    class BitmapFont : public Font {
	vector<Rect>	char_rects;
	vector<int>	advance;
	Surface		*surface;
    public:
	BitmapFont(Surface *s, const char *descr);
	~BitmapFont() { delete surface; }

	int get_lineskip() { return surface->height() + 3; }
	int get_width(char c);
	int get_height();

	Surface *render(const char *str);
        void render(const GC &gc, int x, int y, const char *str);
    };
}

BitmapFont::BitmapFont(Surface *s, const char *descr)
    : char_rects(256), advance(256), surface(s)
{
    // Read and interpret the font description file.
    // expected line format:
    // charno xpos width xadvance

    FILE *fp=fopen(descr, "rt");
    if (!fp) return ; //throw InvalidFont();

    int c;
    int x=0, w=0, adv=0;
    while (fscanf(fp, "%d %d %d %d\n", &c, &x, &w, &adv) != EOF) {
        char_rects[c].x = x;
        char_rects[c].w = w;
        char_rects[c].y = 0;
        char_rects[c].h = s->height();
        advance[c] = adv;
    }
}

int BitmapFont::get_width(char c) {
    return advance[int(c)];
}

int BitmapFont::get_height() {
    return surface->height();
}

Surface *
BitmapFont::render(const char *str)
{
    Surface *s = MakeSurface(Font::get_width(str), get_height(), 16);
    s->set_color_key(0,0,0);
    render (GC(s), 0, 0, str);
    return s;
}

void
BitmapFont::render(const GC &gc, int x, int y, const char *str)
{
    for (const char *p=str; *p; ++p) {
        blit(gc, x, y, surface, char_rects[int(*p)]);
        x += get_width(*p);
    }
}


Font *
ecl::LoadBitmapFont(const char * imgname, const char * descrname)
{
    if (Surface *s = LoadImage(imgname))
        return new BitmapFont(s, descrname);
    return 0;
}



#ifndef HAVE_SDLTTF

Font *ecl::LoadTTF (const char * /*filename*/, int /*ptsize*/, int, int, int)
{
    return 0;
}

#else /* if defined (HAVE_SDLTTF) */

//
// TrueType fonts (using SDL_ttf if available)
//
#include "SDL_ttf.h"

namespace
{
    class TrueTypeFont : public Font {
    	// Variables
	TTF_Font *font;
        SDL_Color fgcolor;


        // Inhibit copying
        TrueTypeFont (const TrueTypeFont &);
        TrueTypeFont &operator= (const TrueTypeFont &);
    public:
    	TrueTypeFont (TTF_Font *font_, int r, int g, int b);
	
	~TrueTypeFont();
	
    	// Font interface
	int get_lineskip();
	int get_height();
	int get_width (char c);
        int get_width (const char *str);

	Surface *render (const char *str);
	void     render (const GC &gc, int x, int y, const char *str);
    };
}

TrueTypeFont::TrueTypeFont (TTF_Font *font_, int r, int g, int b)
: font (font_)
{
    fgcolor.r = r;
    fgcolor.g = g;
    fgcolor.b = b;
}
	
TrueTypeFont::~TrueTypeFont()
{
    TTF_CloseFont (font);
}


int TrueTypeFont::get_lineskip() { 
    return TTF_FontLineSkip (font); 
}
int TrueTypeFont::get_height() { 
    return TTF_FontHeight(font); 
}
	
int TrueTypeFont::get_width(char c) {
    int minx, maxx, miny, maxy, advance;
    TTF_GlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &advance);
    return advance;
}


Surface *TrueTypeFont::render (const char *str) 
{
    SDL_Surface *s = 0;
    SDL_Color bgcolor = { 0, 0, 0, 0 };

    s = TTF_RenderUTF8_Shaded (font, str, fgcolor, bgcolor);
    if (s) {
        SDL_SetColorKey (s, SDL_SRCCOLORKEY,0);
        return Surface::make_surface (s);
    }
    return MakeSurface(0, get_height(), 16);
}

void TrueTypeFont::render (const GC &gc, int x, int y, const char *str) 
{
    std::auto_ptr<Surface> s (render (str));
    if (s.get())
        blit (gc, x, y, s.get());
}

int TrueTypeFont::get_width(const char *str) 
{
    int w,h;
    TTF_SizeUTF8 (font, str, &w, &h);
    return w;
}

Font *ecl::LoadTTF (const char *filename, int ptsize, int r, int g, int b)
{
    if (TTF_Init()) {
    	// Error initializing TTF engine
    }
    TTF_Font *font = TTF_OpenFont (filename, ptsize);
    return (font) ? new TrueTypeFont (font, r, g, b) : 0;
}

#endif
