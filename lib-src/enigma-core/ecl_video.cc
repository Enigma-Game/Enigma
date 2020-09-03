/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
#include "ecl_video.hh"
#include "ecl_error.hh"
#include "ecl_sdl.hh"

#include "SDL_image.h"
#include "SDL_syswm.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"

#include <cassert>
#include <memory>
#include <cstdio>

using namespace ecl;

/* -------------------- Graphics primitives -------------------- */

void ecl::frame(const GC &gc, int x, int y, int w, int h) {
    hline(gc, x, y, w);
    hline(gc, x, y + h - 1, w);
    vline(gc, x, y, h);
    vline(gc, x + w - 1, y, h);
}

void ecl::line(const GC &gc, int x1, int y1, int x2, int y2) {
    gc.drawable->line(gc, x1, y1, x2, y2);
}

/* -------------------- Clipping helper routines -------------------- */
namespace {

inline bool NOCLIP(const GS &gs) {
    return gs.flags & GS_NOCLIP;
}

bool clip_hline(const GS &gs, int &x, int &y, int &w) {
    const Rect &cliprect = gs.cliprect;

    if (y < cliprect.y || y >= cliprect.y + cliprect.h)
        return false;

    int d = x - cliprect.x;
    if (d < 0) {
        w += d;
        x = cliprect.x;
    }
    d = (cliprect.x + cliprect.w) - (x + w);
    if (d < 0)
        w += d;
    return w > 0;
}

bool clip_vline(const GS &gs, int &x, int &y, int &h) {
    const Rect &cliprect = gs.cliprect;

    if (x < cliprect.x || x >= cliprect.x + cliprect.w)
        return false;

    int d = y - cliprect.y;
    if (d < 0) {
        h += d;
        y = cliprect.y;
    }
    d = cliprect.y + cliprect.h - (y + h);
    if (d < 0)
        h += d;
    return h > 0;
}

bool clip_blit(Rect cliprect, int &x, int &y, Rect &r) {
    cliprect.x += r.x - x;
    cliprect.y += r.y - y;
    cliprect.intersect(r);

    if (cliprect.w > 0 && cliprect.h > 0) {
        x += cliprect.x - r.x;
        y += cliprect.y - r.y;
        r = cliprect;
        return true;
    }
    return false;
}

bool clip_rect(const GS &gs, Rect &r) {
    r.intersect(gs.cliprect);
    return r.w > 0 && r.h > 0;
}

inline bool clip_pixel(const GS &gs, int x, int y) {
    return gs.cliprect.contains(x, y);
}

}  // namespace

/* -------------------- Drawable implementation -------------------- */

/* `Xlib.h' also defines a type named `Drawable' so we have to specify
   the namespace explicitly and cannot simply use a using-declaration. */

void ecl::Drawable::set_pixels(const GS &gs, int n, const int *x, const int *y) {
    const int *xp = x, *yp = y;
    for (int i = n; i; --i)
        set_pixel(gs, *xp++, *yp++);
}

void ecl::Drawable::hline(const GS &gs, int x, int y, int w) {
    for (int i = w; i; --i)
        set_pixel(gs, x++, y);
}

void ecl::Drawable::vline(const GS &gs, int x, int y, int h) {
    for (int i = h; i; --i)
        set_pixel(gs, x, y++);
}

void ecl::Drawable::box(const GS &gs, int x, int y, int w, int h) {
    for (int i = h; i; --i)
        hline(gs, x, y--, w);
}

void ecl::Drawable::line(const GS &, int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/) {
}

namespace {

/* ---------- Generic Surface implementation ---------- */
template <class PIXELT>
class TSurface : virtual public Surface {
public:
    TSurface(SDL_Surface *s = 0) : Surface(s) {}

    PIXELT *pixel_pointer(int x, int y) {
        return static_cast<PIXELT *>(Surface::pixel_pointer(x, y));
    }

    /* ---------- Drawable interface ---------- */

    PackedColor get_pixel(int x, int y) { return *pixel_pointer(x, y); }

    void set_pixel(const GS &gs, int x, int y) {
        if (NOCLIP(gs) || clip_pixel(gs, x, y)) {
            *pixel_pointer(x, y) = gs.pcolor;
        }
    }

    void set_pixels(const GS &gs, int n, const int *xlist, const int *ylist, Uint32 color) {
        const int *xp = xlist, *yp = ylist;
        if (NOCLIP(gs)) {
            for (int i = n; i > 0; --i) {
                int x = *xp++, y = *yp++;
                *pixel_pointer(x, y) = gs.pcolor;
            }
        } else {
            for (int i = n; i > 0; --i) {
                int x = *xp++, y = *yp++;
                if (clip_pixel(gs, x, y))
                    *pixel_pointer(x, y) = gs.pcolor;
            }
        }
    }

    void hline(const GS &gs, int x, int y, int w) {
        if (NOCLIP(gs) || clip_hline(gs, x, y, w)) {
            PIXELT *dst = pixel_pointer(x, y);
            for (; w > 0; --w)
                *dst++ = gs.pcolor;
        }
    }

    void vline(const GS &gs, int x, int y, int h) {
        if (NOCLIP(gs) || clip_vline(gs, x, y, h)) {
            PIXELT *dst = pixel_pointer(x, y);
            int offset = pitch() / bypp();
            for (; h > 0; --h) {
                *dst = gs.pcolor;
                dst += offset;
            }
        }
    }
};

typedef TSurface<Uint8> Surface8;
typedef TSurface<Uint16> Surface16;
typedef TSurface<Uint32> Surface32;

/* ---------- Surface24 ---------- */

class Surface24 : virtual public Surface {
public:
    Surface24(SDL_Surface *s = 0) : Surface(s) {}

    void set_pixel(const GS &gs, int x, int y) {
        if (NOCLIP(gs) || clip_pixel(gs, x, y)) {
            Uint8 *p = static_cast<Uint8 *>(pixel_pointer(x, y));
            SDL_GetRGB(gs.pcolor, m_surface->format, p, p + 1, p + 2);
        }
    }

    Uint32 get_pixel(int x, int y) {
        Uint8 *p = static_cast<Uint8 *>(pixel_pointer(x, y));
        return SDL_MapRGB(m_surface->format, p[0], p[1], p[2]);
    }
};

}  // namespace

/* -------------------- Surface -------------------- */

Surface::Surface(SDL_Surface *surface) : m_surface(surface) {
    assert(m_surface);
}

Surface::~Surface() {
    SDL_FreeSurface(m_surface);
}

void Surface::lock() {
    if (SDL_MUSTLOCK(m_surface))
        SDL_LockSurface(m_surface);
}

void Surface::unlock() {
    if (SDL_MUSTLOCK(m_surface))
        SDL_UnlockSurface(m_surface);
}

PackedColor Surface::map_color(int r, int g, int b) {
    return SDL_MapRGB(m_surface->format, r, g, b);
}

PackedColor Surface::map_color(int r, int g, int b, int a) {
    return SDL_MapRGBA(m_surface->format, r, g, b, a);
}

void Surface::box(const GS &gs, int x, int y, int w, int h) {
    Rect r(x, y, w, h);
    if (NOCLIP(gs) || clip_rect(gs, r)) {
        SDL_Rect dr;
        sdl::copy_rect(dr, r);
        SDL_FillRect(m_surface, &dr, gs.pcolor);
    }
}

void Surface::line(const GS &gs, int x1, int y1, int x2, int y2) {
    SDL_Rect s;
    sdl::copy_rect(s, gs.cliprect);
    SDL_SetClipRect(m_surface, &s);

    Uint8 r, g, b, a;
    SDL_GetRGBA(gs.pcolor, m_surface->format, &r, &g, &b, &a);

    if (has_flags(gs.flags, GS_ANTIALIAS))
        aalineRGBA(m_surface, x1, y1, x2, y2, r, g, b, a);
    else
        lineRGBA(m_surface, x1, y1, x2, y2, r, g, b, a);

    SDL_SetClipRect(m_surface, 0);
}

void Surface::blit(const GS &gs, int x, int y, const Surface *s, const Rect &r_) {
    Rect r(r_);
    if (NOCLIP(gs) || clip_blit(gs.cliprect, x, y, r)) {
        SDL_Rect r1;
        SDL_Rect r2;
        sdl::copy_rect(r1, r);
        r2.x = x;
        r2.y = y;
        SDL_BlitSurface(s->m_surface, &r1, m_surface, &r2);
    }
}

void Surface::blit(const GS &gs, int x, int y, const Surface *src) {
    if (src != 0) {
        blit(gs, x, y, src, src->size());
    }
}

void Surface::set_color_key(int r, int g, int b) {
    Uint32 color = map_color(r, g, b);
    SDL_SetColorKey(get_surface(), SDL_TRUE, color);
    SDL_SetSurfaceRLE(get_surface(), SDL_TRUE);
}

void Surface::set_alpha(int a) {
    SDL_SetSurfaceAlphaMod(get_surface(), a);
}

Surface *Surface::zoom(int w, int h) {
    SDL_Surface *s_new;
    s_new = zoomSurface(get_surface(), (double)w / width(), (double)h / height(), true);
    return Surface::make_surface(s_new);
}

Surface *Surface::make_surface(SDL_Surface *sdls) {
    assert(sdls);
    switch (sdls->format->BitsPerPixel) {
    case 8: return new Surface8(sdls); break;
    case 16: return new Surface16(sdls); break;
    case 32: return new Surface32(sdls); break;
    case 24: return new Surface24(sdls); break;
    default:
        fprintf(stderr, "Invalid bit depth in surface.\n");
        return 0;  // throw XVideo("Invalid bit depth in surface.");
    }
}

/* -------------------- Screen -------------------- */

/* `Xlib.h' also defines a type named `Screen' so we have to specify
   the namespace explicitly and cannot simply use a using-declaration. */

ecl::Screen *ecl::Screen::m_instance = 0;

ecl::Screen *ecl::Screen::get_instance() {
    return m_instance;
}

ecl::Screen::Screen(SDL_Window *window)
: m_window(window),
  //m_surface(Surface::make_surface(SDL_GetWindowSurface(window))),
  //m_sdlsurface(SDL_GetWindowSurface(window)),
  m_surface(Surface::make_surface(SDL_CreateRGBSurface(0, 800, 600, 32, 0xff0000, 0xff00, 0xff, 0xff000000))),
  m_sdlsurface(m_surface->get_surface()),
  update_all_p(false) {
    assert(m_window);
    assert(m_surface);
    assert(m_instance == 0);
    m_instance = this;
}

ecl::Screen::~Screen() {
    m_instance = 0;
}

void ecl::Screen::update_all() {
    update_all_p = true;
}

void ecl::Screen::update_rect(const Rect &r) {
    if (m_dirtyrects.size() < 200)
        m_dirtyrects.push_back(r);
    else
        update_all();
}

void ecl::Screen::flush_updates() {
    if (update_all_p) {
        SDL_BlitScaled(m_sdlsurface, NULL, SDL_GetWindowSurface(m_window), NULL);
        SDL_UpdateWindowSurface(m_window);
        update_all_p = false;
    } else if (!m_dirtyrects.empty()) {
        m_dirtyrects.intersect(size());

        std::vector<SDL_Rect> rects(m_dirtyrects.size());
        RectList::iterator j = m_dirtyrects.begin();
        for (unsigned i = 0; i < rects.size(); ++i, ++j)
        {
            SDL_Rect sdlrect;
            sdl::copy_rect(sdlrect, *j);
            int nx = (int)((double) (j->x * window_size().w) / size().w + 0.5);
            int ny = (int)((double) (j->y * window_size().h) / size().h + 0.5);
            int nw = (int)((double) (j->w * window_size().w) / size().w + 0.5);
            int nh = (int)((double) (j->h * window_size().h) / size().h + 0.5);
            ecl::Rect scaledRect = Rect(nx, ny, nw, nh);
            sdl::copy_rect(rects[i], scaledRect);
            SDL_BlitScaled(m_sdlsurface, &sdlrect, SDL_GetWindowSurface(m_window), &rects[i]);
        }
        SDL_UpdateWindowSurfaceRects(m_window, &rects[0], rects.size());
    }
    m_dirtyrects.clear();
}

Rect ecl::Screen::size() const {
    return Rect(0, 0, width(), height());
}

int ecl::Screen::width() const {
    return m_sdlsurface->w;
}

int ecl::Screen::height() const {
    return m_sdlsurface->h;
}

Rect ecl::Screen::window_size() const {
    return Rect(0, 0, window_width(), window_height());
}

int ecl::Screen::window_width() const {
    return SDL_GetWindowSurface(m_window)->w;
}

int ecl::Screen::window_height() const {
    return SDL_GetWindowSurface(m_window)->h;
}

/* -------------------- Functions -------------------- */

Surface *ecl::Duplicate(const Surface *s) {
    assert(s);
    SDL_Surface *sdls = s->get_surface();
    SDL_Surface *copy = SDL_ConvertSurface(sdls, sdls->format, sdls->flags);
    return Surface::make_surface(copy);
}

void ecl::SavePNG(const ecl::Surface *s, const std::string &filename) {
    IMG_SavePNG(s->get_surface(), filename.c_str());
}

void ecl::TintRect(Surface *s, Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    std::unique_ptr<Surface> copy(Grab(s, rect));
    if (copy.get()) {
        copy->set_alpha(a);

        GC gc(s);
        set_color(gc, r, g, b);
        box(gc, rect);
        blit(gc, rect.x, rect.y, copy.get());
    }
}

static SDL_Surface *CropSurface(SDL_Surface *surface, SDL_Rect rect, SDL_PixelFormat *format,
                                Uint32 flags) {
    assert(format->palette == NULL);

    SDL_Surface *cropped =
        SDL_CreateRGBSurface(flags, rect.w, rect.h, format->BitsPerPixel, format->Rmask,
                             format->Gmask, format->Bmask, format->Amask);
    if (cropped == NULL)
        return NULL;

    SDL_Rect bounds;
    bounds.x = 0;
    bounds.y = 0;
    bounds.w = rect.w;
    bounds.h = rect.h;
    SDL_LowerBlit(surface, &rect, cropped, &bounds);

    return cropped;
}

Surface *ecl::Grab(const Surface *s, Rect &r) {
    if (s == NULL)
        return NULL;

    int x = 0;
    int y = 0;
    clip_blit(s->size(), x, y, r);

    SDL_Rect rect;
    sdl::copy_rect(rect, r);
    SDL_Surface *sdls = s->get_surface();
    SDL_Surface *copy = CropSurface(sdls, rect, sdls->format, sdls->flags);
    return Surface::make_surface(copy);
}

// LoadImage is already defined as a macro in some Windows header file
#undef LoadImage

Surface *ecl::LoadImage(const char *filename) {
    return Surface::make_surface(IMG_Load(filename));
}

Surface *ecl::LoadImage(SDL_RWops *src, int freesrc) {
    return Surface::make_surface(IMG_Load_RW(src, freesrc));
}

Surface *ecl::MakeSurface(int w, int h) {
    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, w, h, 32, 0xff0000, 0xff00, 0xff, 0xff000000);
    if (surface == 0)
        return 0;
    return Surface::make_surface(surface);
}

Surface *ecl::MakeSurface(void *data, int w, int h, int bipp, int pitch, const RGBA_Mask &mask) {
    SDL_Surface *surface =
        SDL_CreateRGBSurfaceFrom(data, w, h, bipp, pitch, mask.r, mask.g, mask.b, mask.a);
    if (surface == 0)
        return 0;
    return Surface::make_surface(surface);
}

Surface *ecl::Resample(Surface *s, Rect rect, int neww, int newh) {
    SDL_Surface *sdls = SDL_CreateRGBSurface(SDL_SWSURFACE, rect.w, rect.h, 32, 0, 0, 0, 0);
    SDL_Rect r;
    sdl::copy_rect(r, rect);
    SDL_BlitSurface(s->get_surface(), &r, sdls, 0);

    SDL_Surface *s_new;
    s_new = zoomSurface(sdls, (double)neww / rect.w, (double)newh / rect.h, true);

    SDL_FreeSurface(sdls);
    return Surface::make_surface(s_new);
}
