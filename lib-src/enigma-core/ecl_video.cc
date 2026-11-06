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

#include <SDL3_image/SDL_image.h>
#include "SDL_gfxPrimitives.h"

#include <cassert>
#include <memory>
#include <cstdio>

namespace ecl {
/* -------------------- Graphics primitives -------------------- */

void frame(const GC &gc, int x, int y, int w, int h) {
    hline(gc, x, y, w);
    hline(gc, x, y + h - 1, w);
    vline(gc, x, y, h);
    vline(gc, x + w - 1, y, h);
}

void line(const GC &gc, int x1, int y1, int x2, int y2) {
    gc.drawable->line(gc, x1, y1, x2, y2);
}

/* -------------------- Clipping helper routines -------------------- */
namespace {

    inline bool NOCLIP(const GS &gs) {
        return gs.flags & GS_NOCLIP;
    }

    bool clip_hline(const GS &gs, int &x, int y, int &w) {
        const Rect &cliprect = gs.clipRect;

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

    bool clip_vline(const GS &gs, int x, int &y, int &h) {
        const Rect &cliprect = gs.clipRect;

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
        r.intersect(gs.clipRect);
        return r.w > 0 && r.h > 0;
    }

    inline bool clip_pixel(const GS &gs, int x, int y) {
        return gs.clipRect.contains(x, y);
    }

}  // namespace

/* -------------------- Drawable implementation -------------------- */

/* `Xlib.h' also defines a type named `Drawable' so we have to specify
   the namespace explicitly and cannot simply use a using-declaration. */

void Drawable::set_pixels(const GS &gs, int n, const int *x, const int *y) {
    const int *xp = x, *yp = y;
    for (int i = n; i; --i)
        set_pixel(gs, *xp++, *yp++);
}

void Drawable::hline(const GS &gs, int x, int y, int w) {
    for (int i = w; i; --i)
        set_pixel(gs, x++, y);
}

void Drawable::vline(const GS &gs, int x, int y, int h) {
    for (int i = h; i; --i)
        set_pixel(gs, x, y++);
}

void Drawable::box(const GS &gs, int x, int y, int w, int h) {
    for (int i = h; i; --i)
        hline(gs, x, y--, w);
}

void Drawable::line(const GS &, int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/) {
}

namespace {

    /* ---------- Generic Surface implementation ---------- */
    template <class PIXELT>
    class TSurface : virtual public Surface {
    public:
        explicit TSurface(SDL_Surface* s = nullptr, bool _has_alpha = true)
            : Surface(s, _has_alpha) {}

        PIXELT *pixel_pointer(int x, int y) {
            return static_cast<PIXELT *>(Surface::pixel_pointer(x, y));
        }

        /* ---------- Drawable interface ---------- */

        PackedColor get_pixel(int x, int y) override { return *pixel_pointer(x, y); }

        void set_pixel(const GS &gs, int x, int y) override {
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

        void hline(const GS &gs, int x, int y, int w) override {
            if (NOCLIP(gs) || clip_hline(gs, x, y, w)) {
                PIXELT *dst = pixel_pointer(x, y);
                for (; w > 0; --w)
                    *dst++ = gs.pcolor;
            }
        }

        void vline(const GS &gs, int x, int y, int h) override {
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
        explicit Surface24(SDL_Surface* s = nullptr, bool _has_alpha = true)
            : Surface(s, _has_alpha) {}

        void set_pixel(const GS &gs, int x, int y) override {
            if (NOCLIP(gs) || clip_pixel(gs, x, y)) {
                Uint8 *p = static_cast<Uint8 *>(pixel_pointer(x, y));
                SDL_GetRGB(gs.pcolor, SDL_GetPixelFormatDetails(m_surface->format), nullptr, p, p + 1, p + 2);
            }
        }

        Uint32 get_pixel(int x, int y) override {
            Uint8 *p = static_cast<Uint8 *>(pixel_pointer(x, y));
            return SDL_MapRGB(SDL_GetPixelFormatDetails(m_surface->format), nullptr, p[0], p[1], p[2]);
        }
    };

}  // namespace

/* -------------------- Surface -------------------- */

Surface::Surface(SDL_Surface *surface, bool _has_alpha) {
    static std::string lastErrorMessage;
    hasAlpha = _has_alpha;
    pixel_format = hasAlpha ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_RGB888;
    if (!surface)
        throw XGeneric("Error: Tried to convert null surface.");
    m_surface = SDL_ConvertSurface(surface, pixel_format);
    SDL_DestroySurface(surface);
    if (!m_surface) {
        if (lastErrorMessage.empty()) {
            lastErrorMessage = std::string("SDL_ConvertSurface failed: ") + SDL_GetError();
            throw XSDLError(lastErrorMessage);
        } else {
            throw XGeneric(lastErrorMessage);
        }
    }
}


Surface::~Surface() {
    SDL_DestroySurface(m_surface);
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
    return SDL_MapRGB(SDL_GetPixelFormatDetails(m_surface->format), nullptr, r, g, b);
}

PackedColor Surface::map_color(int r, int g, int b, int a) {
    return SDL_MapRGBA(SDL_GetPixelFormatDetails(m_surface->format), nullptr, r, g, b, a);
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
    sdl::copy_rect(s, gs.clipRect);
    SDL_SetClipRect(m_surface, &s);

    Uint8 r, g, b, a;
    SDL_GetRGBA(gs.pcolor, SDL_GetPixelFormatDetails(m_surface->format), nullptr, &r, &g, &b, &a);

    if (has_flags(gs.flags, GS_ANTIALIAS))
        aalineRGBA(m_surface, x1, y1, x2, y2, r, g, b, a);
    else
        lineRGBA(m_surface, x1, y1, x2, y2, r, g, b, a);

    SDL_SetClipRect(m_surface, nullptr);
}

void Surface::blit(const GS &gs, int x, int y, const Surface *s, const Rect &r_) {
    Rect r(r_);
    if (NOCLIP(gs) || clip_blit(gs.clipRect, x, y, r)) {
        SDL_Rect r1;
        SDL_Rect r2;
        sdl::copy_rect(r1, r);
        r2.x = x;
        r2.y = y;
        SDL_BlitSurface(s->m_surface, &r1, m_surface, &r2);
    }
}

void Surface::blit(const GS &gs, int x, int y, const Surface *surface) {
    if (surface != nullptr) {
        blit(gs, x, y, surface, surface->size());
    }
}

void Surface::set_color_key(int r, int g, int b) {
    Uint32 color = map_color(r, g, b);
    SDL_SetSurfaceColorKey(get_surface(), true, color);
    SDL_SetSurfaceRLE(get_surface(), true);
}

void Surface::set_alpha(int a) {
    if (hasAlpha)
        SDL_SetSurfaceAlphaMod(get_surface(), a);
    else
        fprintf(stderr, "Trying to set alpha channel on a surface without alpha channel.\n");
}

void Surface::set_brightness(int a) {
    SDL_SetSurfaceColorMod(get_surface(), a, a, a);
}

std::unique_ptr<Surface> Surface::zoom(int w, int h) {
    std::unique_ptr<Surface> s_new = MakeSurface(w, h);
    BlitScaled(get_surface(), nullptr, s_new->get_surface(), nullptr);
    return s_new;
}

std::unique_ptr<Surface> Surface::make_surface(SDL_Surface *sdlSurface, bool _has_alpha) {
    if (!sdlSurface) {
        fprintf(stderr, "Could not create SDL surface, error message: %s\n", SDL_GetError());
        assert(false);
    }
    // The constructor will change the surface's pixel format to a 32-bit one.
    // Note: We may use _has_alpha to choose a different bit depth. I tested
    // Surface32 vs. Surface24 and found that Surface32 is about 0.3% faster
    // on my system. -- AL
    return std::make_unique<Surface32>(sdlSurface, _has_alpha);
}


/* -------------------- Screen -------------------- */

/* `Xlib.h' also defines a type named `Screen' so we have to specify
   the namespace explicitly and cannot simply use a using-declaration. */

Screen *Screen::m_instance = nullptr;

Screen *Screen::get_instance() {
    return m_instance;
}

Screen::Screen(SDL_Window *window, int surface_w, int surface_h)
    : m_window(window),
      updateAll(false) {
    m_surface = Surface::make_surface(
            SDL_CreateSurface(surface_w, surface_h, SDL_PIXELFORMAT_ARGB8888),
            NO_ALPHA);
    m_sdlsurface = m_surface->get_surface();
    assert(m_window);
    assert(m_surface);
    assert(m_instance == nullptr);
    m_instance = this;
    m_scaler = std::make_unique<Scaler>(m_surface->get_surface(), nullptr, SDL_GetWindowSurface(m_window));
}

Screen::~Screen() {
    m_instance = nullptr;
}

void Screen::update_all() {
    updateAll = true;
}

void Screen::update_rect(const Rect &r) {
    if (dirtyRects.size() < 200)
        dirtyRects.push_back(r);
    else
        update_all();
}

void Screen::flush_updates() {
    if (updateAll) {
        m_scaler->blit_scaled(m_sdlsurface, nullptr, SDL_GetWindowSurface(m_window), nullptr);
        SDL_UpdateWindowSurface(m_window);
        updateAll = false;
    } else if (!dirtyRects.empty()) {
        dirtyRects.intersect(size());
        std::vector<SDL_Rect> rects(dirtyRects.size());
        auto j = dirtyRects.begin();
        SDL_Surface* window = SDL_GetWindowSurface(m_window);
        for (unsigned i = 0; i < rects.size(); ++i, ++j)
        {
            SDL_Rect srcRect;
            sdl::copy_rect(srcRect, *j);
            int nx = (int)((double) (j->x * window_size().w) / size().w - 0.5);
            int ny = (int)((double) (j->y * window_size().h) / size().h - 0.5);
            int nw = (int)((double) (j->w * window_size().w) / size().w + 3.0);
            int nh = (int)((double) (j->h * window_size().h) / size().h + 3.0);
            nx = Clamp(nx, 0, window->w);
            ny = Clamp(ny, 0, window->h);
            nw = Clamp(nw, 0, window->w - nx);
            nh = Clamp(nh, 0, window->h - ny);
            Rect scaledRect = Rect(nx, ny, nw, nh);
            sdl::copy_rect(rects[i], scaledRect);
            m_scaler->blit_scaled(m_sdlsurface, &srcRect, window, &rects[i]);
        }
        SDL_UpdateWindowSurfaceRects(m_window, &rects[0], rects.size());
    }
    dirtyRects.clear();
}

void Screen::reinitScaler() {
    m_scaler->precalculate(m_surface->get_surface(), nullptr, SDL_GetWindowSurface(m_window));
}

Rect Screen::size() const {
    return Rect(0, 0, width(), height());
}

int Screen::width() const {
    return m_sdlsurface->w;
}

int Screen::height() const {
    return m_sdlsurface->h;
}

Rect Screen::window_size() const {
    return Rect(0, 0, window_width(), window_height());
}

int Screen::window_width() const {
    return SDL_GetWindowSurface(m_window)->w;
}

int Screen::window_height() const {
    return SDL_GetWindowSurface(m_window)->h;
}

/* -------------------- Scaler -------------------- */

// Code originally by Andreas Schiffler (SDL2_rotozoom), heavily adapted to our use case.

Scaler::Scaler(SDL_Surface* _src, SDL_Rect* _srccrop, SDL_Surface* _dst, ScalerMode _mode) {
    assert(_src);
    assert(_dst);
    mode = _mode;
    sax = nullptr;
    say = nullptr;
    precalculate(_src, _srccrop, _dst);
}

Scaler::~Scaler() {
    free(sax);
    free(say);
}

void Scaler::precalculate(SDL_Surface* src, SDL_Rect* srccrop, SDL_Surface* dst) {
    // Free prior allocations.
    free(sax);
    free(say);
    sax = nullptr;
    say = nullptr;

    // Change mode if src/dst are not 32bit depth.
    if ((SDL_GetPixelFormatDetails(src->format)->bytes_per_pixel != 4) ||
        (SDL_GetPixelFormatDetails(dst->format)->bytes_per_pixel != 4)) {
        mode = SC_SDL;
        return;
    }

    // Allocate memory for row/column increments
    if ((sax = (int *) malloc((dst->w + 1) * sizeof(Uint32))) == nullptr) {
        fprintf(stderr, "ecl_video::BlitScaled: Could not allocate memory for row/column increments.\n");
        sax = nullptr;
        return;
    }
    if ((say = (int *) malloc((dst->h + 1) * sizeof(Uint32))) == nullptr) {
        fprintf(stderr, "ecl_video::BlitScaled: Could not allocate memory for row/column increments.\n");
        free(sax);
        sax = nullptr;
        say = nullptr;
        return;
    }

    int cropx, cropy, cropw, croph;

    if (srccrop) {
        cropx = Clamp(srccrop->x, 0, src->w);
        cropy = Clamp(srccrop->y, 0, src->h);
        cropw = Clamp(srccrop->w, 0, src->w - cropx);
        croph = Clamp(srccrop->h, 0, src->h - cropy);
    } else {
        cropx = 0;
        cropy = 0;
        cropw = src->w;
        croph = src->h;
    }

    // Precalculate row increments
    spixelw = (cropw - 1);
    spixelh = (croph - 1);
    spixelgap = src->pitch/4;
    dgap = dst->pitch - dst->w * 4;
    int sx = (int)(65536.0 * (float)spixelw / (float)(dst->w - 1));
    int sy = (int)(65536.0 * (float)spixelh / (float)(dst->h - 1));

    // Maximum scaled source size
    int ssx = (cropw << 16) - 1;
    int ssy = (croph << 16) - 1;

    // Precalculate horizontal row increments
    int csx = cropx;
    int* csax = sax;
    for (int x = 0; x <= dst->w; x++) {
        *csax = csx;
        csax++;
        csx += sx;
        // Guard from overflows
        if (csx > ssx)
            csx = ssx;
    }

    // Precalculate vertical row increments
    int csy = cropy * spixelgap;
    int* csay = say;
    for (int y = 0; y <= dst->h; y++) {
        *csay = csy;
        csay++;
        csy += sy;
        // Guard from overflows
        if (csy > ssy)
            csy = ssy;
    }
}

void Scaler::blit_scaled(
        SDL_Surface* src, const SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect) {
    if (mode == SC_SDL) {
        SDL_BlitSurfaceScaled(src, srcRect, dst, dstRect);
        return;
    }

    // Note: srcrect is ignored in SC_bytewise mode.

    typedef struct tColorRGBA {
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;
    } tColorRGBA;

    int dstrx, dstry, dstrw, dstrh;
    int* salast;
    int sstep;

    if ((sax == nullptr) || (say == nullptr)) {
        fprintf(stderr, "ecl_video::BlitScaled: Precalculations missing. (Error allocating memory?)\n");
        return;
    }

    if (dstRect) {
        dstrx = Clamp(dstRect->x, 0, dst->w);
        dstry = Clamp(dstRect->y, 0, dst->h);
        dstrw = Clamp(dstRect->w, 0, dst->w - dstrx);
        dstrh = Clamp(dstRect->h, 0, dst->h - dstry);
    } else {
        dstrx = 0;
        dstry = 0;
        dstrw = dst->w;
        dstrh = dst->h;
    }

    tColorRGBA* sp = (tColorRGBA*)src->pixels;
    tColorRGBA* dp = (tColorRGBA*)dst->pixels;

    // Advance source pointer y to the beginning of the first line to copy
    int* csay = say + dstry;
    sp += ((*csay >> 16) - (*say >> 16)) * spixelgap;
    // Advance destination pointer to the beginning of the first line to copy
    dp = (tColorRGBA *) ((Uint8 *) (dp + dst->w * dstry) + dgap * dstry);
    for (int y = dstry; y < dstry + dstrh; y++) {
        tColorRGBA* csp = sp;
        // Advance source pointer x
        int* csax = sax + dstrx;
        sp += (*csax >> 16) - (*sax >> 16);
        // Advance destination pointer to first position-to-copy in this row
        dp += dstrx;
        for (int x = dstrx; x < dstrx + dstrw; x++) {
            // Set up color source pointers
            int ex = (*csax & 0xffff);
            int ey = (*csay & 0xffff);
            int cx = (*csax >> 16);
            int cy = (*csay >> 16);
            int sstepx = cx < spixelw;
            int sstepy = cy < spixelh;
            tColorRGBA* c00 = sp;
            tColorRGBA* c01 = sp;
            tColorRGBA* c10 = sp;
            if (sstepy)
                c10 += spixelgap;
            tColorRGBA* c11 = c10;
            if (sstepx) {
                c01++;
                c11++;
            }

            // Draw and interpolate colors
            int t1 = ((((c01->r - c00->r) * ex) >> 16) + c00->r) & 0xff;
            int t2 = ((((c11->r - c10->r) * ex) >> 16) + c10->r) & 0xff;
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

std::unique_ptr<Surface> Duplicate(const Surface *s) {
    assert(s);
    SDL_Surface *sdls = s->get_surface();
    SDL_Surface *copy = SDL_DuplicateSurface(sdls);
    return std::unique_ptr<Surface>(Surface::make_surface(copy));
}

void SavePNG(const Surface *s, const std::string &filename) {
    IMG_SavePNG(s->get_surface(), filename.c_str());
}

void TintRect(Surface *s, Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    std::unique_ptr<Surface> copy(Grab(s, rect));
    if (copy.get()) {
        copy->set_alpha(a);

        GC gc(s);
        set_color(gc, r, g, b);
        box(gc, rect);
        blit(gc, rect.x, rect.y, copy.get());
    }
}

static SDL_Surface* CropSurface(SDL_Surface* surface, SDL_Rect rect) {
    SDL_Surface *cropped = SDL_CreateSurface(rect.w, rect.h, surface->format);
    if (cropped == nullptr)
        return nullptr;

    SDL_Rect bounds;
    bounds.x = 0;
    bounds.y = 0;
    bounds.w = rect.w;
    bounds.h = rect.h;
    SDL_BlitSurface(surface, &rect, cropped, &bounds);

    return cropped;
}

std::unique_ptr<Surface> Grab(const Surface *s, Rect &r) {
    if (s == nullptr)
        return nullptr;

    int x = 0;
    int y = 0;
    clip_blit(s->size(), x, y, r);

    SDL_Rect rect;
    sdl::copy_rect(rect, r);
    SDL_Surface *sdls = s->get_surface();
    SDL_Surface *copy = CropSurface(sdls, rect);
    return Surface::make_surface(copy);
}

// LoadImage is already defined as a macro in some Windows header file
#undef LoadImage

std::unique_ptr<Surface> LoadImage(const char *filename) {
    return Surface::make_surface(IMG_Load(filename));
}

std::unique_ptr<Surface> LoadImage(SDL_IOStream *src, bool freesrc) {
    return Surface::make_surface(IMG_Load_IO(src, freesrc));
}

std::unique_ptr<Surface> MakeSurface(int w, int h) {
    SDL_Surface *surface =
        SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ARGB8888);
    if (surface == nullptr)
        return nullptr;
    return Surface::make_surface(surface);
}

std::unique_ptr<Surface> MakeSurface(void *data, int w, int h, int bipp, int pitch,
                                          const RGBA_Mask &mask) {
    SDL_PixelFormat format = SDL_GetPixelFormatForMasks(bipp, mask.r, mask.g, mask.b, mask.a);
    SDL_Surface *surface =
        SDL_CreateSurfaceFrom(w, h, format, data, pitch);
    if (surface == nullptr)
        return nullptr;
    return Surface::make_surface(surface);
}

void BlitScaled(SDL_Surface* src, SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect, ScalerMode mode) {
    Scaler scaler{src, srcrect, dst, mode};
    scaler.blit_scaled(src, srcrect, dst, dstrect);
}

} // namespace ecl