/** 
 * \todo Descibr PicoTimeBase
*/

#ifndef PRGB_TIMEBASEPICO_HPP
#define PRGB_TIMEBASEPICO_HPP

#include <TimeBase.hpp>
#include <cstdint>

namespace pico {

    using namespace prgb;

    class TimeBasePico : public TimeBase {
        TimeMS start;

        public:
            TimeBasePico() { reset(); };
            void reset() { start = time_us_64() / 1000;};
            TimeMS get_deltatime_ms() {return (time_us_64()/1000)- start; };
    } ;
};

#endif