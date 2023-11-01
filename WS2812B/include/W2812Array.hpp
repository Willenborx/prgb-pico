/**
 * @file W2812LightArray.hpp
 * @author Holger Willenborg (holger@willenb.org)
 * @brief Implementation of a WS2812B LED strip control with the Raspberry Pi Pico
 * @version 0.5
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 */

#ifndef PRGB_W2812ARRAY_HPP
#define PRGB_W2812ARRAY_HPP

#include <prgb.hpp>
//#include <hardware/clocks.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>

using namespace prgb;

/**
 * @brief Implements a hardware array of the WS2812B programmed using the Pi Pico State Machines. Also uses the Pi Pico timers to implement
 *        the timed call to update the LEDs regularly (according to the rate). This also implements a Gamma correction of the output value.
 *        The class maintains a "brightness" member which may be modified during runtime and initially be passed to the constructor. Before sent to display
 *        all RGB values will be sent through @link convert_pixel @endlink to apply gamma and brightness correction. 
 */
class W2812LightArray : public LightArray {
    //! Output Pin number
    int pin;
    PIO pio;

    //! used for the transmission of the data to the WS2812B circuits
    int statemachine;
    uint offset;
    uint brightness;
    bool usegamma;

    //! transfer the data to the LED strip
    virtual void show();

    public:
        //! callback must be static
        static repeating_timer_t timer_repeating;
        static W2812LightArray *array;


        //! handover length and output pin number / brightness 0..99
        W2812LightArray(uint8_t outpin, uint8_t brightness, prgb::Geometry& geometry, TimeBase& tb, bool usegamma = true);

        //! WS2812B uses GRB byte order so every pixel color must be converted from RGB, in addition the Gamma correction will be done here
        ColorValue inline convertPixel(ColorValue pixel);
        
        //! Set to a value from 0-100 to adjust allover brightness. May result in darkness for low values (see Gamma correction)
        void set_brightness(uint8_t newbrightness);
        
        virtual void start_display(uint16_t rate);

        //! static callback_display_timer()
        static bool callback_display_timer(repeating_timer *rt);

};

//! Gamma array taken from https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
const uint8_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

  #endif