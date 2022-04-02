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

uint8_t dPWMUsed[] = {10,11,12,13,14,15,16,17,18,19,20,21};

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

    for(int myidx = 0; myidx < sizeof(dPWMUsed); myidx++)
    {
        gpio_set_function(dPWMUsed[myidx], GPIO_FUNC_PWM);
    }
    
    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 4.f);

    // Figure out which slice we just connected to the LED pin
    // Load the configuration into our PWM slice, and set it running.
    uint slice_num;
    for(int myidx = 0; myidx < sizeof(dPWMUsed); myidx++)
    {
        slice_num = pwm_gpio_to_slice_num(dPWMUsed[myidx]);
        pwm_init(slice_num, &config, true);
    }

    while (1)
    {
        for(int myidx = 0; myidx < sizeof(dPWMUsed); myidx++)
        {
            setPWM(dPWMUsed[myidx]);
        }
        
        sleep_us(STEP_DURATION);           
        
    }        
}
