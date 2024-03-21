/**
 * @file W2812LightArray.cpp
 * @author Holger Willenborg (holger@willenb.org)
 * @brief Implementation of an WS2812B LED strip
 * @version 0.5
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "W2812Array.hpp"
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include <stdio.h>
#include <Log.hpp>

namespace prgb {
    const int PICO_MODE_RGB = 1;
    const int PICO_MODE_RGBW = 2;
}

repeating_timer_t W2812LightArray::timer_repeating;
W2812LightArray *W2812LightArray::array;

//! number of LEDs and output pin on the raspberry pi pico
W2812LightArray::W2812LightArray(uint8_t newoutpin, uint8_t brightness, prgb::Geometry& geometry, TimeBase& tb, bool usegamma) : prgb::LightArray(geometry,tb) {
    pin = newoutpin;
    pio = pio0;
    statemachine = 0;
    this->usegamma = usegamma;
    W2812LightArray::array = this;

    this->brightness = brightness;
    offset = pio_add_program(pio,&ws2812_program);
    ws2812_program_init(pio,statemachine,offset,pin,800000,false);

    LOG("WS2812B initialization done");    
}

//! RGB needs to be converted to match the WS2812 order and to get some Gamma corection to make the colours looking more natural
inline ColorValue W2812LightArray::convert_pixel(ColorValue pixel) {

    uint ur = (uint)(R(pixel));
    uint ug = (uint)(G(pixel));
    uint ub = (uint)(B(pixel));

    uint r =  static_cast<uint>(ur * brightness/100);
    uint g =  static_cast<uint>(ug * brightness/100);
    uint b =  static_cast<uint>(ub * brightness/100);

    // 0xGGRRBBww
    if (usegamma) 
        return (( gamma8[r>255?255:r] << 16) + (gamma8[b>255?255:b] << 8) + (gamma8[g>255?255:g]<<24));
    else
        return (((r>255?255:r)<<16) + ((g>255?255:g)<<24) + ((b>255?255:b)<<8)); // tolerate out-of-range color values and cut down to 255. So you can use a brightness value > 100 to make the output brighter.

}

//! transmits the pixel data to the hardware
void W2812LightArray::show() {
   
    int dspbuffer = (currentbuffer == 0) ? 1 : 0;
    
    for (int i=0; i < this->length; i++) {
        pio_sm_put_blocking(this->pio, statemachine, (uint32_t) convert_pixel(this->pixels[dspbuffer][i]));
    }
    
}

//! Set brightness using a value between 0-100
void W2812LightArray::set_brightness(uint8_t newbrightness) {
    this->brightness = newbrightness;
}

void W2812LightArray::start_display(uint16_t rate) {
    add_repeating_timer_ms((int32_t) (-1000/rate),W2812LightArray::callback_display_timer,NULL,&W2812LightArray::timer_repeating);
}

bool W2812LightArray::callback_display_timer(repeating_timer_t *rt) {   
    if (W2812LightArray::array) W2812LightArray::array->output();
    return (true);
}
