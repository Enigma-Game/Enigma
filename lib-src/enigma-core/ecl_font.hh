/*
 * Copyright (C) 2002,2004 Daniel Heck
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
#ifndef ECL_FONT_HH
#define ECL_FONT_HH

namespace ecl
{
    class GC;
    class Surface;

    class Font {
    public:
	virtual ~Font() {}

        virtual int get_lineskip() =0;
	virtual int get_height() = 0;

	virtual int get_width(char c) = 0;
	virtual int get_width(const char *str);

	virtual Surface *render(const char *str) = 0;
        virtual void render(const GC &gc, int x, int y, 
                            const char *str) = 0;
    };

    /** Load a bitmap font with image FILENAME and font metric
      DESCRNAME. */
    Font *LoadBitmapFont(const char * filename,
                         const char * descrname);
			
    /** Load a TrueType font from FILENAME with size PTSIZE. */ 
    Font *LoadTTF (const char *filename, int ptsize, int r=0xff, int g=0xff, int b=0xff);

}
#endif
