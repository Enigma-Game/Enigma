/*
 * Copyright (C) 2005 Ronald Lamprecht
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

#include "DOMErrorReporter.hh"
#include "main.hh"
#include "ecl_utf.hh"
#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XercesDefs.hpp>

using namespace ecl;
XERCES_CPP_NAMESPACE_USE

namespace enigma
{
    DOMErrorReporter::DOMErrorReporter() :
        sawErrors(false), severity(DOMError::DOM_SEVERITY_WARNING) {
    }
    
    DOMErrorReporter::~DOMErrorReporter() {
    }
    
    
    // ---------------------------------------------------------------------------
    //  DOMCountHandlers: Overrides of the DOM ErrorHandler interface
    // ---------------------------------------------------------------------------
    bool DOMErrorReporter::handleError(const DOMError& domError) {
        sawErrors = true;
        if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
            Log << "\nWarning at file ";
        else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR) {
            Log << "\nError at file ";
            if (severity == DOMError::DOM_SEVERITY_WARNING)
                severity = DOMError::DOM_SEVERITY_ERROR;
        }
        else {
            Log << "\nFatal Error at file ";
            severity = DOMError::DOM_SEVERITY_FATAL_ERROR;
        }
    
        Log << XML2Local(domError.getLocation()->getURI())
             << ", line " << domError.getLocation()->getLineNumber()
             << ", char " << domError.getLocation()->getColumnNumber()
             << "\n  Message: " << XML2Local(domError.getMessage()) << std::endl;
    
        // try to continue
        return true;
    }
    
    bool DOMErrorReporter::getSawErrors() const {
        return sawErrors;
    }
    
    DOMError::ErrorSeverity DOMErrorReporter::getSeverity() const {
        return severity;
    }
    
    void DOMErrorReporter::resetErrors() {
        sawErrors = false;
        severity = DOMError::DOM_SEVERITY_WARNING;
    }

} // namespace enigma

