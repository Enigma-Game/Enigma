/*
 * Copyright (C) 2002,2003 Daniel Heck
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
#ifndef ECL_CACHE_HH
#define ECL_CACHE_HH

#include <unordered_map>
#include <memory>

namespace ecl {

// A generic class for caching external data. Stored data is owned by the
// cache and is automatically deleted as required. Missing values are
// automatically created using the 'acquire' method.
template <class T>
class Cache {
public:
    Cache();
    virtual ~Cache() {}

    // ---------- Methods ----------
    void clear();
    T* get(const std::string &key);
    void remove(const std::string &key);
    unsigned size() const;

    T* store(const std::string &key, std::unique_ptr<T> value);

private:
    Cache(const Cache &other) = delete;
    Cache &operator=(const Cache &other) = delete;

    // ---------- Interface ----------
    virtual std::unique_ptr<T> acquire(const std::string &name) = 0;

    // ---------- Variables ----------
    std::unordered_map<std::string, std::unique_ptr<T>> cache;
};

template <class T>
Cache<T>::Cache() : cache(1223) {
}

template <class T>
void Cache<T>::clear() {
    cache.clear();
}

template <class T>
T *Cache<T>::get(const std::string &key) {
    auto i = cache.find(key);
    if (i != cache.end())
        return i->second.get();
    return store(key, acquire(key));
}

template <class T>
void Cache<T>::remove(const std::string &key) {
    cache.erase(key);
}

template <class T>
unsigned Cache<T>::size() const {
    return cache.size();
}

template <class T>
T *Cache<T>::store(const std::string &key, std::unique_ptr<T> value) {
    T *ptr = value.get();
    cache[key] = std::move(value);
    return ptr;
}

}  // namespace ecl

#endif
