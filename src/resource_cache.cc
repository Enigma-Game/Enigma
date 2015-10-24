/*
 * Copyright (C) 2002,2003,2004,2015 Daniel Heck
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
 */
#include "resource_cache.hh"

#include <iostream>

#include "ecl_cache.hh"
#include "ecl_video.hh"
#include "file.hh"
#include "main.hh"

using ecl::Font;

namespace enigma {
namespace {

struct FontDescr {
    // Variables
    std::string name;
    std::string ttf_name;
    int ttf_size;
    std::string bitmap_name;
    int r, g, b;

    // Constructor
    FontDescr(std::string name_, std::string ttf_name_, int ttf_size_, std::string bitmap_name_,
              int r_, int g_, int b_)
    : name(std::move(name_)),
      ttf_name(std::move(ttf_name_)),
      ttf_size(ttf_size_),
      bitmap_name(std::move(bitmap_name_)),
      r(r_),
      g(g_),
      b(b_) {}
};

class FontCache : public ecl::PtrCache<Font> {
public:
    Font *acquire(const std::string &name) override {
        Font *f = nullptr;
        if (m_fonts.has_key(name)) {
            const FontDescr &fd = m_fonts[name];
            f = load_ttf(fd.ttf_name, fd.ttf_size, fd.r, fd.g, fd.b);
            if (f == nullptr) {
                std::cerr << "Could not load .ttf file " << fd.ttf_name << "\n";
                f = load_bmf(fd.bitmap_name);
            }
        } else {
            f = load_bmf(name);
        }
        return f;
    }

    void define_font(const FontDescr &descr) {
        remove(descr.name);  // remove entry in cache (if any)
        if (m_fonts.has_key(descr.name))
            m_fonts[descr.name] = descr;
        else
            m_fonts.insert(descr.name, descr);
    }

    void clear() {
        PtrCache<Font>::clear();  // crazy double cache - TODO cleanup
        m_fonts.clear();
    }

private:
    Font *load_bmf(const std::string &name) {
        std::string png, bmf;
        if (app.resourceFS->findFile(std::string("fonts/") + name + ".png", png) &&
            app.resourceFS->findFile(std::string("fonts/") + name + ".bmf", bmf)) {
            return ecl::LoadBitmapFont(png.c_str(), bmf.c_str());
        }
        return nullptr;
    }

    Font *load_ttf(const std::string &name, int ptsize, int r, int g, int b) {
        std::string ttf;
        if (app.resourceFS->findFile(std::string("fonts/") + name, ttf))
            return ecl::LoadTTF(ttf.c_str(), ptsize, r, g, b);
        return nullptr;
    }

    // Variables
    ecl::Dict<FontDescr> m_fonts;
};

// ---------- Variables ----------

FontCache font_cache;
ImageCache image_cache;

}  // namespace

void DefineFont(const char *name, const char *ttf_name, int ttf_size, const char *bmf_name, int r,
                int g, int b) {
    font_cache.define_font(FontDescr(name, ttf_name, ttf_size, bmf_name, r, g, b));
}

ecl::Font *GetFont(const char *name) {
    return font_cache.get(name);
}

void ClearFontCache() {
    font_cache.clear();
}

ecl::Surface *LoadImage(const char *name) {
    std::string filename;
    if (app.resourceFS->findImageFile(std::string(name) + ".png", filename))
        return ecl::LoadImage(filename.c_str());
    return nullptr;
}

ecl::Surface *GetImage(const char *name, const char *ext) {
    std::string filename;
    if (app.resourceFS->findImageFile(std::string(name) + ext, filename))
        return image_cache.get(filename);
    return nullptr;
}

ecl::Surface *RegisterImage(const char *name, ecl::Surface *s) {
    image_cache.store(name, s);
    return s;
}

void ClearImageCache() {
    image_cache.clear();
}

}  // namespace enigma
