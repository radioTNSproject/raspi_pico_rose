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

// Setup
/***************************************************/

uint8_t dPWMUsed[] = {10,11,12,13,14,15,16,17,18,19,20,21};

#define RANDOMSPARKS        false
enum BrightnessType Brightness = LINEAR;

#define STEP_DURATION_US    6000
#define OFFSET_DELAY        500

/***************************************************/

// theMax = power of 2, 2^11 = 2048
uint16_t get_random_16bits(uint8_t theMax) 
{
    uint16_t myValue = 0;
    for(uint8_t myIdx = 0; myIdx < theMax ; myIdx++ )
    {
        myValue <<= 1;
        myValue += rosc_hw->randombit;
    }        
    return myValue;
}

void setup_initial_values(uint8_t PWMnum)
{
    dPWMdelay[PWMnum] = get_random_16bits(11) + OFFSET_DELAY;
    if (Brightness == RANDOM)  
    {
        dPWMbrightnessCoef[PWMnum] = get_random_16bits(8);    
    }

#ifdef RANDOMSPARKS    
    dPWMWaveArray[PWMnum] = get_random_16bits(2);                         // 0 to 3
    switch(dPWMWaveArray[PWMnum])
    {
        case 0:
            pPWMarray[PWMnum] = byte_array_dec0;                    
            break;
        case 1:
            pPWMarray[PWMnum] = byte_array_dec1;
            break;
        case 2:
            pPWMarray[PWMnum] = byte_array_dec2;
            break;
        case 3:
            pPWMarray[PWMnum] = byte_array_dec3;
            break;
        default:
            pPWMarray[PWMnum] = byte_array_dec1;
            break;
    }
#else
    pPWMarray[PWMnum] = byte_array_dec1;
#endif
    pwm_set_gpio_level(PWMnum, 0);                                            // Take no risks
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
        uint8_t dPWMArrayValue;
        dPWMArrayValue = *(pPWMarray[PWMnum] + dPWMidx[PWMnum]);    

        uint16_t dFinalValue;
        switch (Brightness)
        {
            case RANDOM:
                dFinalValue = (uint16_t)(dPWMArrayValue * dPWMbrightnessCoef[PWMnum]);
                break;

            case LINEAR:
                dFinalValue = (uint16_t)(dPWMArrayValue * dPWMArrayValue);
                break;

            case NORMAL:
                dFinalValue = (uint16_t)(dPWMArrayValue * 255);
                break;                
        }
        pwm_set_gpio_level(PWMnum, dFinalValue);
        ++dPWMidx[PWMnum];

        // If we've reached end of array
        if(!dPWMArrayValue)
        {                
            dPWMidx[PWMnum] = 0;  
            setup_initial_values(PWMnum);
        }
    }
}


int main() {    

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

    for(int myidx = 0; myidx < sizeof(dPWMUsed); myidx++)
    {
        setup_initial_values(dPWMUsed[myidx]);
    }

    while (1)
    {
        for(int myidx = 0; myidx < sizeof(dPWMUsed); myidx++)
        {
            setPWM(dPWMUsed[myidx]);
        }
        
        sleep_us(STEP_DURATION_US);           
        
    }        
}
