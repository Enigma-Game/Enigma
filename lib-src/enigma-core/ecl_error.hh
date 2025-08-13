/*
 * Copyright (C) 2002,2004,2005,2006 Daniel Heck
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
#ifndef ECL_ERROR_HH
#define ECL_ERROR_HH

#include "ecl_util.hh"

#include <string>
#include <exception>

#ifdef ENABLE_ASSERT
#define ASSERT(p, t, m)                                                                           \
    ((p) ? (void)0 : ecl::Assert<t>(false, ecl::strf("Assert(%s) at %s:%d: %s() failed - %s", #p, \
                                                     __FILE__, __LINE__, __func__, m)))
#else
#define ASSERT(p, t, m) ((void)0)
#endif

namespace ecl {

class XGeneric : public std::exception {
public:
    // Constructor.
    XGeneric(std::string str = "") : m_string(std::move(str)) {}
    virtual ~XGeneric() throw() {}

    // Accessors
    const std::string &get_string() const { return m_string; }
    const char *what() const throw() override { return m_string.c_str(); }

private:
    std::string m_string;
};

#define ECL_DEF_EXCEPTION(name, parent, message)                \
    class name : public parent {                                \
    public:                                                     \
        name(const std::string &str = message) : parent(str) {} \
    }

ECL_DEF_EXCEPTION(XInputOutput, XGeneric, "InputOutput");
ECL_DEF_EXCEPTION(XFileNotFound, XInputOutput, "File not found");
ECL_DEF_EXCEPTION(XEndOfFile, XInputOutput, "End of file");
ECL_DEF_EXCEPTION(XFileFormat, XInputOutput, "File format");
ECL_DEF_EXCEPTION(XVideo, XGeneric, "Video");

class XSDLError : public ecl::XGeneric {
public:
    XSDLError(const std::string &msg) : XGeneric(msg) {}
};

template <class EXC>
void Assert(bool expr, const std::string &msg) {
    if (!expr)
        throw EXC(msg);
}

template <class EXC>
void Assert(bool expr) {
    if (!expr)
        throw EXC();
}

}  // namespace ecl

#endif
