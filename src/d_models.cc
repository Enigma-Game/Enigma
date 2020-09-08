/*
 * Copyright (C) 2002,2003,2005 Daniel Heck
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
#include "d_models.hh"

#include "lua.hh"
#include "options.hh"
#include "d_engine.hh"
#include "ecl_cache.hh"
#include "ecl_video.hh"
#include "video.hh"
#include "main.hh"
#include "nls.hh"
#include "gui/ErrorMenu.hh"

#include "SDL_image.h"

#include <cstdio>
#include <cstring>
#include <iostream>

using namespace enigma;
using namespace display;
using namespace std;
using namespace ecl;

#ifndef CXXLUA
extern "C" {
#include "lualib.h"
#include "tolua++.h"
}
#else
#include "lualib.h"
#include "tolua++.h"
#endif

#include "lua-global.hh"
#include "lua-display.hh"
#include "lua-enigma.hh"
#include "lua-ecl.hh"

/* -------------------- Types -------------------- */

namespace {
class SurfaceCache_Alpha : public PtrCache<Surface> {
public:
    Surface *acquire(const std::string &name) override;
};

class SurfaceCache : public PtrCache<Surface> {
public:
    Surface *acquire(const std::string &name) override;
};

class ModelManager {
public:
    ModelManager();
    ~ModelManager();

    void define(const std::string name, Model *m);

    /* Create new model of type `name'.  Returns 0 if no such
       model exists. */
    Model *create(const std::string &name);

    /* Remove model definition for `name'. */
    void remove(const std::string &name);

    bool has_model(const std::string &name) const;

    size_t num_templates() const;

private:
    // Variables
    typedef ecl::Dict<Model *> ModelMap;
    ModelMap m_templates;
};
}

/* -------------------- SurfaceCache -------------------- */

Surface *SurfaceCache_Alpha::acquire(const std::string &name) {
    const VMInfo *vminfo = video_engine->GetInfo();
    std::string filename;
    std::unique_ptr<ecl::Surface> es;

    FindImageReturnCode found = app.resourceFS->findImageFile(name + ".png", filename);
    if (found != IMAGE_NOT_FOUND)
        es.reset(ecl::LoadImage(filename.c_str()));
    if (found == IMAGE_NEEDS_SCALING_32_TO_16)
        return es->zoom(es->width() / 2, es->height() / 2);
    if (found == IMAGE_NEEDS_SCALING_48_TO_64)
        return es->zoom((es->width() * 4) / 3, (es->height() * 4) / 3);
    if (found == IMAGE_NEEDS_SCALING_32_TO_64)
        return es->zoom(es->width() * 2, es->height() * 2);
    return es.release();
}

Surface *SurfaceCache::acquire(const std::string &name) {
    const VMInfo *vminfo = video_engine->GetInfo();
    std::string filename;
    std::unique_ptr<ecl::Surface> es;

    FindImageReturnCode found = app.resourceFS->findImageFile(name + ".png", filename);
    if (found != IMAGE_NOT_FOUND) {
        // TODO(sdl2): is there a reason this is different from SurfaceCache_Alpha?
        if (SDL_Surface *s = IMG_Load(filename.c_str()))
            es.reset(ecl::LoadImage(filename.c_str()));
    }
    if (found == IMAGE_NEEDS_SCALING_32_TO_16)
        return es->zoom(es->width() / 2, es->height() / 2);
    if (found == IMAGE_NEEDS_SCALING_48_TO_64)
        return es->zoom((es->width() * 4) / 3, (es->height() * 4) / 3);
    if (found == IMAGE_NEEDS_SCALING_32_TO_64)
        return es->zoom(es->width() * 2, es->height() * 2);
    return es.release();
}

/* -------------------- ModelManager -------------------- */

ModelManager::ModelManager() : m_templates(1069) {
}

ModelManager::~ModelManager() {
    delete_map(m_templates.begin(), m_templates.end());
}

void ModelManager::define(const std::string name, Model *m) {
    m_templates.insert(name, m);
}

Model *ModelManager::create(const std::string &name) {
    auto i = m_templates.find(name);
    if (i != m_templates.end())
        return i->second->clone();
    return nullptr;
}

void ModelManager::remove(const std::string &name) {
    ModelMap::iterator i = m_templates.find(name);
    if (i != m_templates.end()) {
        delete i->second;
        m_templates.remove(name);
    }
}

bool ModelManager::has_model(const std::string &name) const {
    return m_templates.has_key(name);
}

size_t ModelManager::num_templates() const {
    return m_templates.size();
}

/* -------------------- Variables -------------------- */

namespace {

SurfaceCache surface_cache;
SurfaceCache_Alpha surface_cache_alpha;
ModelManager *modelmgr = nullptr;
vector<Surface *> image_pile;
string anim_templ_name;
Anim2d *anim_templ = nullptr;

}  // namespace

/* -------------------- Functions -------------------- */

void display::InitModels() {
    const VideoTileset *vts = video_engine->GetTileset();

    modelmgr = new ModelManager;

    lua_State *L = lua_open();
    luaL_openlibs(L);
    lua_register(L, "FindDataFile", lua::FindDataFile);
    tolua_open(L);
    tolua_global_open(L);
    tolua_enigma_open(L);
    tolua_display_open(L);
    tolua_px_open(L);

    if (lua::DoSysFile(L, "compat.lua") != lua::NO_LUAERROR) {
        std::string message =
            ecl::strf("Error loading 'compat.lua'\nError: '%s'\n", lua::LastError(L).c_str());
        fprintf(stderr, "%s", message.c_str());
        gui::ErrorMenu m(message + _("\n\nThis error may cause the application to behave strange!"),
                         N_("Continue"));
        m.manage();
    }

    string fname;

    fname = app.systemFS->findFile(vts->initscript);
    if (lua::DoSysFile(L, vts->initscript) != lua::NO_LUAERROR) {
        std::string message = ecl::strf("Error loading '%s'\nError: '%s'\n", fname.c_str(),
                                        lua::LastError(L).c_str());
        fprintf(stderr, "%s", message.c_str());
        gui::ErrorMenu m(message + _("\n\nThis error may cause the application to behave strange!"),
                         N_("Continue"));
        m.manage();
    }
    enigma::Log << "# models: " << modelmgr->num_templates() << endl;

    surface_cache_alpha.clear();
    lua_close(L);
}

void display::ShutdownModels() {
    delete modelmgr;
    surface_cache.clear();
    delete_sequence(image_pile.begin(), image_pile.end());
    image_pile.clear();
    anim_templ_name = "";
    anim_templ = nullptr;
}

Surface *display::CropSurface(const Surface *s, Rect r) {
    return ecl::Grab(s, r);
}

/* Register a new model template `m' under the name `name'. */
void display::DefineModel(const char *name, Model *m) {
    if (modelmgr->has_model(name)) {
        enigma::Log << "Redefining model '" << name << "'\n";
        modelmgr->remove(name);
    }
    modelmgr->define(name, m);
}

Model *display::MakeModel(const string &name) {
    if (Model *m = modelmgr->create(name))
        return m;
    else {
        enigma::Log << "Unknown model " << name << endl;
        return modelmgr->create("dummy");
    }
}

int display::DefineImage(const char *name, const char *fname, int xoff, int yoff, int padding) {
    ecl::Surface *sfc = surface_cache.get(fname);
    if (!sfc)
        return 1;

    ecl::Rect r = sfc->size();
    r.x += padding;
    r.y += padding;
    r.w -= 2 * padding;
    r.h -= 2 * padding;
    DefineModel(name, new ImageModel(sfc, r, xoff + padding, yoff + padding));
    return 0;
}

void display::DefineImageModel(const char *name, ecl::Surface *s) {
    DefineModel(name, new ImageModel(s, 0, 0));
}

int display::DefineSubImage(const char *name, const char *fname, int xoff, int yoff,
                            ecl::Rect subrect) {
    ecl::Surface *sfc = surface_cache.get(fname);
    if (!sfc)
        return 1;

    DefineModel(name, new ImageModel(sfc, subrect, xoff, yoff));
    return 0;
}

void display::DefineRandModel(const char *name, int n, char **names) {
    auto m = new RandomModel();
    for (int i = 0; i < n; i++)
        m->add_model(names[i]);
    DefineModel(name, m);
}

void display::DefineShadedModel(const char *name, const char *model, const char *shadow) {
    DefineModel(name, new ShadowModel(MakeModel(model), MakeModel(shadow)));
}

/* Create an image by overlaying several other images.  The first entry in
   `images' is the name of the background image, the following images are
   drawn on top of it. */
void display::DefineOverlayImage(const char *name, int n, char **images) {
    Surface *sfc = Duplicate(surface_cache.get(images[0]));
    if (sfc) {
        GC gc(sfc);
        for (int i = 1; i < n; i++)
            blit(gc, 0, 0, surface_cache_alpha.get(images[i]));
        DefineModel(name, new ImageModel(sfc, 0, 0));
        image_pile.push_back(sfc);  // make sure it gets destructed
    }
}

void display::DefineComposite(const char *name, const char *bgname, const char *fgname) {
    DefineModel(name, new CompositeModel(MakeModel(bgname), MakeModel(fgname)));
}

void display::DefineAnim(const char *name, bool loop_p) {
    anim_templ = new Anim2d(loop_p);
    DefineModel(name, anim_templ);
    anim_templ_name = name;
}

void display::AddFrame(const char *name, const char *model, double time) {
    if (anim_templ_name != name)
        fprintf(stderr, "AddFrame: Cannot add frames to completed animations.");
    else
        anim_templ->add_frame(MakeModel(model), time / 1000.0);
}

void display::DefineAlias(const char *name, const char *othername) {
    if (std::strcmp(name, othername) != 0)
        DefineModel(name, new AliasModel(othername));
}

/* -------------------- Model -------------------- */
void Model::get_extension(ecl::Rect &r) {
}

/* -------------------- Image -------------------- */

Image::Image(ecl::Surface *sfc) : surface(sfc), rect(surface->size()), refcount(1) {
}

Image::Image(ecl::Surface *sfc, ecl::Rect r) : surface(sfc), rect(std::move(r)), refcount(1) {
}

void display::incref(Image *i) {
    ++i->refcount;
}

void display::decref(Image *i) {
    if (--i->refcount == 0) {
        delete i;
    }
}

void display::draw_image(Image *i, ecl::GC &gc, int x, int y) {
    blit(gc, x, y, i->surface, i->rect);
}

/* -------------------- ImageModel -------------------- */

ImageModel::ImageModel(Image *i, int xo, int yo) : image(i), xoff(xo), yoff(yo) {
    assert(image);
    incref(image);
}

ImageModel::ImageModel(Surface *s, int xo, int yo) : image(new Image(s)), xoff(xo), yoff(yo) {
}

ImageModel::ImageModel(Surface *s, const ecl::Rect &r, int xo, int yo)
: image(new Image(s, r)), xoff(xo), yoff(yo) {
}

ImageModel::~ImageModel() {
    decref(image);
}

void ImageModel::draw(ecl::GC &gc, int x, int y) {
    draw_image(image, gc, x + xoff, y + yoff);
}

Model *ImageModel::clone() {
    return new ImageModel(image, xoff, yoff);
}

void ImageModel::get_extension(ecl::Rect &r) {
    r.x = xoff;
    r.y = yoff;
    r.w = image->rect.w;
    r.h = image->rect.h;
}

/* -------------------- ShadowModel -------------------- */

ShadowModel::ShadowModel(Model *m, Model *sh) {
    model = m;
    shade = sh;
    ecl::Rect r1, r2;
    model->get_extension(r1);
    shade->get_extension(r2);
    extension = boundingbox(r1, r2);
}

ShadowModel::~ShadowModel() {
    delete model;
    delete shade;
}

void ShadowModel::expose(ModelLayer *ml, int vx, int vy) {
    model->expose(ml, vx, vy);
    shade->expose(ml, vx, vy);
}
void ShadowModel::remove(ModelLayer *ml) {
    shade->remove(ml);
    model->remove(ml);
}

void ShadowModel::set_callback(ModelCallback *cb) {
    model->set_callback(cb);
}

void ShadowModel::reverse() {
    model->reverse();
    shade->reverse();
}

void ShadowModel::restart() {
    model->restart();
    shade->restart();
}

void ShadowModel::draw(ecl::GC &gc, int x, int y) {
    model->draw(gc, x, y);
}

void ShadowModel::draw_shadow(ecl::GC &gc, int x, int y) {
    shade->draw(gc, x, y);
}

Model *ShadowModel::get_shadow() const {
    return shade;
}

Model *ShadowModel::clone() {
    return new ShadowModel(model->clone(), shade->clone());
}

void ShadowModel::get_extension(ecl::Rect &r) {
    r = extension;
}

/* -------------------- RandomModel -------------------- */

Model *RandomModel::clone() {
    if (!modelnames.empty()) {
        int r = enigma::IntegerRand(0, modelnames.size() - 1, false);
        return MakeModel(modelnames[r]);
    } else {
        fprintf(stderr, "display_2d.cc: empty RandomModel\n");
        return nullptr;
    }
}

/* -------------------- AliasModel -------------------- */

Model *AliasModel::clone() {
    return MakeModel(name);
}

/* -------------------- Anim2d -------------------- */

Anim2d::Anim2d(bool loop) : rep(new AnimRep(loop)) {
}

Anim2d::Anim2d(AnimRep *r, ecl::Rect &ext_r)
: rep(r),
  curframe(0),
  frametime(0),
  finishedp(false),
  changedp(false),
  reversep(false),
  videox(0),
  videoy(0),
  callback(nullptr),
  extension(ext_r) {
    rep->refcount++;
    frametime = 0;
}

Anim2d::~Anim2d() {
    if (--rep->refcount == 0)
        delete rep;
}

void Anim2d::restart() {
    finishedp = false;
    frametime = 0;
    curframe = 0;
    changedp = true;
}

void Anim2d::add_frame(Model *m, double duration) {
    rep->frames.push_back(new AnimFrame(m, duration));

    // Cache the bounding box of all frames to ensure that it is constant.
    ecl::Rect r1, r2;
    m->get_extension(r1);
    r2 = extension;
    extension = boundingbox(r1, r2);
}

void Anim2d::draw(ecl::GC &gc, int x, int y) {
    if (!finishedp) {
        AnimFrame *f = rep->frames[curframe];
        f->model->draw(gc, x, y);
        changedp = false;
    }
}

void Anim2d::draw_shadow(ecl::GC &gc, int x, int y) {
    if (!finishedp) {
        AnimFrame *f = rep->frames[curframe];
        f->model->draw_shadow(gc, x, y);
    }
}

void Anim2d::expose(ModelLayer *ml, int vx, int vy) {
    ml->activate(this);
    videox = vx;
    videoy = vy;
}

void Anim2d::remove(ModelLayer *ml) {
    ml->deactivate(this);
}

bool Anim2d::has_changed(Rect &r) {
    bool retval = changedp;
    if (changedp) {
        get_extension(r);
        r.x += videox;
        r.y += videoy;
    }
    return retval;
}

void Anim2d::move(int newx, int newy) {
    videox = newx;
    videoy = newy;
}

void Anim2d::get_extension(ecl::Rect &r) {
    r = extension;
}

void Anim2d::tick(double dtime) {
    assert(curframe < rep->frames.size());
    frametime += dtime;
    double framedur = rep->frames[curframe]->duration;

    if (frametime >= framedur) {
        frametime -= framedur;
        changedp = true;

        if (reversep) {
            if (curframe >= 1)
                curframe--;
            else if (rep->loop)
                curframe = rep->frames.size() - 1;
            else
                finishedp = true;
        } else {
            if (curframe + 1 < rep->frames.size())
                curframe++;
            else if (rep->loop)
                curframe = 0;
            else
                finishedp = true;
        }
        if (finishedp && callback != nullptr)
            callback->animcb();
    }
}

/* -------------------- Functions -------------------- */

namespace display {

Surface *GetSurface(const std::string &filename) {
    return surface_cache.get(filename);
}

}  // namespace display
