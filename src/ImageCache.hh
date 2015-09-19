#ifndef ENIGMA_IMAGE_CACHE_HH_INCLUDED
#define ENIGMA_IMAGE_CACHE_HH_INCLUDED

#include "ecl_cache.hh"
#include "ecl_video.hh"
#include <string>

namespace enigma {

class ImageCache : public ecl::PtrCache<ecl::Surface> {
public:
    using ecl::PtrCache<ecl::Surface>::store;

    ecl::Surface *acquire(const std::string &name) {
        return ecl::LoadImage(name.c_str());
    }
};

} // namespace enigma

#endif  // ENIGMA_IMAGE_CACHE_HH_INCLUDED
