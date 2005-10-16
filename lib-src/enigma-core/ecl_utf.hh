/**
 * @file ecl_utf.hh Several helper classes and functions for coding convertions
 *                  between XMLCh * (utf-16), char * (utf-8, local code page)
 *                  and String (utf-8, local code page).
 */

#include <iostream>
#include <xercesc/util/XMLString.hpp>


namespace ecl
{
    /**
     * Transcoding utility for XMLCh strings to local code page strings.
     * Provides a simple interface for the memory management paradigm shift
     * even though not terribly efficient. Make sure all XML2Local objects
     * exist only temporarily, f.e. 
     * <code>new String(XML2Local(XMLCh *toTranscode).c_str())</code> or
     * <code>Log << XML2Local(XMLCh *toTranscode)</code>
     */
    class XML2Local {
    public :
        /**
         * Makes a transcoding to the local code page.
         *
         * @param toTranscode XML managed string
         */
        XML2Local(const XMLCh* const toTranscode);
        ~XML2Local();
    
        /**
         * Returns the string coded in the local page.  It remains owner of
         * the string.
         */
        const char* c_str() const;
    
    private :
        /**
         * A copy coded in the local code page.
         * The string is managed by XMLString - we are the owner.
         */
        char*   localString;
    };
    
    /**
     * Enables efficient stream output of XMLCh * strings. Use it as
     * <code>Log << XML2Local(XMLCh *toTranscode)</code>. All resources
     * are managed and released.
     */
    inline std::ostream& operator<<(std::ostream& target, const XML2Local& toDump) {
        target << toDump.c_str();
        return target;
    }
} //namespace ecl
