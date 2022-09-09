/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 * Copyright (C) 2022 Andreas Lochmann
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
#include "ecl_util.hh"

#include "SDL_image.h"
#include "SDL_syswm.h"
#include "SDL_gfxPrimitives.h"

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
    TSurface(SDL_Surface *s = 0, bool _has_alpha = true) : Surface(s, _has_alpha) {}

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
    Surface24(SDL_Surface *s = 0, bool _has_alpha = true) : Surface(s, _has_alpha) {}

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

Surface::Surface(SDL_Surface *surface, bool _has_alpha) {
    // If you change the pixel format, remember to replace Surface32
    // for another SurfaceXY in Surface::make_surface.
    has_alpha = _has_alpha;
    if (has_alpha) {
        pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
    } else {
        pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    }
    assert(surface);
    m_surface = SDL_ConvertSurface(surface, pixel_format, 0);
    SDL_FreeSurface(surface);
    assert(m_surface);
}


Surface::~Surface() {
    SDL_FreeSurface(m_surface);
    SDL_FreeFormat(pixel_format);
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
    if (has_alpha)
        SDL_SetSurfaceAlphaMod(get_surface(), a);
    else
        fprintf(stderr, "Trying to set alpha channel on a surface without alpha channel.\n");
}

void Surface::set_brightness(int a) {
    SDL_SetSurfaceColorMod(get_surface(), a, a, a);
}

Surface *Surface::zoom(int w, int h) {
    Surface *s_new = MakeSurface(w, h);
    BlitScaled(get_surface(), NULL, s_new->get_surface(), NULL, SC_bytewise);
    return s_new;
}

Surface *Surface::make_surface(SDL_Surface *sdls, bool _has_alpha) {
    if(!sdls) {
        fprintf(stderr, "Could not create SDL surface, error message: %s\n", SDL_GetError());
        assert(false);
    }
    // The constructor will change the surface's pixel format to a 32-bit one.
    // Note: We may use _has_alpha to choose a different bit depth. I tested
    // Surface32 vs. Surface24 and found that Surface32 is about 0.3% faster
    // on my system. -- AL
    return new Surface32(sdls, _has_alpha);
}


/* -------------------- Screen -------------------- */

/* `Xlib.h' also defines a type named `Screen' so we have to specify
   the namespace explicitly and cannot simply use a using-declaration. */

ecl::Screen *ecl::Screen::m_instance = 0;

ecl::Screen *ecl::Screen::get_instance() {
    return m_instance;
}

ecl::Screen::Screen(SDL_Window *window, int surface_w, int surface_h)
: m_window(window),
  m_surface(Surface::make_surface(
      SDL_CreateRGBSurface(0, surface_w, surface_h, 32, 0xff0000, 0xff00, 0xff, 0xff000000),
      NO_ALPHA)),
  m_sdlsurface(m_surface->get_surface()),
  update_all_p(false) {
    assert(m_window);
    assert(m_surface);
    assert(m_instance == 0);
    m_instance = this;
    m_scaler = new ecl::Scaler(m_surface->get_surface(), SDL_GetWindowSurface(m_window), SC_bytewise);
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
        m_scaler->blit_scaled(m_sdlsurface, NULL, SDL_GetWindowSurface(m_window), NULL);
        SDL_UpdateWindowSurface(m_window);
        update_all_p = false;
    } else if (!m_dirtyrects.empty()) {
        m_dirtyrects.intersect(size());
        std::vector<SDL_Rect> rects(m_dirtyrects.size());
        RectList::iterator j = m_dirtyrects.begin();
        SDL_Surface* window = SDL_GetWindowSurface(m_window);
        for (unsigned i = 0; i < rects.size(); ++i, ++j)
        {
            SDL_Rect srcrect;
            sdl::copy_rect(srcrect, *j);
            int nx = (int)((double) (j->x * window_size().w) / size().w - 0.5);
            int ny = (int)((double) (j->y * window_size().h) / size().h - 0.5);
            int nw = (int)((double) (j->w * window_size().w) / size().w + 3.0);
            int nh = (int)((double) (j->h * window_size().h) / size().h + 3.0);
            nx = Clamp(nx, 0, window->w);
            ny = Clamp(ny, 0, window->h);
            nw = Clamp(nw, 0, window->w - nx);
            nh = Clamp(nh, 0, window->h - ny);
            ecl::Rect scaledRect = Rect(nx, ny, nw, nh);
            sdl::copy_rect(rects[i], scaledRect);
            m_scaler->blit_scaled(m_sdlsurface, &srcrect, window, &rects[i]);
        }
        SDL_UpdateWindowSurfaceRects(m_window, &rects[0], rects.size());
    }
    m_dirtyrects.clear();
}

void ecl::Screen::reinitScaler() {
    m_scaler->precalculate(m_surface->get_surface(), SDL_GetWindowSurface(m_window));
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

/* -------------------- Scaler -------------------- */

// Code originally by Andreas Schiffler (SDL2_rotozoom), heavily adapted to our use case.

Scaler::Scaler(SDL_Surface* _src, SDL_Surface* _dst, ScalerMode _mode) {
    assert(_src);
    assert(_dst);
    mode = _mode;
    sax = NULL;
    say = NULL;
    precalculate(_src, _dst);
}

Scaler::~Scaler() {
    free(sax);
    free(say);
}

void Scaler::precalculate(SDL_Surface* src, SDL_Surface* dst) {
    // Free prior allocations.
    free(sax);
    free(say);
    sax = NULL;
    say = NULL;

    // TODO: Change mode if src/dst are not 32bit depth.

    // Allocate memory for row/column increments
    if ((sax = (int *) malloc((dst->w + 1) * sizeof(Uint32))) == NULL) {
        fprintf(stderr, "ecl_video::BlitScaled: Could not allocate memory for row/column increments.\n");
        sax = NULL;
        return;
    }
    if ((say = (int *) malloc((dst->h + 1) * sizeof(Uint32))) == NULL) {
        fprintf(stderr, "ecl_video::BlitScaled: Could not allocate memory for row/column increments.\n");
        free(sax);
        sax = NULL;
        say = NULL;
        return;
    }

    // Precalculate row increments
    spixelw = (src->w - 1);
    spixelh = (src->h - 1);
    sx = (int) (65536.0 * (float) spixelw / (float) (dst->w - 1));
    sy = (int) (65536.0 * (float) spixelh / (float) (dst->h - 1));

    // Maximum scaled source size
    ssx = (src->w << 16) - 1;
    ssy = (src->h << 16) - 1;

    // Precalculate horizontal row increments
    csx = 0;
    csax = sax;
    for (x = 0; x <= dst->w; x++) {
        *csax = csx;
        csax++;
        csx += sx;
        // Guard from overflows
        if (csx > ssx)
            csx = ssx;
    }

    // Precalculate vertical row increments
    csy = 0;
    csay = say;
    for (y = 0; y <= dst->h; y++) {
        *csay = csy;
        csay++;
        csy += sy;
        // Guard from overflows
        if (csy > ssy)
            csy = ssy;
    }

    dgap = dst->pitch - dst->w * 4;
    spixelgap = src->pitch/4;
}

void Scaler::blit_scaled(SDL_Surface* src, SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect) {
    if (mode == SC_SDL) {
        SDL_BlitScaled(src, srcrect, dst, dstrect);
        return;
    }

    // TODO: Care for srcrect!

    typedef struct tColorRGBA {
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    } tColorRGBA;

    int dstrx, dstry, dstrw, dstrh;
    tColorRGBA *c00, *c01, *c10, *c11, *sp, *csp, *dp;

    if ((sax == NULL) || (say == NULL)) {
        fprintf(stderr, "ecl_video::BlitScaled: Precalculations missing. (Error allocating memory?)\n");
        return;
    }

    if (dstrect) {
        dstrx = Clamp(dstrect->x, 0, dst->w);
        dstry = Clamp(dstrect->y, 0, dst->h);
        dstrw = Clamp(dstrect->w, 0, dst->w - dstrx);
        dstrh = Clamp(dstrect->h, 0, dst->h - dstry);
    } else {
        dstrx = 0;
        dstry = 0;
        dstrw = dst->w;
        dstrh = dst->h;
    }

    sp = (tColorRGBA *) src->pixels;
    dp = (tColorRGBA *) dst->pixels;

    // Advance source pointer y to the beginning of the first line to copy
    csay = say + dstry;
    sp += ((*csay >> 16) - (*say >> 16)) * spixelgap;
    // Advance destination pointer to the beginning of the first line to copy
    dp = (tColorRGBA *) ((Uint8 *) (dp + dst->w * dstry) + dgap * dstry);
    for (y = dstry; y < dstry + dstrh; y++) {
        csp = sp;
        // Advance source pointer x
        csax = sax + dstrx;
        sp += (*csax >> 16) - (*sax >> 16);
        // Advance destination pointer to first position-to-copy in this row
        dp += dstrx;
        for (x = dstrx; x < dstrx + dstrw; x++) {
            // Setup color source pointers
            ex = (*csax & 0xffff);
            ey = (*csay & 0xffff);
            cx = (*csax >> 16);
            cy = (*csay >> 16);
            sstepx = cx < spixelw;
            sstepy = cy < spixelh;
            c00 = sp;
            c01 = sp;
            c10 = sp;
            if (sstepy)
                c10 += spixelgap;
            c11 = c10;
            if (sstepx) {
                c01++;
                c11++;
            }

            // Draw and interpolate colors
            t1 = ((((c01->r - c00->r) * ex) >> 16) + c00->r) & 0xff;
            t2 = ((((c11->r - c10->r) * ex) >> 16) + c10->r) & 0xff;
            dp->r = (((t2 - t1) * ey) >> 16) + t1;
            t1 = ((((c01->g - c00->g) * ex) >> 16) + c00->g) & 0xff;
            t2 = ((((c11->g - c10->g) * ex) >> 16) + c10->g) & 0xff;
            dp->g = (((t2 - t1) * ey) >> 16) + t1;
            t1 = ((((c01->b - c00->b) * ex) >> 16) + c00->b) & 0xff;
            t2 = ((((c11->b - c10->b) * ex) >> 16) + c10->b) & 0xff;
            dp->b = (((t2 - t1) * ey) >> 16) + t1;
            t1 = ((((c01->a - c00->a) * ex) >> 16) + c00->a) & 0xff;
            t2 = ((((c11->a - c10->a) * ex) >> 16) + c10->a) & 0xff;
            dp->a = (((t2 - t1) * ey) >> 16) + t1;

            // Advance source pointer x
            salast = csax;
            csax++;
            sstep = (*csax >> 16) - (*salast >> 16);
            sp += sstep;

            // Advance destination pointer x
            dp++;
        }
        // Advance source pointer x
        salast = csax;
        csax += dst->w + dstrw - dstrx;
        sp += (*csax >> 16) - (*salast >> 16);
        // Advance source pointer y
        salast = csay;
        csay++;
        sstep = (*csay >> 16) - (*salast >> 16);
        sstep *= spixelgap;
        sp = csp + sstep;

        // Advance destination pointer to beginning of next row
        dp = (tColorRGBA *) ((Uint8 *) (dp + dst->w - (dstrx+dstrw)) + dgap);
    }
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

void ecl::BlitScaled(SDL_Surface* src, SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect, ScalerMode mode) {
    Scaler* scaler = new Scaler(src, dst, mode);
    scaler->blit_scaled(src, srcrect, dst, dstrect);
}
