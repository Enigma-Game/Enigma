#include "ecl_utf.hh"


XERCES_CPP_NAMESPACE_USE

namespace ecl
{
    XML2Local::XML2Local(const XMLCh* const toTranscode) {
        // Use XML for transcoding -- the returned string is owned by us
        // but managed by XMLString!
        localString = XERCES_CPP_NAMESPACE_QUALIFIER 
                XMLString::transcode(toTranscode);
    }

    XML2Local::~XML2Local() {
        XERCES_CPP_NAMESPACE_QUALIFIER XMLString::release(&localString);
    }

    const char* XML2Local::c_str() const {
        return localString;
    }
    
} //namespace ecl
