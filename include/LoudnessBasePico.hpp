/**
 * @file LoudnessBasePico.hpp
 * @author Holger Willenborg (holger@willenb.org)
 * @brief Implementation of LoudnessBase for Pico
 * 
 * @version 0.5
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LoudnessBasePico_hpp
#define LoudnessBasePico_hpp

#include <pico/stdlib.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include <pico/multicore.h>
#include <LoudnessBase.hpp>
#include <stdint.h>

namespace pico {

    using namespace prgb;

    /**
     * @brief This class implements the loudness source to be one of the ADCs (0..2). 
     *        It was tested with a electret microphone amplifier module max4466
     */
    class LoudnessBasePico : public LoudnessBase {
        public:
            inline static LoudnessBasePico *loudness_base_pico_instance = nullptr;

            // \todo ugly: factory with default parameter: First call must have the correct adc_num, this also limits
            // the system to use only one loudness base at a time.
            static LoudnessBasePico *get_instance(uint8_t adc_num = 0) {
                return ((loudness_base_pico_instance == nullptr) ? new LoudnessBasePico(adc_num) : loudness_base_pico_instance); 
                
            }

            LoudnessBasePico(uint8_t adc_num) : LoudnessBase(), adc_num(adc_num), adc_pin(adc_num+26)  {

                //! Prepare Pico

                // Reduce noise by setting GPIO23 to high, increases power consumption
                gpio_set_dir(23,true);
                gpio_put(23,true);

                adc_gpio_init(adc_pin);

                adc_init();
                adc_select_input(adc_num);
                adc_fifo_setup(
                        true,    // Write each completed conversion to the sample FIFO
                        true,    // Enable DMA data request (DREQ)
                        1,       // DREQ (and IRQ) asserted when at least 1 sample present
                        false,   // We won't see the ERR bit because of 8 bit reads; disable.
                        true     // Shift each sample to 8 bits when pushing to FIFO
                        );

                // set sample rate
                adc_set_clkdiv((uint32_t)(48000000/sample_frequency));

                sleep_us(200);

                // Set up the DMA to start transferring data as soon as it appears in FIFO
                dma_channel = dma_claim_unused_channel(true);
                this->dma_config = dma_channel_get_default_config(dma_channel);

                // Reading from constant address, writing to incrementing byte addresses
                channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_8);
                channel_config_set_read_increment(&dma_config, false);
                channel_config_set_write_increment(&dma_config, true);

                // Pace transfers based on availability of ADC samples
                channel_config_set_dreq(&dma_config, DREQ_ADC);

                // blink on every sample -> set direction of Pin 25 (on-board LED)
                gpio_init(25);
                gpio_set_dir(25,GPIO_OUT);

                // launch setup_sampler from other core so the invoked timer also uses the second core
                multicore_launch_core1(&setup_sampler);
            }

            // This is started by the constructor to run on Core 1
            static void setup_sampler() {
                LoudnessBasePico *that = get_instance(); /// \todo #ugly
                that->alarmpool = alarm_pool_create(0,32);

                // start the timeer
                alarm_pool_add_repeating_timer_ms(that->alarmpool,-sample_interval,timer_callback,NULL,&(that->sample_timer));

            }

            // timer-called sample collect and FFT analysing
            static bool timer_callback(repeating_timer_t *rt) {
                
                // visually indicate that audio analysis on Core1 is running
                static bool blink = false;
                blink = !blink;
                gpio_put(25,blink);
                
                // LoudnessBasePico *that = (LoudnessBasePico *) rt->user_data;
                LoudnessBasePico *that = LoudnessBasePico::get_instance();

                that->collect_sample();
                that->calculate_loudness();
                return true;
            }

            // start DMA collection of data into buffer
            void collect_sample() {
                adc_fifo_drain();
                adc_run(false);

                dma_channel_configure(dma_channel, &dma_config,
                            sample_buffer,    // dst
                            &adc_hw->fifo,  // src
                            sample_count,          // transfer count
                            true            // start immediately
                            );

                adc_run(true);
                dma_channel_wait_for_finish_blocking(dma_channel);

            }

        private:
            uint8_t adc_num;
            uint8_t adc_pin;

            /// @brief  PiPico DMA configuration
            dma_channel_config dma_config;

            uint dma_channel;

            /// @brief components to start a timer on the second core
            alarm_pool_t *alarmpool;
            repeating_timer_t sample_timer;

    };
};
#endif