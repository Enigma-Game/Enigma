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
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef D_MODELS_HH
#define D_MODELS_HH

#include "display_internal.hh"
#include "display.hh"
#include "ecl_video.hh"
#include "ecl_geom.hh"
#include <vector>
#include <string>

namespace display {

/* -------------------- Image -------------------- */

struct Image {
    // Variables.
    ecl::Surface *surface;
    ecl::Rect rect;  // location of image inside surface
    int refcount;    // reference count, initialized to 1

    // Constructors.
    Image(ecl::Surface *sfc);
    Image(ecl::Surface *sfc, ecl::Rect r);
};

void incref(Image *i);
void decref(Image *i);
void draw_image(Image *i, ecl::GC &gc, int x, int y);

/* -------------------- ImageModel -------------------- */

class ImageModel : public Model {
    Image *image;
    int xoff, yoff;  // relative origin of the image
public:
    // Constructors
    ImageModel(Image *i, int xo, int yo);
    ImageModel(ecl::Surface *s, int xo, int yo);
    ImageModel(ecl::Surface *s, const ecl::Rect &r, int xo, int yo);
    ~ImageModel();

    // Model interface
    void draw(ecl::GC &gc, int x, int y) override;
    Model *clone() override;
    void get_extension(ecl::Rect &r) override;
    Image *get_image() { return image; }
};

/* -------------------- ShadowModel -------------------- */

class ShadowModel : public Model {
public:
    ShadowModel(Model *m, Model *sh);
    ~ShadowModel();

    // Model interface
    void expose(ModelLayer *ml, int vx, int vy) override;
    void remove(ModelLayer *ml) override;

    void set_callback(ModelCallback *cb) override;
    void reverse() override;
    void restart() override;
    void draw(ecl::GC &gc, int x, int y) override;
    void draw_shadow(ecl::GC &gc, int x, int y) override;
    Model *get_shadow() const override;
    Model *clone() override;

    void get_extension(ecl::Rect &r) override;

private:
    Model *model, *shade;
    ecl::Rect extension;  // bounding extesion of model and shade
};

/* -------------------- CompositeModel -------------------- */

class CompositeModel : public Model {
    Model *bg, *fg;

public:
    CompositeModel(Model *b, Model *f) : bg(b), fg(f) {}
    ~CompositeModel() {
        delete bg;
        delete fg;
    }

    // Animation interface
    void set_callback(ModelCallback *cb) override {
        fg->set_callback(cb);
    }
    void reverse() override {
        fg->reverse();
    }
    void restart() override { fg->restart(); }

    // Model interface
    Model *get_shadow() const override { return bg->get_shadow(); }
    virtual void expose(ModelLayer *ml, int vx, int vy) override {
        fg->expose(ml, vx, vy);
    }
    virtual void remove(ModelLayer *ml) override {
        fg->remove(ml);
    }
    void draw(ecl::GC &gc, int x, int y) override {
        bg->draw(gc, x, y);
        fg->draw(gc, x, y);
    }
    void draw_shadow(ecl::GC &gc, int x, int y) override {
        bg->draw_shadow(gc, x, y);
    }
    Model *clone() override { return new CompositeModel(bg->clone(), fg->clone()); }

    void get_extension(ecl::Rect &r) override {
        fg->get_extension(r);
    }
};

/* -------------------- RandomModel -------------------- */

/* Creates new models randomly from a set of template models. */
class RandomModel : public Model {
    std::vector<std::string> modelnames;

public:
    void add_model(const std::string &name) { modelnames.push_back(name); }
    Model *clone() override;
};

/* -------------------- AliasModel -------------------- */

class AliasModel : public Model {
    std::string name;

public:
    AliasModel(std::string modelname) : name(std::move(modelname)) {}
    Model *clone() override;
};

/* -------------------- Animations -------------------- */

struct AnimFrame : public ecl::Nocopy {
    // Variables
    Model *model;
    double duration;

    // Constructor and Destructor
    AnimFrame(Model *m, double dur) : model(m), duration(dur) {}

    ~AnimFrame() { delete model; }
};

struct AnimRep {
    // Variables
    std::vector<AnimFrame *> frames;
    bool loop;
    int refcount;

    // Constructor and Destructor
    AnimRep(bool l) : loop(l), refcount(1) {}

    ~AnimRep() { delete_sequence(frames.begin(), frames.end()); }
};

class Anim2d : public Model, public ecl::Nocopy {
public:
    Anim2d(bool loop);
    ~Anim2d();
    void set_callback(ModelCallback *cb) override { callback = cb; }

    void add_frame(Model *m, double duration);

    /* ---------- Model interface ---------- */
    void draw(ecl::GC &gc, int x, int y) override;
    void draw_shadow(ecl::GC &gc, int x, int y) override;
    Model *clone() override { return new Anim2d(rep, extension); }
    void reverse() override { reversep = !reversep; }
    void restart() override;

    void expose(ModelLayer *ml, int vx, int vy) override;
    void remove(ModelLayer *ml) override;

    void tick(double dtime) override;
    bool has_changed(ecl::Rect &changed_region) override;
    bool is_garbage() const override { return finishedp; }

    void move(int newx, int newy);
    void get_extension(ecl::Rect &r) override;

private:
    Anim2d(AnimRep *r, ecl::Rect &ext_r);

    /* ---------- Variables ---------- */
    AnimRep *rep;
    unsigned curframe;  // Current frame number
    double frametime;   // Elapsed time since frame was activated
    bool finishedp;     // Animation has finished
    bool changedp;      // Model state has changed since last redraw
    bool reversep;      // Play the animation in reverse direction

    int videox, videoy;   // Video coordinates of sprite
    ecl::Rect extension;  // bounding extension of all frames
    ModelCallback *callback;
};

ecl::Surface *GetSurface(const std::string &filename);
ecl::Surface *CropSurface(const ecl::Surface *s, ecl::Rect r);
void DefineModel(const char *name, Model *m);
void DefineImageModel(const char *name, ecl::Surface *s);

}  // namespace display

#endif
