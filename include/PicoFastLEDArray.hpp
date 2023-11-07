/**
 * @file PicoFastLEDArray.hpp
 * @author Holger Willenborg (holger@willenb.org)
 * @brief 
 * @version 0.1
 * @date 2023-11-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#define ARDUINO_ARCH_RP2040

#include <TimeBase.hpp>
#include <Geometry.hpp>
#include <LightArray.hpp>
#include <pico/time.h>
#include <hardware/timer.h>

#define micros() time_us_64()

#include <FastLED.h>

using namespace prgb;



class PicoFastLEDArray : public LightArray {
    int gpio_pin;
    int16_t brightness;
    bool use_gamma;

    public:
        PicoFastLEDArray(int gpio_pin, int16_t brightness, Geometry& geometry, TimeBase& tb, bool use_gamma) 
          : LightArray(geometry,tb), gpio_pin(gpio_pin), brightness(brightness), use_gamma(use_gamma) {};
        
        virtual void start_display(uint16_t rate) {
            /// \todo obviously
            
            
        }

        virtual void show() {
            /// \todo obviously
        }

};