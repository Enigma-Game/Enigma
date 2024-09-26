/*
 * Copyright (C) 2002,2004 Daniel Heck
 * Copyright (C) 2007 Ronald Lamprecht
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
#ifndef ENIGMA_HH
#define ENIGMA_HH

// This file contains declarations for facilities used by several different
// parts of the program, like common types and constants.

#include "ecl_math.hh"
#include <utility>
#include <stdint.h>
#include <string>

#define NUMENTRIES(array) (sizeof(array) / sizeof(*array))

namespace enigma {

enum Player { YIN = 0, YANG = 1 };

enum Color { BLACK = 0, WHITE = 1, GLASS = 2, BLUE = 3, YELLOW = 4 };

/* -------------------- Various types -------------------- */

enum Difficulty { DIFFICULTY_EASY = 1, DIFFICULTY_HARD = 2, DIFFICULTY_ANY = 3 };

// Enigma can run its own levels but also emulate various versions of Oxyd.
// All these games behave similarly, but there are a few differences in object
// behaviour, visual appearance, etc.
enum GameType {
    GAMET_FIRST,

    GAMET_ENIGMA = GAMET_FIRST,
    GAMET_OXYD1,
    GAMET_PEROXYD,
    GAMET_OXYDEXTRA,
    GAMET_OXYDMAGNUM,

    GAMET_LAST = GAMET_OXYDMAGNUM,
    GAMET_COUNT,
    GAMET_UNKNOWN
};

GameType GetGameType(const std::string &name);
std::string GetGameTypeName(GameType type);

/* -------------------- Direction, DirectionBits -------------------- */

enum Direction {
    NODIR = -1,
    WEST = 0,
    SOUTH = 1,
    EAST = 2,
    NORTH = 3,
};

Direction reverse(Direction d);
Direction rotate_cw(Direction d);
Direction rotate_ccw(Direction d);
Direction next(Direction d);
Direction previous(Direction d);

std::string to_suffix(Direction d);
std::string toSuffix(Direction d);

enum DirectionBits {
    NODIRBIT = 0,
    WESTBIT = 1 << WEST,
    SOUTHBIT = 1 << SOUTH,
    EASTBIT = 1 << EAST,
    NORTHBIT = 1 << NORTH,
    ALL_DIRECTIONS = 15
};

DirectionBits rotate(DirectionBits d, bool clockwise);
DirectionBits to_bits(Direction d);
bool has_dir(DirectionBits db, Direction dir);
DirectionBits reverse(DirectionBits dirs);
DirectionBits added_dirs(DirectionBits oldDirs, DirectionBits newDirs);
DirectionBits removed_dirs(DirectionBits oldDirs, DirectionBits newDirs);

/* -------------------- GridPos -------------------- */

struct GridPos {
    // Variables
    int x, y;

    // Methods
    explicit GridPos(int xx = 0, int yy = 0);
    explicit GridPos(const ecl::V2 &p);
    void move(Direction dir);
    ecl::V2 center() const;

    /**
     * Conversion of a value to a position vector.
     */
    operator ecl::V2() const;
};

GridPos move(GridPos p, Direction dir);
GridPos move(GridPos p, Direction dir, Direction dir2);
bool operator==(GridPos a, GridPos b);
bool operator!=(GridPos a, GridPos b);
bool operator<(GridPos a, GridPos b);

/* 516
   203
   748 */
GridPos get_neighbour(GridPos p, int i);

// source and target have to be adjacent
Direction direction_fromto(GridPos source, GridPos target);

std::ostream &operator<<(std::ostream &os, const GridPos &val);

// ---------- GridLayer ----------

enum GridLayer { GRID_FLOOR, GRID_ITEMS, GRID_STONES, GRID_COUNT };

enum GridLayerBits {
    GRID_NONE_BIT = 0,
    GRID_FLOOR_BIT = 1,
    GRID_ITEMS_BIT = 2,
    GRID_STONES_BIT = 4
};

// ---------- GridLoc ----------

struct GridLoc {
    // Variables
    GridPos pos;
    GridLayer layer;

    // Constructor
    GridLoc(GridLayer l = GRID_FLOOR, GridPos p = GridPos());
};

/*! Converts strings like "it(10 10)", "st(5 2)" to GridLoc
  structures. */
bool to_gridloc(const char *str, GridLoc &loc);

/* -------------------- Random Numbers -------------------- */

/**
 * Maximum random number 2^31-1 that is valid for all operating systems
 * and processor types.
 */
#define ENIGMA_RAND_MAX 2147483647

extern int32_t SystemRandomState;  ///< the internal seed used for system demands

/**
 * Initialization of the random generator by a random seed.
 * @arg isLevel   random seed for the calculation of the level itself
 */
void Randomize(bool isLevel = true);

/**
 * Initialization of the random generator by a given seed. This function
 * allows a reset of the level random sequence for replay of a level or
 * calculation of parallel worlds.
 * @arg isLevel   random seed for the calculation of the level itself
 */
void Randomize(unsigned seed, bool isLevel = true);

/**
 * Centralized mandatory random function that substitutes any operating
 * system dependent std::rand() implementation. Enigma needs random
 * number sequences that are repeatable and identical on all computers
 * without any special demands on the distribution. This is a fast
 * implementation of the classical BSD rand function that generates 31 bit
 * integer random numbers with the number itself being the seed for the
 * next number. All random functions take a bool argument, that seperates
 * random requests based on commmon system demands from those based on
 * the calculation of the level. The related seed for the level random
 * requests is kept as part of the level thus allowing parallel calculations
 * of different worlds in different threads. All random numbers, even those
 * generated in the Lua random functions need to be based on this central function.
 * @arg isLevel   random request for the calculation of the level itself
 * @return        random integer in the range 0 - ENIGMA_RAND_MAX
 */
int Rand(bool isLevel = true);

/**
 * Random integer number that is guaranteed to be in the range [min, max]. See
 * Rand() for details.
 * @arg  min      lower included boundary of number range
 * @arg  max      upper included boundary of number range
 * @arg  isLevel  random request for the calculation of the level itself
 * @return        random integer in the range [min, max]
 */
int IntegerRand(int min, int max, bool isLevel = true);

/**
 * Random integer number that is guaranteed to be in the range [min, max]. See
 * Rand() for details.
 * @arg  min      lower included boundary of number range
 * @arg  max      upper included boundary of number range
 * @arg  isLevel  random request for the calculation of the level itself
 * @return        random integer in the range [min, max]
 */
double DoubleRand(double min, double max, bool isLevel = true);

/* -------------------- Time & Date -------------------- */

const char *date(const char *format);  // format see 'man strftime'

/* ==================== Inline definitions ==================== */

inline DirectionBits to_bits(Direction d) {
    if (d == NODIR)
        return NODIRBIT;
    return DirectionBits(1 << d);
}

inline bool has_dir(DirectionBits db, Direction dir) {
    return (db & to_bits(dir)) != 0;
}

inline DirectionBits reverse(DirectionBits dirs) {
    return (DirectionBits)(~dirs & ALL_DIRECTIONS);
}

inline DirectionBits added_dirs(DirectionBits oldDirs, DirectionBits newDirs) {
    return (DirectionBits)(~oldDirs & newDirs);
}

inline DirectionBits removed_dirs(DirectionBits oldDirs, DirectionBits newDirs) {
    return (DirectionBits)(oldDirs & ~newDirs);
}

// ---------- GridPos ----------

inline GridPos::GridPos(int xx, int yy) : x(xx), y(yy) {
}

inline void GridPos::move(Direction dir) {
    switch (dir) {
    case NORTH: y--; break;
    case SOUTH: y++; break;
    case EAST: x++; break;
    case WEST: x--; break;
    case NODIR: break;
    }
}

inline ecl::V2 GridPos::center() const {
    return ecl::V2(x + .5, y + .5);
}

inline GridPos move(GridPos p, Direction dir) {
    GridPos tmp = p;
    tmp.move(dir);
    return tmp;
}

inline GridPos move(GridPos p, Direction dir, Direction dir2) {
    GridPos tmp = p;
    tmp.move(dir);
    tmp.move(dir2);
    return tmp;
}

inline bool operator==(GridPos a, GridPos b) {
    return (a.x == b.x && a.y == b.y);
}

inline bool operator!=(GridPos a, GridPos b) {
    return (a.x != b.x || a.y != b.y);
}

inline bool operator<(GridPos a, GridPos b) {
    return ((a.y << 16) + a.x) < ((b.y << 16) + b.x);
}

// ---------- GridLoc ----------

inline GridLoc::GridLoc(GridLayer l, GridPos p) : pos(std::move(p)), layer(l) {
}

}  // namespace enigma

#endif
