/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Fade an LED between low and high brightness. An interrupt handler updates
// the PWM slice's output level each time the counter wraps.

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/structs/rosc.h"

#include "table.h"

#define PWM9 9
#define PWM10 10
#define PWM11 11
#define PWM12 12
#define PWM13 13

#define PWM15 15
#define PWM16 16

#define PWM18 18
#define PWM19 19
#define PWM20 20
#define PWM21 21
#define PWM22 22

#define TEMPORISATION 4000

uint16_t get_random_16bits() 
{
    uint16_t myValue = 0;
    for(uint8_t myIdx = 0; myIdx < 13 ; myIdx++ )
    {
        myValue <<= 1;
        myValue += rosc_hw->randombit;
    }
    return myValue;
}

void setPWM(uint8_t PWMnum)
{
    if(dPWMdelay[PWMnum])
    {
        dPWMdelay[PWMnum]--;
    }
    else
    {
        if(bPWMphase[PWMnum])
        {
            pwm_set_gpio_level(PWMnum, (uint16_t)(byte_array_dec[dPWMidx[PWMnum]] * 255));                
            if(++dPWMidx[PWMnum] == 768)
            {
                dPWMidx[PWMnum] = 0;
                bPWMphase[PWMnum] = false;
            }                
        }
        else
        {
            dPWMdelay[PWMnum] = get_random_16bits();                
            pwm_set_gpio_level(PWMnum, 0);
            bPWMphase[PWMnum] = true;
        }
    }
}


/*
#ifdef PWM15
void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(PWM15));

    if (going_up) {
        ++fade;
        if (fade > 255) {
            fade = 255;
            going_up = false;
        }
    } else {
        --fade;
        if (fade < 0) {
            fade = 0;
            going_up = true;
        }
    }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
    pwm_set_gpio_level(PWM15, fade * fade);
}
#endif
*/

int main() {
#ifndef PWM15
#warning pwm/led_fade example requires a board with a regular LED
#else

    /*
    //PWM15
    uint16_t dPWM15delay = 0;
    bool bPWM15phase = false;
    uint16_t dPWM15idx = 0;

    //PWM16
    uint16_t dPWM16delay = 0;
    bool bPWM16phase = false;
    uint16_t dPWM16idx = 0;
    */

    // Tell the LED pin that the PWM is in charge of its value.

    gpio_set_function(PWM9,  GPIO_FUNC_PWM);
    gpio_set_function(PWM10, GPIO_FUNC_PWM);
    gpio_set_function(PWM11, GPIO_FUNC_PWM);
    gpio_set_function(PWM12, GPIO_FUNC_PWM);
    gpio_set_function(PWM13, GPIO_FUNC_PWM);

    gpio_set_function(PWM15, GPIO_FUNC_PWM);
    gpio_set_function(PWM16, GPIO_FUNC_PWM);

    gpio_set_function(PWM18, GPIO_FUNC_PWM);
    gpio_set_function(PWM19, GPIO_FUNC_PWM);
    gpio_set_function(PWM20, GPIO_FUNC_PWM);
    gpio_set_function(PWM21, GPIO_FUNC_PWM);
    gpio_set_function(PWM22, GPIO_FUNC_PWM);

    // Figure out which slice we just connected to the LED pin

    uint slice_num9  = pwm_gpio_to_slice_num(PWM9);
    uint slice_num10 = pwm_gpio_to_slice_num(PWM10);
    uint slice_num11 = pwm_gpio_to_slice_num(PWM11);
    uint slice_num12 = pwm_gpio_to_slice_num(PWM12);
    uint slice_num13 = pwm_gpio_to_slice_num(PWM13);

    uint slice_num15 = pwm_gpio_to_slice_num(PWM15);
    uint slice_num16 = pwm_gpio_to_slice_num(PWM16);

    uint slice_num18 = pwm_gpio_to_slice_num(PWM18);
    uint slice_num19 = pwm_gpio_to_slice_num(PWM19);
    uint slice_num20 = pwm_gpio_to_slice_num(PWM20);
    uint slice_num21 = pwm_gpio_to_slice_num(PWM21);
    uint slice_num22 = pwm_gpio_to_slice_num(PWM22);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    //pwm_clear_irq(slice_num);
    //pwm_set_irq_enabled(slice_num, true);
    //irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    //irq_set_enabled(PWM_IRQ_WRAP, true);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 4.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num9,  &config, true);
    pwm_init(slice_num10, &config, true);
    pwm_init(slice_num11, &config, true);
    pwm_init(slice_num12, &config, true);
    pwm_init(slice_num13, &config, true);

    pwm_init(slice_num15, &config, true);
    pwm_init(slice_num16, &config, true);

    pwm_init(slice_num18, &config, true);
    pwm_init(slice_num19, &config, true);
    pwm_init(slice_num20, &config, true);
    pwm_init(slice_num21, &config, true);
    pwm_init(slice_num22, &config, true);

    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    //uint16_t myRand = 0;
    while (1)
    {
        /*myRand = get_random_16bits();
        for(uint16_t myIdx = 0; myIdx < myRand ; myIdx++)
        {
            pwm_set_gpio_level(PWM15, 0);
            sleep_us(300);
        }*/
        
        /*
        //PWM15
        if(dPWM15delay)
        {
            dPWM15delay--;
        }
        else
        {
            if(bPWM15phase)
            {
                pwm_set_gpio_level(PWM15, (uint16_t)(byte_array_dec[dPWM15idx] * 255));                
                if(++dPWM15idx == 768)
                {
                    dPWM15idx = 0;
                    bPWM15phase = false;
                }                
            }
            else
            {
                dPWM15delay = get_random_16bits();                
                pwm_set_gpio_level(PWM15, 0);
                bPWM15phase = true;
            }
        }

        //PWM16
        if(dPWM16delay)
        {
            dPWM16delay--;
        }
        else
        {
            if(bPWM16phase)
            {
                pwm_set_gpio_level(PWM16, (uint16_t)(byte_array_dec[dPWM16idx] * 255));                
                if(++dPWM16idx == 768)
                {
                    dPWM16idx = 0;
                    bPWM16phase = false;
                }                
            }
            else
            {
                dPWM16delay = get_random_16bits();                
                pwm_set_gpio_level(PWM16, 0);
                bPWM16phase = true;
            }
        }*/
        setPWM(PWM9);
        setPWM(PWM10);
        setPWM(PWM11);
        setPWM(PWM12);
        setPWM(PWM13);

        setPWM(PWM15);
        setPWM(PWM16);

        setPWM(PWM18);
        setPWM(PWM19);
        setPWM(PWM20);
        setPWM(PWM21);
        setPWM(PWM22);

        sleep_us(TEMPORISATION);
        /*
        for(uint16_t myIdx = 0; myIdx < 768 ; myIdx++)
        {
            if (going_up) 
            {
                ++fade;
                sleep_us(2000);
                if (fade == 255) 
                {                
                    going_up = false;
                }
            }
            else 
            {
                --fade;
                sleep_us(2000);
                if (fade == 0) 
                {                
                    going_up = true;
                }
            }
            
            // Square the fade value to make the LED's brightness appear more linear
            // Note this range matches with the wrap value
            pwm_set_gpio_level(PWM15, (uint16_t)(byte_array_dec[myIdx] * 255));
            sleep_us(2000);
        }*/
        
        
    }
        
#endif
}
