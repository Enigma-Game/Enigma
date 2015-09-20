/*
 * Copyright (C) 2002,2004,2015 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#ifndef ENIGMA_RESOURCE_CACHE_HH
#define ENIGMA_RESOURCE_CACHE_HH

#include "ecl_font.hh"
#include "ecl_cache.hh"
#include "ecl_video.hh"
#include <string>

namespace enigma {

class ImageCache : public ecl::PtrCache<ecl::Surface> {
public:
    using ecl::PtrCache<ecl::Surface>::store;

    ecl::Surface *acquire(const std::string &name) {
        return ecl::LoadImage(name.c_str());
    }
};

/* The `Get...' functions return a pointer to a cached copy of the
font or image.  The `Load...' functions load a new copy which
you must deallocate yourself.  */

void DefineFont(const char *name, const char *ttf_name, int ttf_size, const char *bmf_name, int r,
        int g, int b);
ecl::Font *GetFont(const char *name);
void ClearFontCache();

ecl::Surface *LoadImage(const char *name);
ecl::Surface *GetImage(const char *name, const char *ext = ".png");
ecl::Surface *RegisterImage(const char *name, ecl::Surface *s);
void ClearImageCache();

} // namespace enigma

#endif // ENIGMA_RESOURCE_CACHE_HH
