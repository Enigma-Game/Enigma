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
#include "timer.hh"

#include <list>
#include <algorithm>
#include <functional>

#include "ecl_util.hh"
#include "errors.hh"

namespace enigma {

/* -------------------- Alarm -------------------- */

namespace {

// This class helps Timer keep track of the current list of registered
// TimeHandlers.
class Alarm {
public:
    Alarm(enigma::TimeHandler *h, double interval, bool repeatp);
    void tick(double dtime);
    bool expired() const;
    void mark_removed();
    bool has_handler(enigma::TimeHandler *th) const;

    // Variables
    enigma::TimeHandler *handler;
    double interval;
    double timeleft;
    bool repeatp;
    bool removed;
};

Alarm::Alarm(TimeHandler *h, double i, bool r)
: handler(h), interval(i), timeleft(i), repeatp(r), removed(false) {
}

bool Alarm::expired() const {
    return removed || timeleft <= 0;
}

void Alarm::mark_removed() {
    removed = true;
}

void Alarm::tick(double dtime) {
    if (!removed) {
        timeleft -= dtime;
        if (repeatp) {
            while (timeleft <= 0) {
                handler->alarm();
                timeleft += interval;
            }
        } else if (timeleft <= 0) {
            handler->alarm();
        }
    }
}

bool Alarm::has_handler(TimeHandler *th) const {
    return handler == th;
}

} // namespace

/* -------------------- Timer implementation -------------------- */

struct Timer::Rep {
    std::list<TimeHandler *> handlers;
    std::list<Alarm> alarms;
};

Timer::Timer() : self(*new Rep) {
}

Timer::~Timer() {
    clear();
    delete &self;
}

void Timer::deactivate(TimeHandler *th) {
    std::list<TimeHandler *>::iterator i;

    i = find(self.handlers.begin(), self.handlers.end(), th);
    if (i != self.handlers.end()) {
        *i = 0;
    }
}

void Timer::activate(TimeHandler *th) {
    if (find(self.handlers.begin(), self.handlers.end(), th) == self.handlers.end())
        self.handlers.push_back(th);
}

void Timer::set_alarm(TimeHandler *th, double interval, bool repeatp) {
    ASSERT(interval > 0, XLevelRuntime, "Timer error: timer with interval <= 0 seconds");
    ASSERT(!repeatp || interval >= 0.01, XLevelRuntime,
           "Timer error: looping timer with interval < 0.01 seconds");
    if (interval > 0)
        self.alarms.push_back(Alarm(th, interval, repeatp));
}

double Timer::remove_alarm(TimeHandler *th) {
    double timeleft = 0;
    std::list<Alarm>::iterator e = self.alarms.end();
    for (std::list<Alarm>::iterator it = self.alarms.begin(); it != e; ++it) {
        if (it->has_handler(th)) {
            it->mark_removed();
            timeleft = it->timeleft;
        }
    }
    return timeleft;
}

void Timer::tick(double dtime) {
    self.handlers.remove(0);  // remove inactive entries
    for_each(self.handlers.begin(), self.handlers.end(),
             std::bind2nd(std::mem_fun(&TimeHandler::tick), dtime));

    // Explicit loop to allow remove_alarm() to be called from inside alarm()
    for (std::list<Alarm>::iterator i = self.alarms.begin(); i != self.alarms.end();) {
        std::list<Alarm>::iterator next = i;
        ++next;
        i->tick(dtime);
        i = next;
    }
    self.alarms.remove_if(std::mem_fun_ref(&Alarm::expired));
}

void Timer::clear() {
    self.handlers.clear();
    self.alarms.clear();
}

}  // namespace enigma
