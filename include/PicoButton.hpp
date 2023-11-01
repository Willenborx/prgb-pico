/**
 * @file PicoButton.hpp
 * @author Holger Willenborg (holger@willenb.org)
 * @brief Derive a class from this class and call @link PicoButtons::register_picobutton_interrupt @endlink to initiate the interrupt. The derived class
 *          overwrites on_press() and on_release() member functions which are called automatically
 * @version 0.5
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PRGB_PICOBUTTON_HPP
#define PRGB_PICOBUTTON_HPP

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include <map>

#include <TimeBase.hpp>

const uint32_t picobutton_bounce_time_us = 50000; // us

/**
 * @brief encapsulates the interrupt setup for a GPIO input and allows the use of a button without
 *        need to consider implementation details
 * 
 */
class PicoButton {
    bool pressed = false; // TODO set with initial state

    public:
        TimeMS time_pressed, time_released;

        virtual void event_callback(uint gpio, uint32_t events) {
            gpio_set_irq_enabled (gpio, events, false);

            TimeMS time = time_us_32(); 
            
            if ((pressed && (time > time_pressed + picobutton_bounce_time_us)) || (!pressed && (time > time_released+picobutton_bounce_time_us))) {

                if (events & GPIO_IRQ_EDGE_RISE) {
                    time_released = time;
                    this->pressed = false;
                    on_release();
                } 
                else
                if (events & GPIO_IRQ_EDGE_FALL) {            
                    time_pressed = time;
                    time_released = 0;
                    this->pressed = true;
                    on_press();
                }  
            }

            gpio_set_irq_enabled (gpio, events, true);            
        }
        virtual void on_press() {};
        virtual void on_release() {};
        bool is_pressed() const { return pressed; };
};

//inline std::map<uint, PicoButton *>PicoButtons::buttons = std::map<uint, PicoButton *>();

class PicoButtons {

    public:
        inline static std::map<uint, PicoButton *> buttons = std::map<uint, PicoButton *>();
    
        static void register_picobutton_interrupt(PicoButton *button, uint gpio, uint32_t events) {
            PicoButtons::buttons[gpio] = button;
            gpio_init(gpio);
            gpio_set_dir(gpio,GPIO_IN);
            gpio_set_irq_enabled_with_callback(gpio, events, true, &PicoButtons::gpio_callback); 
        }

        static void gpio_callback(uint gpio, uint32_t events) {    
            if (PicoButtons::buttons.find(gpio) != PicoButtons::buttons.end()) {
                PicoButtons::buttons[gpio]->event_callback(gpio, events);
            }
        }

};
#endif


