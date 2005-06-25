/*
 * Copyright (C) 2002,2003 Daniel Heck
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: pxfwd.hh,v 1.2 2003/03/05 22:06:50 dheck Exp $
 */
#ifndef PXFWD_HH
#define PXFWD_HH

namespace px
{
    class Buffer;

    // math.hh
    template <class T, int N> class Vector;
    class V3;
    class V2;

    // font.hh
    class Font;

    // video.hh
    class Drawable;
    class GC;
    class Screen;
    class Surface;

    // geom.hh
    class RectList;
}
#endif
