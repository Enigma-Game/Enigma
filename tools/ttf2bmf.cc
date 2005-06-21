//======================================================================
// Copyright (C) 2002 Daniel Heck
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
//======================================================================

/*
 * This program converts TrueType fonts to special bitmapped fonts.
 * These bitmap fonts constists (a) of an image file containing the
 * individual character glyphs and (b) a simplified font metrics file
 * describing the dimension of the characters and their coordinates
 * inside the image.
 *
 * The image file is a regular 32bit BMP, the font metrics file is a
 * text file with each line describing one particular character.  Each
 * line contains four numbers, separated by white space:
 *
 *      * character number (ASCII/ISO code of the character)
 *	* x coordinate of the character glyph (in pixels)
 *	* width of the character glyph (in pixels)
 *	* logical width of the character (in pixels)
 *
 * For a list of command line options, call `ttf2bmf --help'.
 */

#include "SDL_ttf.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include "getopt.h"

using namespace std;

//----------------------------------------
// Variables
//----------------------------------------


/* The first and last characters that are included in a font unless
   the "-g" option is passed to the program. */
const char firstchar = 32;
const char lastchar = 127;

/* This string contains the characters that are converted. */
string chars_to_render;

/* Glyph size in pixels. */
int glyph_size = 12;

/* Use anti-aliasing? */
bool antialias_p = false;

/* Filename of the True Type font. */
string ttfname;

/* Name of the output file (without extension). */
string output_file;

SDL_Color fgcolor = { 0xff, 0xff, 0xff, 0};
SDL_Color bgcolor = { 0, 0, 0, 0 };


//----------------------------------------
// Functions
//----------------------------------------


static void
usage()
{
    cout << "ttf2bmf [options] file.ttf\n\n"
         << "-o name      basename of the output files\n"
         << "-a           use antialiasing\n"
         << "-c rgb       foreground color as RGB (e.g., 0xff0000 is red)\n"
         << "-s pxsize    size of the generated font in pixels\n"
         << "-g chars     only render glyphs for certain characters\n"
         << "-h           print this help message\n";
}

static void
start_conversion (TTF_Font* font)
{
    ofstream fontinfo((output_file + ".bmf").c_str());

    char str[2];
    str[1] = 0;

    // Calculate size for pixmap
    int width=0, height=0;
    for (unsigned i=0; i<chars_to_render.size(); ++i)
    {
        int c = chars_to_render[i];
        int w,h;
        str[0]=c;
        TTF_SizeText(font, str, &w, &h);
	width += w;
    }
    height = TTF_FontHeight(font);
    SDL_Surface *glyphs = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                               width, height, 32, 0,0,0,0);
    SDL_FillRect(glyphs, 0, SDL_MapRGB(glyphs->format,0xff,0,0xff));

    // Render the glyphs
    int xoff=0;
    for (unsigned i=0; i<chars_to_render.size(); ++i)
    {
        int c = chars_to_render[i];

	SDL_Surface *glyph = 0;
        str[0] = c;
        if (antialias_p) {
            glyph = TTF_RenderText_Shaded(font, str, fgcolor, bgcolor);
            SDL_SetColorKey(glyph, SDL_SRCCOLORKEY,0);
        }
        else
            glyph = TTF_RenderText_Solid(font, str, fgcolor);
        assert(glyph);

	int minx, maxx, miny, maxy, advance;
	TTF_GlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &advance);

        SDL_Rect dr;
        dr.x = xoff;
	dr.y = 0;
        dr.w = glyph->w;
        dr.h = glyph->h;

	SDL_BlitSurface(glyph, NULL, glyphs, &dr);
        SDL_FreeSurface(glyph);

        fontinfo << c << " "<<xoff<<" "<<glyph->w<<" " <<advance<<endl;
	xoff += glyph->w;
    }
    SDL_SaveBMP(glyphs, (output_file + ".bmp").c_str());
}

int
main (int argc, char **argv)
{
    for (int c=firstchar; c<=lastchar; ++c)
        chars_to_render += static_cast<char>(c);

    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "ao:c:s:hg:")) != -1) {
        switch (c) {
        case 'a':
            antialias_p = true;
            break;
        case 'o':
            output_file = optarg;
            break;
        case 'g':
            chars_to_render = optarg;
            break;
        case 'c':
            {
                int color;
                sscanf(optarg, "%x", &color);
                fgcolor.r = (color >> 16) & 0xff;
                fgcolor.g = (color >> 8) & 0xff;
                fgcolor.b = color & 0xff;
            }
            break;
        case 's':
            glyph_size = atoi(optarg);
            break;
        case 'h':
            usage();
            return 0;
        case '?':
            cerr << "Unknown option `-" << optopt << "'.\n";
            return 1;
        default:
            abort();
        }
    }
    if (optind + 1 < argc) {
        cerr << "More than one input file specified.\n";
        return 1;
    } else if (optind == argc) {
        cerr << "No input file specified.\n";
        return 1;
    }
    ttfname = argv[optind];

    // Create default output file name if none specified
    if (output_file == "") {
        char xx[10];
        sprintf(xx, "%d", glyph_size);
        output_file = string(ttfname, 0, ttfname.find(".ttf"));
        output_file += xx;
    }

    // Initialize True Type library & open font
    if (TTF_Init() < 0) {
	cerr << "cannot initialize SDL_ttf: " << TTF_GetError() << endl;
	return 1;
    }
    TTF_Font *font = TTF_OpenFont(ttfname.c_str(), glyph_size);
    if (!font) {
	cerr << "cannot open font: " << TTF_GetError() << endl;
	return 1;
    }

    start_conversion (font);
}
