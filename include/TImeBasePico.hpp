/**
 * @file TImeBasePico.hpp
 * @author Holger Willenborg (holger@willenb.org)
 * @brief Implementation of the TimeBase for the Raspberry Pi Pixo
 * @version 0.5
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
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