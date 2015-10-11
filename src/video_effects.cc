/*
 * Copyright (C) 2002,2004 Daniel Heck
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

#include "video_effects.hh"
#include <memory>
#include "enigma.hh"

#define SCREEN ecl::Screen::get_instance()

namespace video {

void FX_Fade(FadeMode mode) {
    ecl::Screen *screen = ecl::Screen::get_instance();
    ecl::Surface *d = screen->get_surface();
    const double fadesec = 0.6;
    double v = 255 / fadesec;

    ecl::Surface *buffer = Duplicate(d);

    double a = mode == FADEIN ? 0 : 255;

    ecl::GC gc(d);

    while (true) {
        Uint32 otime = SDL_GetTicks();

        box(gc, d->size());
        buffer->set_alpha(int(a));
        blit(gc, 0, 0, buffer);
        screen->update_all();
        screen->flush_updates();

        double dt = (SDL_GetTicks() - otime) / 1000.0;
        if ((mode == FADEIN && (a += v * dt) > 255) || (mode == FADEOUT && (a -= v * dt) < 0))
            break;
    }

    if (mode == FADEIN) {
        buffer->set_alpha(255);
        blit(gc, 0, 0, buffer);
    } else {
        box(gc, d->size());
    }
    screen->update_all();
    screen->flush_updates();
    delete buffer;
}

void FX_Fly(ecl::Surface *newscr, int originx, int originy) {
    double rest_time = 0.5;

    double velx = -originx / rest_time;
    double vely = -originy / rest_time;

    double origx = originx;
    double origy = originy;

    ecl::Screen *scr = SCREEN;
    ecl::GC scrgc(scr->get_surface());

    while (rest_time > 0) {
        Uint32 otime = SDL_GetTicks();

        ecl::Rect r(static_cast<int>(origx), static_cast<int>(origy), scr->width(), scr->height());
        blit(scrgc, r.x, r.y, newscr);

        scr->update_rect(r);
        scr->flush_updates();

        double dt = (SDL_GetTicks() - otime) / 1000.0;
        if (dt > rest_time)
            dt = rest_time;
        rest_time -= dt;
        origx += velx * dt;
        origy += vely * dt;
    }
}

namespace {

class Effect_Push : public TransitionEffect {
public:
    Effect_Push(ecl::Surface *newscr, int originx, int originy);
    void tick(double dtime);
    bool finished() const;

private:
    double rest_time;
    ecl::Surface *newscr;
    std::auto_ptr<ecl::Surface> oldscr;
    int originx, originy;
    double velx, vely;
    double accx, accy;
    double x, y;
    double t;
};

Effect_Push::Effect_Push(ecl::Surface *newscr_, int originx_, int originy_)
: rest_time(0.7),
  newscr(newscr_),
  oldscr(Duplicate(SCREEN->get_surface())),
  originx(originx_),
  originy(originy_),
  velx(-2 * originx / rest_time),
  vely(-2 * originy / rest_time),
  accx(-0.5 * velx / rest_time),
  accy(-0.5 * vely / rest_time),
  x(originx),
  y(originy),
  t(0) {
}

void Effect_Push::tick(double dtime) {
    ecl::Screen *scr = SCREEN;
    ecl::GC scrgc(scr->get_surface());

    if (rest_time > 0) {
        if (dtime > rest_time)
            dtime = rest_time;
        rest_time -= dtime;
        t += dtime;

        x = (accx * t + velx) * t + originx;
        y = (accy * t + vely) * t + originy;

        blit(scrgc, (int)x - originx, (int)y, oldscr.get());
        blit(scrgc, (int)x, (int)y - originy, oldscr.get());
        blit(scrgc, (int)x - originx, (int)y - originy, oldscr.get());

        blit(scrgc, (int)x, (int)y, newscr);

        scr->update_all();
        scr->flush_updates();
    } else {
        blit(scrgc, 0, 0, newscr);
        scr->update_all();
        scr->flush_updates();
    }
}

bool Effect_Push::finished() const {
    return rest_time <= 0;
}

}  // namespace

std::unique_ptr<TransitionEffect> CreateEffect(TransitionModes tm, ecl::Surface *newscr) {
    int scrw = SCREEN->width();
    int scrh = SCREEN->height();

    TransitionEffect *effect = nullptr;
    switch (tm) {
    case TM_PUSH_RANDOM: {
        int xo = 0, yo = 0;
        while (xo == 0 && yo == 0) {
            xo = enigma::IntegerRand(-1, 1, false) * scrw;
            yo = enigma::IntegerRand(-1, 1, false) * scrh;
        }
        effect = new Effect_Push(newscr, xo, yo);
        break;
    }
    case TM_PUSH_N: effect = new Effect_Push(newscr, 0, -scrh); break;
    case TM_PUSH_S: effect = new Effect_Push(newscr, 0, +scrh); break;
    case TM_PUSH_E: effect = new Effect_Push(newscr, +scrw, 0); break;
    case TM_PUSH_W: effect = new Effect_Push(newscr, -scrw, 0); break;
    default: break;
    };
    return std::unique_ptr<TransitionEffect>(effect);
}

void ShowScreen(TransitionModes tm, ecl::Surface *newscr) {
    int scrw = SCREEN->width();
    int scrh = SCREEN->height();

    switch (tm) {
    case TM_FLY_N: FX_Fly(newscr, 0, -scrh); break;
    case TM_FLY_S: FX_Fly(newscr, 0, +scrh); break;
    case TM_FLY_E: FX_Fly(newscr, +scrw, 0); break;
    case TM_FLY_W: FX_Fly(newscr, -scrw, 0); break;

    default: break;
    }
}

}  // namespace video
