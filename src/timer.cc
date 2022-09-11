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
    Alarm(enigma::TimeHandler *h, double interval, bool repeatp, int alarmnr);
    void tick(double dtime);
    bool expired() const;
    void mark_removed();
    bool has_handler(enigma::TimeHandler *th, int n) const;

    // Variables
    enigma::TimeHandler *handler;
    double interval;
    double timeleft;
    bool repeatp;
    bool removed;
    int alarmnr;
};

Alarm::Alarm(TimeHandler *h, double i, bool r, int n)
: handler(h), interval(i), timeleft(i), repeatp(r), alarmnr(n), removed(false) {
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
                handler->distinguished_alarm(alarmnr);
                handler->alarm();
                timeleft += interval;
            }
        } else if (timeleft <= 0) {
            handler->distinguished_alarm(alarmnr);
            handler->alarm();
        }
    }
}

bool Alarm::has_handler(TimeHandler *th, int n) const {
    return (handler == th) && (alarmnr == n);
}

} // namespace

/* -------------------- Timer implementation -------------------- */

struct Timer::Rep {
    std::list<TimeHandler *> handlers;
    std::list<Alarm> alarms;
};

Timer::Timer() : self(new Rep) {
}

Timer::~Timer() = default;

void Timer::activate(TimeHandler *th) {
    if (find(self->handlers.begin(), self->handlers.end(), th) == self->handlers.end())
        self->handlers.push_back(th);
}

void Timer::set_alarm(TimeHandler *th, double interval, bool repeatp, int alarmnr) {
    ASSERT(interval > 0, XLevelRuntime, "Timer error: timer with interval <= 0 seconds");
    ASSERT(!repeatp || interval >= 0.01, XLevelRuntime,
           "Timer error: looping timer with interval < 0.01 seconds");
    if (interval > 0)
        self->alarms.push_back(Alarm(th, interval, repeatp, alarmnr));
}

double Timer::remove_alarm(TimeHandler *th, int alarmnr) {
    double timeleft = 0;
    for (auto &alarm : self->alarms) {
        if (alarm.has_handler(th, alarmnr)) {
            alarm.mark_removed();
            timeleft = alarm.timeleft;
        }
    }
    return timeleft;
}

void Timer::tick(double dtime) {
    self->handlers.remove(nullptr);  // remove inactive entries
    for (auto &handler : self->handlers)
        handler->tick(dtime);

    // Explicit loop to allow remove_alarm() to be called from inside alarm()
    for (auto i = self->alarms.begin(); i != self->alarms.end();) {
        auto next = i;
        ++next;
        i->tick(dtime);
        i = next;
    }
    self->alarms.remove_if(std::mem_fn(&Alarm::expired));
}

void Timer::clear() {
    self->handlers.clear();
    self->alarms.clear();
}

}  // namespace enigma
