
#include <list>
#include <algorithm>
#include <functional>

#include "ecl_util.hh"

#include "util.hh"


using namespace enigma::util;
using namespace std;


/* -------------------- Alarm -------------------- */

namespace
{
    class Alarm {
    public:
        Alarm(TimeHandler* h, double interval, bool repeatp);
        void tick(double dtime);
        bool expired() const { return removed || timeleft <= 0; }

        void remove() { 
            removed = true; 
        }

        bool operator==(const Alarm &a) const {
            return a.handler == handler;
        }
    private:
        // Variables
        TimeHandler *handler;
        double       interval;
        double       timeleft;
        bool         repeatp;
        bool         removed;
    };
}

Alarm::Alarm(TimeHandler* h, double i, bool r)
: handler(h), interval(i), timeleft(i), repeatp(r), removed(false)
{}

void Alarm::tick (double dtime) 
{
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


/* -------------------- Timer implementation -------------------- */

struct Timer::Rep {
    std::list<TimeHandler*>  handlers;
    std::list<Alarm>         alarms;
};


Timer::Timer() : self(*new Rep) 
{
}

Timer::~Timer() 
{
    clear();
    delete &self;
}

void Timer::deactivate(TimeHandler* th) 
{
    std::list<TimeHandler*>::iterator i;

    i = find(self.handlers.begin(), self.handlers.end(), th);
    if (i != self.handlers.end()) {
        *i = 0;
    }
    //    self.handlers.remove(th);
    // This doesn't work because some objects deactivate themselves
    // from their `tick' method!
}

void Timer::activate(TimeHandler *th) 
{
    if (find(self.handlers.begin(), self.handlers.end(), th) == self.handlers.end())
        self.handlers.push_back(th);
}

void Timer::set_alarm(TimeHandler *th, double interval, bool repeatp) 
{
    if (interval > 0)
        self.alarms.push_back(Alarm(th, interval, repeatp));
}

void Timer::remove_alarm(TimeHandler *th) {
    // does not work (crashes if alarm_n removed alarm_n+1)
    // self.alarms.remove(Alarm(th,0,0));

    list<Alarm>::iterator e = self.alarms.end();
    // only the ``handler'' entries are compared:
    list<Alarm>::iterator a = find(self.alarms.begin(), e, Alarm(th, 0, 0));
    if (a != e)
        a->remove(); // only mark for removal!
}


void Timer::tick (double dtime) 
{
    self.handlers.remove(0);     // remove inactive entries
    for_each(self.handlers.begin(), self.handlers.end(),
             std::bind2nd (std::mem_fun (&TimeHandler::tick), dtime));

    // explicit loop to allow remove_alarm() to be called from inside alarm()
    for (list<Alarm>::iterator i=self.alarms.begin(); i != self.alarms.end(); ) {
        list<Alarm>::iterator n = ecl::next(i);

        i->tick(dtime);
        i = n;
    }
    self.alarms.remove_if (mem_fun_ref(&Alarm::expired));
}


void Timer::clear() 
{
    self.handlers.clear();
    self.alarms.clear();
}
