/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
 * Copyright (C) 2006,2007      Ronald Lamprecht
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
#include "ecl_font.hh"

#include "ecl_geom.hh"
#include "ecl_utf.hh"
#include "ecl_video.hh"
#include <SDL3_ttf/SDL_ttf.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace ecl {

std::string normalizeSpaces(const std::string& text) {
    std::string result;
    int state = 0;
    // TODO: This might fail for general UTF-8 encoding!
    for (char c : text) {
        if(std::isspace(c)) {
            state *= state;
        } else {
            result += ((state == 1) ? " " : "");
            result += c;
            state = -1;
        }
    }
    return result;
}

std::string::size_type breakString(
        Font* font, const std::string& text, const std::string& breakChars, int targetWidth) {
    if (font->get_width(text) <= targetWidth)
        return text.size();  // the complete string fits into a line

    bool breakFound = false;
    std::string::size_type pos = 0;
    while (true) {
        std::string::size_type nextPos = text.find_first_of(breakChars, pos);

        if (nextPos == std::string::npos)
            // no more line breaks
                return breakFound ? pos : text.size();

        if (font->get_width(text.substr(0, nextPos + 1)) > targetWidth)
            // now the string is too long
                return breakFound ? pos : nextPos + 1;

        pos = nextPos + 1;
        breakFound = true;
    }
}

std::vector<std::string> breakToLines(
        Font* font, const std::string& text, const std::string& breakChars, int targetWidth) {
    std::vector<std::string> lines;
    if (text.empty())
        return lines;
    std::string::size_type breakPos = breakString(font, text, breakChars, targetWidth);
    lines = breakToLines(font, text.substr(breakPos), breakChars, targetWidth);
    if (breakPos > 0 && text.substr(breakPos - 1, 1) == " ")
        breakPos -= 1;
    lines.insert(lines.begin(), text.substr(0, breakPos).c_str());
    return lines;
}

//
// Bitmap fonts
//

namespace {

    class BitmapFont : public Font {
        std::vector<Rect> charRects;
        std::vector<int> advance;
        std::unique_ptr<Surface> surface;

    public:
        BitmapFont(std::unique_ptr<Surface> s, const char* descr);
        ~BitmapFont() override = default;

        int get_lineskip() override { return surface->height() + 3; }
        int get_width(char c) override;
        int get_width(const std::string& text, Font* altFont = nullptr) override;
        int get_height() override;

        std::unique_ptr<Surface> render(
                const std::string& text, Font* altFont = nullptr, int maxWidth = -1) override;
        void render(const GC& gc, int x, int y, const std::string& text, Font* altFont = nullptr,
                int maxWidth = -1) override;
    };

}  // namespace

BitmapFont::BitmapFont(std::unique_ptr<Surface> s, const char* descr)
    : charRects(256), advance(256), surface(std::move(s)) {
    // Read and interpret the font description file.
    // expected line format:
    // charno xpos width xadvance

    FILE *fp = fopen(descr, "rt");
    if (!fp)
        return;  // throw InvalidFont();

    int c;
    int x = 0, w = 0, adv = 0;
    while (fscanf(fp, "%d %d %d %d\n", &c, &x, &w, &adv) != EOF) {
        charRects[c].x = x;
        charRects[c].w = w;
        charRects[c].y = 0;
        charRects[c].h = surface->height();
        advance[c] = adv;
        if (adv == 0)
            std::cout << "BitFont 0\n";
    }
}

int BitmapFont::get_width(char c) {
    return advance[int(c)];
}

int BitmapFont::get_width(const std::string &text, Font *altFont) {
    int width = 0;
    const char *cstr = text.c_str();
    for (const char *p = cstr; *p; ++p) {
        // utf-8 char handling
        int len = utf8NextCharSize(p);  // num of bytes that represents one real character
        if (len == 0) {
            // a spurious follow-up byte
            continue;
        }

        if (len > 1 || advance[int(*p)] == 0) {
            if (altFont != nullptr) {
                std::string utf8char(p, len);
                width += altFont->get_width(utf8char);
            }
            p += len - 1;
        } else {
            width += get_width(*p);
        }
    }
    return width;
}

int BitmapFont::get_height() {
    return surface->height();
}

std::unique_ptr<Surface> BitmapFont::render(const std::string& text, Font *altFont, int maxWidth) {
    std::unique_ptr<Surface> s = MakeSurface(get_width(text, altFont), get_height());
    s->set_color_key(0, 0, 0);
    int width = 0;
    int x = 0;
    const char *cstr = text.c_str();
    for (const char *p = cstr; *p; ++p) {
        // utf-8 char handling
        int len = utf8NextCharSize(p);  // num of bytes that represents one real character
        if (len == 0) {
            // a spurious follow-up byte
            continue;
        }

        if (len > 1 || advance[int(*p)] == 0) {
            if (altFont != nullptr) {
                std::string utf8char(p, len);
                int charWidth = altFont->get_width(utf8char);
                width += charWidth;
                if (maxWidth <= 0 || width < maxWidth) {
                    altFont->render(GC(s.get()), x, 1, utf8char);
                    x += altFont->get_width(utf8char);
                }
            }
            p += len - 1;
        } else {
            int charWidth = get_width(*p);
            width += charWidth;
            if (maxWidth <= 0 || width <= maxWidth) {
                blit(GC(s.get()), x, 0, surface.get(), charRects[int(*p)]);
                x += charWidth;
            }
        }
    }
    return s;
}

void BitmapFont::render(
        const GC& gc, int x, int y, const std::string& text, Font* altFont, int maxWidth) {
    std::unique_ptr<Surface> s = render(text, altFont, maxWidth);
    blit(gc, x, y, s.get());
}

std::unique_ptr<Font> LoadBitmapFont(const char *imgname, const char *descrname) {
    if (std::unique_ptr<Surface> s = LoadImage(imgname))
        return std::make_unique<BitmapFont>(std::move(s), descrname);
    return nullptr;
}

//
// TrueType fonts
//

namespace {

    class TrueTypeFont : public Font {
        // Variables
        TTF_Font *font;
        SDL_Color fgcolor;

    public:
        TrueTypeFont(TTF_Font *font_, int r, int g, int b);

        // Inhibit copying
        TrueTypeFont(const TrueTypeFont &) = delete;
        TrueTypeFont &operator=(const TrueTypeFont &) = delete;

        ~TrueTypeFont() override;

        // Font interface
        int get_lineskip() override;
        int get_height() override;
        int get_width(char c) override;
        int get_width(const std::string& text, Font *altFont = nullptr) override;

        std::unique_ptr<Surface> render(
                const std::string& text, Font *altFont = nullptr, int maxWidth = -1);
        void render(const GC &gc, int x, int y, const std::string& text, Font *altFont = nullptr, int maxWidth = -1) override;

    private:
        SDL_PixelFormat pixel_format;
    };

}  // namespace

TrueTypeFont::TrueTypeFont(TTF_Font *font_, int r, int g, int b) : font(font_) {
    fgcolor.r = r;
    fgcolor.g = g;
    fgcolor.b = b;
    pixel_format = SDL_PIXELFORMAT_RGB888;
}

TrueTypeFont::~TrueTypeFont() {
    TTF_CloseFont(font);
}

int TrueTypeFont::get_lineskip() {
    return TTF_GetFontLineSkip(font);
}
int TrueTypeFont::get_height() {
    return TTF_GetFontHeight(font);
}

int TrueTypeFont::get_width(char c) {
    int minx, maxx, miny, maxy, advance;
    TTF_GetGlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &advance);
    return advance;
}

std::unique_ptr<Surface> TrueTypeFont::render(
        const std::string& text, Font *altFont, int maxWidth) {
    // Note: altFont is only used by BitmapFont.
    // TODO: Implement maxWidth. (Not actually used right now.)
    SDL_Color bgcolor = {0, 0, 0, 0};
    SDL_Surface *si = TTF_RenderText_Shaded(font, text.c_str(), 0, fgcolor, bgcolor);
    if (si) {
        SDL_Surface *s = SDL_ConvertSurface(si, pixel_format);
        SDL_DestroySurface(si);
        SDL_SetSurfaceColorKey(s, true, 0);
        return Surface::make_surface(s);
    }
    return MakeSurface(0, get_height());
}

void TrueTypeFont::render(const GC &gc, int x, int y, const std::string& text, Font *altFont, int maxWidth) {
    std::unique_ptr<Surface> surface(render(text, altFont, maxWidth));
    if (surface)
        blit(gc, x, y, surface.get());
}

int TrueTypeFont::get_width(const std::string& text, Font *altFont) {
    int width, height;
    TTF_GetStringSize(font, text.c_str(), 0, &width, &height);
    return width;
}

std::unique_ptr<Font> LoadTTF(const char *filename, int ptSize, int r, int g, int b) {
    if (!TTF_WasInit() && !TTF_Init()) {
        fprintf(stderr, "Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
        exit(1);
    }
    TTF_Font *font = TTF_OpenFont(filename, ptSize);
    return font ? std::make_unique<TrueTypeFont>(font, r, g, b) : nullptr;
}

} // namespace ecl