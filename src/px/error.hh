/*
 * Copyright (C) 2002,2004 Daniel Heck
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
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: error.hh,v 1.4 2004/03/20 16:15:29 dheck Exp $
 */
#ifndef PX_ERROR_HH
#define PX_ERROR_HH

#include <string>
#include <exception>

namespace px
{
    using std::string;

    class XGeneric : public std::exception {
    public:
        // Constructor.
        XGeneric (const std::string& str = "")
        : m_string (str)
        {}
        virtual ~XGeneric() throw()
        {}
        
        // Accessors
        const string& get_string() const { return m_string; }
        const char *what() const throw() { return m_string.c_str(); }
    private:
        std::string m_string;
    };

#define PX_DEF_EXCEPTION(name, parent, message)                 \
    class name : public parent {                                \
    public:                                                     \
        name(const std::string &str = message) : parent(str) {} \
    }

    PX_DEF_EXCEPTION( XInputOutput,     XGeneric,       "InputOutput" );
    PX_DEF_EXCEPTION( XFileNotFound,    XInputOutput,   "File not found" );
    PX_DEF_EXCEPTION( XEndOfFile,       XInputOutput,   "End of file" );
    PX_DEF_EXCEPTION( XFileFormat,      XInputOutput,   "File format" );
    PX_DEF_EXCEPTION( XVideo,           XGeneric,       "Video");


    template <class EXC>
    void Assert (bool expr, const std::string &msg)
    {
        if (!expr)
            throw EXC(msg);
    }

    template <class EXC>
    void Assert (bool expr)
    {
        if (!expr)
            throw EXC();
    }
}
#endif
