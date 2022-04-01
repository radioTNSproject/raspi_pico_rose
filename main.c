/**
 * 
 *
 * 
 */


#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/structs/rosc.h"

#include "table.h"

#define PWM10 10
#define PWM11 11
#define PWM12 12
#define PWM13 13

#define PWM14 14
#define PWM15 15

#define PWM16 16
#define PWM17 17

#define PWM18 18
#define PWM19 19
#define PWM20 20
#define PWM21 21

#define STEP_DURATION 4000
#define OFFSET 500

// theMax = power of 2, 2^11 = 2048
uint16_t get_random_16bits(uint8_t theMax) 
{
    uint16_t myValue = 0;
    for(uint8_t myIdx = 0; myIdx < theMax ; myIdx++ )
    {
        myValue <<= 1;
        myValue += rosc_hw->randombit;
    }
    // offset, 500 = 2s
    myValue += OFFSET;
    return myValue;
}

void setPWM(uint8_t PWMnum)
{
    // If there is still time before starting PWM wave, decrease and leave
    if(dPWMdelay[PWMnum])
    {
        dPWMdelay[PWMnum]--;
    }

    // Else we start the wave and find a random number 
    else
    {
        if(bPWMphase[PWMnum])
        {
            pwm_set_gpio_level(PWMnum, (uint16_t)(byte_array_dec[dPWMidx[PWMnum]] * 255));                
            if(++dPWMidx[PWMnum] == sizeof(byte_array_dec))
            {
                dPWMidx[PWMnum] = 0;
                bPWMphase[PWMnum] = false;                
            }                
        }
        else
        {
            dPWMdelay[PWMnum] = get_random_16bits(11);                
            pwm_set_gpio_level(PWMnum, 0);
            bPWMphase[PWMnum] = true;
        }
    }
}


int main() {

    // Tell the LED pin that the PWM is in charge of its value.
    for(int myidx = 0; myidx < 32; myidx++)
    {
        dPWMdelay[myidx] = 0;
    }
    for(int myidx = 0; myidx < 32; myidx++)
    {
        bPWMphase[myidx] = false;
    }
    for(int myidx = 0; myidx < 32; myidx++)
    {
        dPWMidx[myidx] = 0;
    }

    gpio_set_function(PWM10, GPIO_FUNC_PWM);
    gpio_set_function(PWM11, GPIO_FUNC_PWM);
    gpio_set_function(PWM12, GPIO_FUNC_PWM);
    gpio_set_function(PWM13, GPIO_FUNC_PWM);

    gpio_set_function(PWM14, GPIO_FUNC_PWM);
    gpio_set_function(PWM15, GPIO_FUNC_PWM);

    gpio_set_function(PWM16, GPIO_FUNC_PWM);
    gpio_set_function(PWM17, GPIO_FUNC_PWM);

    gpio_set_function(PWM18, GPIO_FUNC_PWM);
    gpio_set_function(PWM19, GPIO_FUNC_PWM);
    gpio_set_function(PWM20, GPIO_FUNC_PWM);
    gpio_set_function(PWM21, GPIO_FUNC_PWM);

    // Figure out which slice we just connected to the LED pin

    uint slice_num10 = pwm_gpio_to_slice_num(PWM10);
    uint slice_num11 = pwm_gpio_to_slice_num(PWM11);
    uint slice_num12 = pwm_gpio_to_slice_num(PWM12);
    uint slice_num13 = pwm_gpio_to_slice_num(PWM13);

    uint slice_num14 = pwm_gpio_to_slice_num(PWM14);
    uint slice_num15 = pwm_gpio_to_slice_num(PWM15);

    uint slice_num16 = pwm_gpio_to_slice_num(PWM16);
    uint slice_num17 = pwm_gpio_to_slice_num(PWM17);

    uint slice_num18 = pwm_gpio_to_slice_num(PWM18);
    uint slice_num19 = pwm_gpio_to_slice_num(PWM19);
    uint slice_num20 = pwm_gpio_to_slice_num(PWM20);
    uint slice_num21 = pwm_gpio_to_slice_num(PWM21);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 4.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num10, &config, true);
    pwm_init(slice_num11, &config, true);
    pwm_init(slice_num12, &config, true);
    pwm_init(slice_num13, &config, true);

    pwm_init(slice_num14, &config, true);
    pwm_init(slice_num15, &config, true);

    pwm_init(slice_num16, &config, true);
    pwm_init(slice_num17, &config, true);

    pwm_init(slice_num18, &config, true);
    pwm_init(slice_num19, &config, true);
    pwm_init(slice_num20, &config, true);
    pwm_init(slice_num21, &config, true);


    while (1)
    {
        
        setPWM(PWM10);
        setPWM(PWM11);
        setPWM(PWM12);
        setPWM(PWM13);

        setPWM(PWM14);
        setPWM(PWM15);

        setPWM(PWM16);
        setPWM(PWM17);

        setPWM(PWM18);
        setPWM(PWM19);
        setPWM(PWM20);
        setPWM(PWM21);

        sleep_us(STEP_DURATION);           
        
    }        
}
