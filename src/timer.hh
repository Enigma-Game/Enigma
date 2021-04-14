/*
 * Copyright (C) 2006 Daniel Heck
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
#ifndef ENIGMA_TIMER_HH
#define ENIGMA_TIMER_HH

#include "ecl_util.hh"
#include <memory>

namespace enigma {

/* Interface for time event handlers. */
class TimeHandler {
public:
    virtual ~TimeHandler() {}
    virtual void tick(double /*dtime*/) {}
    virtual void distinguished_alarm(int alarmnr) {}
    virtual void alarm() {}
};

/**
 * This class maintains a list of time handlers that are either
 * invoked at every tick or after a specified amount of time.  If
 * the TimeHandler is registered using #activate, it is invoked at
 * every tick, the #set_alarm method can be used to register a
 * time handler that is invoked (either once or repeatedly) after
 * a specified time interval. Each TimeHandler may request several
 * timers by choosing different alarmnr. This will be returned
 * via the TimeHandlers #alarm method.
 */
class Timer : public ecl::Nocopy {
public:
    Timer();
    ~Timer();

    void activate(TimeHandler *th);
    void set_alarm(TimeHandler *th, double interval, bool repeatp = false, int alarmnr = 0);
    double remove_alarm(TimeHandler *th, int alarmnr = 0);
    void clear();

    void tick(double dtime);

private:
    struct Rep;
    std::unique_ptr<Rep> self;
};

}  // namespace enigma

#endif  // ENIGMA_TIMER_HH
