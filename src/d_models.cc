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

#include "d_engine.hh"
#include "ecl_cache.hh"
#include "ecl_video.hh"
#include "gui/ErrorMenu.hh"
#include "lua.hh"
#include "main.hh"
#include "nls.hh"
#include "video.hh"

#include <cstdio>
#include <cstring>
#include <iostream>

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


namespace enigma::display {

/* -------------------- Types -------------------- */

namespace {
    class SurfaceCache_Alpha : public ecl::Cache<ecl::Surface> {
        std::unique_ptr<ecl::Surface> acquire(const std::string& name) override;
    };

    class SurfaceCache : public ecl::Cache<ecl::Surface> {
        std::unique_ptr<ecl::Surface> acquire(const std::string& name) override;
    };

    class ModelManager {
    public:
        ModelManager();
        ~ModelManager();

        void define(const std::string& name, Model* model);

        /// Create a new model of type 'name'.  Returns null if no such model exists.
        std::unique_ptr<Model> create(const std::string& name);

        /* Remove the model definition for 'name'. */
        void remove(const std::string& name);

        bool hasModel(const std::string& name) const;

        size_t numPrototypes() const;

    private:
        // Variables
        using ModelMap = std::unordered_map<std::string, std::unique_ptr<Model>>;
        ModelMap prototypes;
    };
} // namespace

/* -------------------- SurfaceCache -------------------- */

std::unique_ptr<ecl::Surface> SurfaceCache_Alpha::acquire(const std::string &name) {
    std::string filename;
    std::unique_ptr<ecl::Surface> es;

    FindImageReturnCode found = app.resourceFS->findImageFile(name + ".png", filename);
    if (found != IMAGE_NOT_FOUND)
        es = ecl::LoadImage(filename.c_str());
    if (found == IMAGE_NEEDS_SCALING_32_TO_16)
        return es->zoom(es->width() / 2, es->height() / 2);
    if (found == IMAGE_NEEDS_SCALING_48_TO_64)
        return es->zoom((es->width() * 4) / 3, (es->height() * 4) / 3);
    if (found == IMAGE_NEEDS_SCALING_32_TO_64)
        return es->zoom(es->width() * 2, es->height() * 2);
    return es;
}

std::unique_ptr<ecl::Surface> SurfaceCache::acquire(const std::string &name) {
    std::string filename;
    std::unique_ptr<ecl::Surface> es;

    FindImageReturnCode found = app.resourceFS->findImageFile(name + ".png", filename);
    if (found != IMAGE_NOT_FOUND) {
        es = ecl::LoadImage(filename.c_str());
    }
    if (found == IMAGE_NEEDS_SCALING_32_TO_16)
        return es->zoom(es->width() / 2, es->height() / 2);
    if (found == IMAGE_NEEDS_SCALING_48_TO_64)
        return es->zoom((es->width() * 4) / 3, (es->height() * 4) / 3);
    if (found == IMAGE_NEEDS_SCALING_32_TO_64)
        return es->zoom(es->width() * 2, es->height() * 2);
    return es;
}

/* -------------------- ModelManager -------------------- */

ModelManager::ModelManager() : prototypes(1069) {
}

ModelManager::~ModelManager() = default;

void ModelManager::define(const std::string &name, Model *model) {
    prototypes[name] = std::unique_ptr<Model>(model);
}

std::unique_ptr<Model> ModelManager::create(const std::string &name) {
    auto i = prototypes.find(name);
    return i != prototypes.end() ? std::unique_ptr<Model>(i->second->clone()) : nullptr;
}

void ModelManager::remove(const std::string &name) {
    prototypes.erase(name);
}

bool ModelManager::hasModel(const std::string &name) const {
    return prototypes.find(name) != prototypes.end();
}

size_t ModelManager::numPrototypes() const {
    return prototypes.size();
}

/* -------------------- Variables -------------------- */

namespace {

    SurfaceCache surfaceCache;
    SurfaceCache_Alpha surfaceCacheAlpha;
    ModelManager* modelManager = nullptr;
    std::string currentAnimationName;
    Anim2d* currentAnimation = nullptr;

}  // namespace

/* -------------------- Functions -------------------- */

void InitModels() {
    const VideoTileset *vts = video_engine->GetTileset();

    modelManager = new ModelManager;

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

    std::string fname;

    fname = app.systemFS->findFile(vts->initscript);
    if (lua::DoSysFile(L, vts->initscript) != lua::NO_LUAERROR) {
        std::string message = ecl::strf("Error loading '%s'\nError: '%s'\n", fname.c_str(),
                                        lua::LastError(L).c_str());
        fprintf(stderr, "%s", message.c_str());
        gui::ErrorMenu m(message + _("\n\nThis error may cause the application to behave strange!"),
                         N_("Continue"));
        m.manage();
    }
    Log << "# models: " << modelManager->numPrototypes() << std::endl;

    surfaceCacheAlpha.clear();
    lua_close(L);
}

void ShutdownModels() {
    delete modelManager;
    surfaceCache.clear();
    currentAnimationName = "";
    currentAnimation = nullptr;
}

ecl::Surface* CropSurface(const ecl::Surface *s, ecl::Rect r) {
    // We have to unbox the unique_ptr here because the pointer is
    // passed to the Lua layer.
    return ecl::Grab(s, r).release();
}

/// Register a new model template `m' under the name `name'.
/// Takes ownership of 'm'.
void DefineModel(const char *name, Model *m) {
    if (modelManager->hasModel(name)) {
        Log << "Redefining model '" << name << "'\n";
        modelManager->remove(name);
    }
    modelManager->define(name, m);
}

std::unique_ptr<Model> MakeModel(const std::string &name) {
    if (std::unique_ptr<Model> m = modelManager->create(name)) {
        return m;
    } else {
        Log << "Unknown model " << name << std::endl;
        return modelManager->create("dummy");
    }
}

int DefineImage(const char *name, const char *fname, int xoff, int yoff, int padding) {
    ecl::Surface *surface = surfaceCache.get(fname);
    if (!surface)
        return 1;

    ecl::Rect r = surface->size();
    r.x += padding;
    r.y += padding;
    r.w -= 2 * padding;
    r.h -= 2 * padding;
    DefineModel(name, new ImageModel(surface, r, xoff + padding, yoff + padding));
    return 0;
}

void DefineImageModel(const char *name, ecl::Surface *s) {
    DefineModel(name, new ImageModel(std::unique_ptr<ecl::Surface>(s), 0, 0));
}

int DefineSubImage(const char* name, const char* fname, int xOff, int yOff, ecl::Rect subRect) {
    ecl::Surface *surface = surfaceCache.get(fname);
    if (!surface)
        return 1;

    DefineModel(name, new ImageModel(surface, subRect, xOff, yOff));
    return 0;
}

void DefineRandModel(const char *name, int n, char **names) {
    auto m = new RandomModel();
    for (int i = 0; i < n; i++)
        m->add_model(names[i]);
    DefineModel(name, m);
}

void DefineShadedModel(const char *name, const char *model, const char *shadow) {
    DefineModel(name, new ShadowModel(MakeModel(model), MakeModel(shadow)));
}

/* Create an image by overlaying several other images.  The first entry in
   `images' is the name of the background image, the following images are
   drawn on top of it. */
void DefineOverlayImage(const char *name, int n, char **images) {
    std::unique_ptr<ecl::Surface> surface = Duplicate(surfaceCache.get(images[0]));
    if (surface) {
        ecl::GC gc(surface.get());
        for (int i = 1; i < n; i++)
            blit(gc, 0, 0, surfaceCacheAlpha.get(images[i]));
        DefineModel(name, new ImageModel(std::move(surface), 0, 0));
    }
}

void DefineComposite(const char *name, const char *bgname, const char *fgname) {
    DefineModel(name, new CompositeModel(MakeModel(bgname), MakeModel(fgname)));
}

void DefineAnim(const char *name, bool looping) {
    currentAnimation = new Anim2d(looping);
    DefineModel(name, currentAnimation);
    currentAnimationName = name;
}

void AddFrame(const char *name, const char *model, double time) {
    if (currentAnimationName != name)
        fprintf(stderr, "AddFrame: Cannot add frames to completed animations.");
    else
        currentAnimation->addFrame(MakeModel(model), time / 1000.0);
}

void DefineAlias(const char *name, const char *othername) {
    if (std::strcmp(name, othername) != 0)
        DefineModel(name, new AliasModel(othername));
}

/* -------------------- Model -------------------- */
ecl::Rect Model::boundingBox() {
    return ecl::Rect();
}

/* -------------------- Image -------------------- */

Image::Image(std::unique_ptr<ecl::Surface> sfc) : surface(std::move(sfc)), rect(surface->size()) {
}

Image::Image(ecl::Surface *sfc, ecl::Rect r) : surface(Duplicate(sfc)), rect(r) {
}

void draw_image(Image *image, ecl::GC &gc, int x, int y) {
    blit(gc, x, y, image->surface.get(), image->rect);
}

/* -------------------- ImageModel -------------------- */

ImageModel::ImageModel(const std::shared_ptr<Image> &image, int xo, int yo)
: image(image), xoff(xo), yoff(yo) {
    assert(image);
}

ImageModel::ImageModel(std::unique_ptr<ecl::Surface> s, int xo, int yo)
: image(std::make_shared<Image>(std::move(s))), xoff(xo), yoff(yo) {
}

ImageModel::ImageModel(ecl::Surface *s, const ecl::Rect &r, int xo, int yo)
: image(std::make_shared<Image>(s, r)), xoff(xo), yoff(yo) {
}

ImageModel::~ImageModel() = default;

void ImageModel::draw(ecl::GC &gc, int x, int y) {
    draw_image(image.get(), gc, x + xoff, y + yoff);
}

std::unique_ptr<Model> ImageModel::clone() {
    return std::make_unique<ImageModel>(image, xoff, yoff);
}

ecl::Rect ImageModel::boundingBox() {
    return ecl::Rect(xoff, yoff, image->rect.w, image->rect.h);
}

/* -------------------- ShadowModel -------------------- */

ShadowModel::ShadowModel(std::unique_ptr<Model> m, std::unique_ptr<Model> sh)
    : model(std::move(m)), shadow(std::move(sh)) {
    bbox = ecl::boundingbox(model->boundingBox(), shadow->boundingBox());
}

ShadowModel::~ShadowModel() = default;

void ShadowModel::expose(ModelLayer *ml, int vx, int vy) {
    model->expose(ml, vx, vy);
    shadow->expose(ml, vx, vy);
}
void ShadowModel::removeFromLayer(ModelLayer *ml) {
    shadow->removeFromLayer(ml);
    model->removeFromLayer(ml);
}

void ShadowModel::setCallback(ModelCallback *cb) {
    model->setCallback(cb);
}

void ShadowModel::reverse() {
    model->reverse();
    shadow->reverse();
}

void ShadowModel::restart() {
    model->restart();
    shadow->restart();
}

void ShadowModel::draw(ecl::GC &gc, int x, int y) {
    model->draw(gc, x, y);
}

void ShadowModel::drawShadow(ecl::GC &gc, int x, int y) {
    shadow->draw(gc, x, y);
}

Model *ShadowModel::get_shadow() const {
    return shadow.get();
}

std::unique_ptr<Model> ShadowModel::clone() {
    return std::make_unique<ShadowModel>(model->clone(), shadow->clone());
}

ecl::Rect ShadowModel::boundingBox() {
    return bbox;
}

/* -------------------- RandomModel -------------------- */

std::unique_ptr<Model> RandomModel::clone() {
    if (!modelNames.empty()) {
        int index = IntegerRand(0, modelNames.size() - 1, false);
        return MakeModel(modelNames[index]);
    } else {
        fprintf(stderr, "display_2d.cc: empty RandomModel\n");
        return nullptr;
    }
}

/* -------------------- AliasModel -------------------- */

std::unique_ptr<Model> AliasModel::clone() {
    return MakeModel(name);
}

/* -------------------- Anim2d -------------------- */

Anim2d::Anim2d(bool looping) : rep(std::make_shared<AnimRep>(looping)) {
}

Anim2d::Anim2d(const std::shared_ptr<AnimRep> &rep, const ecl::Rect &bbox) : rep(rep), bbox(bbox) {
    frameTime = 0;
}

void Anim2d::setCallback(ModelCallback *cb) {
    callback = cb;
}

void Anim2d::reverse() {
    reversed = !reversed;
}

void Anim2d::restart() {
    finished = false;
    frameTime = 0;
    curframe = 0;
    changed = true;
}

void Anim2d::addFrame(std::unique_ptr<Model> model, double duration) {
    ecl::Rect frameBbox = model->boundingBox();
    rep->frames.push_back(std::make_unique<AnimFrame>(std::move(model), duration));

    // Cache the bounding box of all frames to ensure that it is constant.
    bbox = ecl::boundingbox(frameBbox, bbox);
}

void Anim2d::draw(ecl::GC &gc, int x, int y) {
    if (!finished) {
        AnimFrame *frame = rep->frames[curframe].get();
        frame->model->draw(gc, x, y);
        changed = false;
    }
}

void Anim2d::drawShadow(ecl::GC &gc, int x, int y) {
    if (!finished) {
        AnimFrame *frame = rep->frames[curframe].get();
        frame->model->drawShadow(gc, x, y);
    }
}

std::unique_ptr<Model> Anim2d::clone() {
    return std::make_unique<Anim2d>(rep, bbox);
}

void Anim2d::expose(ModelLayer *layer, int vx, int vy) {
    layer->activate(this);
    screenX = vx;
    screenY = vy;
}

void Anim2d::removeFromLayer(ModelLayer *layer) {
    layer->deactivate(this);
}

bool Anim2d::hasChanged(ecl::Rect &changedRegion) {
    if (changed) {
        changedRegion = bbox;
        changedRegion.x += screenX;
        changedRegion.y += screenY;
    }
    return changed;
}

void Anim2d::move(int newX, int newY) {
    screenX = newX;
    screenY = newY;
}

ecl::Rect Anim2d::boundingBox() {
    return bbox;
}

void Anim2d::tick(double dtime) {
    assert(curframe < rep->frames.size());
    frameTime += dtime;
    double frameDuration = rep->frames[curframe]->duration;

    if (frameTime >= frameDuration) {
        frameTime -= frameDuration;
        changed = true;

        if (reversed) {
            if (curframe >= 1)
                curframe--;
            else if (rep->looping)
                curframe = rep->frames.size() - 1;
            else
                finished = true;
        } else {
            if (curframe + 1 < rep->frames.size())
                curframe++;
            else if (rep->looping)
                curframe = 0;
            else
                finished = true;
        }
        if (finished && callback != nullptr)
            callback->animcb();
    }
}

/* -------------------- Functions -------------------- */


ecl::Surface *GetSurface(const std::string &filename) {
    return surfaceCache.get(filename);
}

} // namespace enigma::display
