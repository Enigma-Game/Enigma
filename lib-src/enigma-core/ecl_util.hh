/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
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
#ifndef ECL_UTIL_HH_INCLUDED
#define ECL_UTIL_HH_INCLUDED

#include <string>
#include <algorithm>
#include <cstdint>

/* hide GNU extensions for non-gnu compilers: */
#ifndef __GNU__
#ifndef __attribute__
// #  define __attribute__(x)
// # error wuah
#endif
#endif

namespace ecl {

// Base class for classes that may not be copied, neither via copy constructor
// or assignment operator.
class Nocopy {
public:
    Nocopy() {}

private:
    // Not implemented to cause compiler error message on usage.
    Nocopy(const Nocopy &);
    Nocopy &operator=(const Nocopy &);
};

template <class For>
inline void delete_sequence(For begin, For end) {
    while (begin != end)
        delete *begin++;
}

template <class For>
inline void delete_map(For begin, For end) {
    while (begin != end) {
        delete begin->second;
        ++begin;
    }
}

template <class T>
inline const T &Max(const T &a, const T &b) {
    return (a > b) ? a : b;
}

template <class T>
inline const T &Min(const T &a, const T &b) {
    return (a < b) ? a : b;
}

template <class T>
inline const T &Clamp(const T &a, const T &min, const T &max) {
    if (a < min)
        return min;
    else if (a > max)
        return max;
    else
        return a;
}

template <class T>
inline const bool isOrdered(const T &a, const T &b, const T &c) {
    return (a <= b) && (b <= c);
}

// Split a string at every occurrence of 'sep' and store the resulting strings
// using the iterator 'out'.
//
// Example:
//     vector<string> vs;
//     split_copy("/usr/bin/enigma", '/', back_inserter(vs));
template <class Ch, class OutIt>
void split_copy(const std::basic_string<Ch> &str, Ch sep, OutIt out) {
    typename std::basic_string<Ch>::const_iterator i, j;
    i = str.begin();
    while (true) {
        j = std::find(i, str.end(), sep);
        *out = std::basic_string<Ch>(i, j);
        ++out;
        if (j == str.end())
            break;
        i = j + 1;
    }
}

template <class Ch, class OutIt>
size_t split_copy_n(const std::basic_string<Ch> &str, Ch sep, OutIt out, size_t n) {
    typename std::basic_string<Ch>::const_iterator i, j;
    i = str.begin();
    size_t cnt = 0;
    for (; cnt < n; ++cnt) {
        j = std::find(i, str.end(), sep);
        *out = std::basic_string<Ch>(i, j);
        ++out;
        if (j == str.end())
            break;
        i = j + 1;
    }
    return cnt;
}

// Remove white space at the beginning and end of a string.
template <class Str>
Str trim(const Str &str) {
    if (str.size() == 0)
        return str;
    typename Str::size_type b = str.find_first_not_of(" \t\n\r");
    typename Str::size_type e = str.find_last_not_of(" \t\n\r");
    if (b == Str::npos)
        return "";
    return Str(str, b, e - b + 1);
}

//
// Some helper functions for dealing with bit flags.
//

template <class E, class I>
inline void set_flags(E &val, I flags) {
    val = static_cast<E>(val | flags);
}

template <class E, class I>
inline void clear_flags(E &val, I flags) {
    val = static_cast<E>(val & ~flags);
}

template <class E, class I>
inline void toggle_flags(E &val, I flags) {
    val = static_cast<E>(val ^ flags);
}

template <class E, class I>
inline bool has_flags(E val, I flags) {
    return (static_cast<int>(val & flags) == static_cast<int>(flags));
}

//
// Some helper functions for strings and filenames.
//

std::string strf(const char *format, ...) __attribute__((format(printf, 1, 2)));
std::string timeformat(int duration);

std::string concat_paths(const std::string &path, const std::string &filename);

/**
 * Split a given path into the leading directory part and the last filename
 * part. The path may use unix slash separators as well as windows backlashes.
 * Even mixed usage is allowed. Trailing path separators are skipped.
 * @param path           the path to be split
 * @param dir_part       ptr to a return string where the leading directory
 *                       part without trailing separators will be stored.
 *                       The string will not be set if the result is false.
 * @param filename_part  ptr to a return string where the trailing filename
 *                       part will be stored.
 *                       The string will not be set if the result is false.
 * @return               success of splitting the path into 2 parts. If no
 *                       path separator was found just false will be returned
 *                       without modification of the return strings.
 */
bool split_path(const std::string &path, std::string *dir_part, std::string *filename_part);

/**
 * Check a string against a given template that may contain wildcards of '*' and
 * '?'. A '?' in the template means 'any single character'. A '*' means '0 or more characters'.
 * @param str     string to be analysed
 * @param templ   template with wildcards
 * @return        true if string matches template, false otherwise
 */
bool string_match(const char *str, const char *templ);

/**
 * Check a string against a given template that may contain wildcards of '*' and
 * '?'. A '?' in the template means 'any single character'. A '*' means '0 or more characters'.
 * @param str     string to be analysed
 * @param templ   template with wildcards
 * @return        true if string matches template, false otherwise
 */
bool string_match(std::string str, std::string templ);

/**
 * Take an unsigned long (32 bit) integer x and convert it into a string of 4 unsigned chars.
 */
std::string uint32_to_string(uint32_t x);

/**
 * Take a std::string of length 4 or more. Interpret the first four chars as bytes and convert
 * them into an unsigned long (32 bit) integer. If there are less than four chars, the higher
 * value bytes will be interpreted as null.
 */
uint32_t string_to_uint32(std::string s);
uint32_t chars_to_uint32(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3);

}  // namespace ecl

#endif
