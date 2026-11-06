/*
 * Copyright (C) 2002,2003,2004,2005,2006 Daniel Heck, Ronald Lamprecht
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

#include "gui/LevelPreviewCache.hh"
#include "ecl_system.hh"
#include "ecl_sdl.hh"
#include "file.hh"
#include "game.hh"
#include "main.hh"
#include "resource_cache.hh"
#include "video.hh"
#include "lev/Proxy.hh"

#include <SDL3/SDL.h>
#include <fstream>
#include <sstream>

using namespace ecl;
using namespace std;

namespace enigma {
namespace gui {

/* -------------------- LevelPreviewCache -------------------- */

LevelPreviewCache *LevelPreviewCache::theSingleton = 0;

LevelPreviewCache *LevelPreviewCache::instance() {
    if (theSingleton == 0) {
        theSingleton = new LevelPreviewCache();
    }
    return theSingleton;
}

LevelPreviewCache::LevelPreviewCache() : cachedIndex(nullptr) {
}

void LevelPreviewCache::clear() {
    Log << "LevelPreviewCache clear\n";
    cache.clear();
    imgCache.clear();
}

Surface *LevelPreviewCache::getPreview(lev::Proxy *levelProxy, const ThumbnailInfo &thumbinfo,
                                       bool allowGeneration, bool &didGenerate) {
    didGenerate = false;
    std::string mapIndex = levelProxy->getId() + ecl::strf("#%d", levelProxy->getReleaseVersion());
    // first look in cache
    PreviewMap::iterator i = cache.find(mapIndex);
    if (i != cache.end())
        return i->second;

    // we have to cache a new preview - check if we should clear cache first
    if (cachedIndex == nullptr) {
        // remember the index we are caching
        cachedIndex = lev::Index::getCurrentIndex();
    } else if (cachedIndex != lev::Index::getCurrentIndex()) {
        // the index changed - cache only previews from new index
        clear();
        cachedIndex = lev::Index::getCurrentIndex();
    }

    const VMInfo &vminfo = *video_engine->GetInfo();
    std::string previewSubPath = makePreviewPath(levelProxy, thumbinfo);
    Surface* surface = nullptr;

    // load preview from file bundled with the level itself
    std::string absLevelPath;
    std::stringstream imageDataStream;
    ByteVec imageData;
    if (levelProxy->getNormPathType() == lev::Proxy::pt_resource &&
        app.resourceFS->findFile("levels/" + levelProxy->getNormLevelPath() + thumbinfo.suffix + ".png",
                                 absLevelPath, imageDataStream)) {
        if (imageDataStream.rdbuf()->in_avail()) {
            // zipped file
            Readfile(imageDataStream, imageData);
        } else {
            // plain file
            basic_ifstream<char> ifs(absLevelPath.c_str(), ios::binary | ios::in);
            Readfile(ifs, imageData);
        }
        SDL_IOStream *imageHandle = SDL_IOFromMem(&(imageData[0]), imageData.size());
        std::unique_ptr<Surface> newSurface = ecl::LoadImage(imageHandle, true);
        surface = imgCache.store(previewSubPath, std::move(newSurface));  // insert in imgCache
    }

    // load preview from stored file if possible
    string previewFullPath;
    if (!surface && app.resourceFS->findFile(previewSubPath, previewFullPath))
        surface = imgCache.get(previewFullPath);

    // generate new preview otherwise
    if (!surface && allowGeneration) {
        std::unique_ptr<ecl::Surface> newSurface = newPreview(levelProxy, thumbinfo);
        if (newSurface) {
            surface = imgCache.store(previewSubPath, std::move(newSurface));      // insert in imgCache
            savePreview(levelProxy, thumbinfo, surface);  // save on filesystem
        } else {
            surface = enigma::GetImage(("error" + thumbinfo.suffix).c_str());
        }
        didGenerate = true;
    }

    // update index
    if (surface)
        cache[mapIndex] = surface;
    return surface;
}

std::unique_ptr<ecl::Surface> LevelPreviewCache::newPreview(lev::Proxy *levelProxy,
                                                            const ThumbnailInfo &thumbinfo) {
    const VMInfo &vminfo = *video_engine->GetInfo();
    std::unique_ptr<Surface> surface;
    ecl::GC gc(video_engine->BackBuffer());
    if (game::DrawLevelPreview(gc, levelProxy)) {
        SDL_Rect r;
        sdl::copy_rect(r, vminfo.gamearea);
        surface = MakeSurface(thumbinfo.width, thumbinfo.height);
        BlitScaled(video_engine->BackBuffer()->get_surface(), &r, surface->get_surface(), nullptr);
    }
    return surface;
}

void LevelPreviewCache::updatePreview(lev::Proxy *levelProxy, const ThumbnailInfo &thumbinfo) {
    if (std::unique_ptr<ecl::Surface> surface = newPreview(levelProxy, thumbinfo)) {
        std::string idx = levelProxy->getId() + ecl::strf("#%d", levelProxy->getReleaseVersion());
        cache[idx] = surface.get();

        string preview_name = makePreviewPath(levelProxy, thumbinfo);
        savePreview(levelProxy, thumbinfo, surface.get());  // save on filesystem
        imgCache.store(preview_name, std::move(surface));        // insert in imgCache
    }
}

std::string LevelPreviewCache::makePreviewPath(lev::Proxy *levelProxy,
                                               const ThumbnailInfo &thumbinfo) {
    return "thumbs" + thumbinfo.suffix + "/" + levelProxy->getLocalSubstitutionLevelPath() +
           ecl::strf("#%d", levelProxy->getReleaseVersion()) + ".png";
}

void LevelPreviewCache::savePreview(lev::Proxy *levelProxy, const ThumbnailInfo &thumbinfo,
                                    ecl::Surface *s) {
    std::string savePath = app.userImagePath + "/" + makePreviewPath(levelProxy, thumbinfo);
    Log << "savePreview to " << savePath << "\n";
    // auto-create the directory if necessary
    string directory;
    if (ecl::split_path(savePath, &directory, 0) && !ecl::FolderExists(directory)) {
        ecl::FolderCreate(directory);
    }
    ecl::SavePNG(s, savePath);
}

void LevelPreviewCache::makeSystemPreview(lev::Proxy *levelProxy, const ThumbnailInfo &thumbinfo,
                                          const std::string &systemDataPath) {
    std::string savePath =
        systemDataPath + "/levels/" + levelProxy->getNormLevelPath() + thumbinfo.suffix + ".png";
    // auto-create the directory if necessary -- on an installed Enigma
    // distribution this is of course unnecessary, but you start Enigma
    // without prior installation. This is useful to get a directory with
    // just the previews.
    string directory;
    if (ecl::split_path(savePath, &directory, 0) && !ecl::FolderExists(directory)) {
        ecl::FolderCreate(directory);
    }
    std::unique_ptr<ecl::Surface> preview = newPreview(levelProxy, thumbinfo);
    if (preview)
        ecl::SavePNG(preview.get(), savePath);
}

}  // namespace gui
}  // namespace enigma
