#include "enigma.hh"

namespace enigma_util
{
    using namespace enigma;


    class Timer : public ecl::Nocopy {
    public:
        Timer();
        ~Timer();
        void activate(TimeHandler *th);
        void deactivate(TimeHandler* th);
        void set_alarm(TimeHandler* th, double interval, bool repeatp = false);
        void remove_alarm(TimeHandler *cb);
        void clear();

        void tick(double dtime);
    private:
        struct Rep;
        Rep &self;
    };
}

namespace enigma
{
    namespace util = enigma_util;
}
