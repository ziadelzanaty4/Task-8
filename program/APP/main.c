#include "../MCAL/RCC/RCC.h"
#include "../MCAL/GPIO/GPIO.h"
#include "./Memory_adresses.h"

#define LED_RED   3
#define LED_GREEN 4
#define LED_BLUE  5

#define SW1 6
#define SW2 7
#define SW3 8

void setup(){
    // RCC_Init() turns on the clock for every peripheral at once, including GPIOA; without this, none of the writes below would reach the hardware's flip-flops
    RCC_Init();

    // configure each LED pin: output, push-pull, low speed, no pull
    GPIO_InitOutput_PA3();
    GPIO_InitOutput_PA4();
    GPIO_InitOutput_PA5();

    // configure each switch pin: input, pulled down
    GPIO_InitInput_PA6();
    GPIO_InitInput_PA7();
    GPIO_InitInput_PA8();

    //  Configure PA0 as Analog Input for ADC (MODER0 = 11)
    SET_BIT(GPIOA_MODER, 0);
    SET_BIT(GPIOA_MODER, 1);

    //  Configure PA3 as General Purpose Output (MODER3 = 01)
    SET_BIT(GPIOA_MODER,   6);
    CLEAR_BIT(GPIOA_MODER, 7);

    //  Configure PA4 as General Purpose Output (MODER4 = 01)
    SET_BIT(GPIOA_MODER,   8);
    CLEAR_BIT(GPIOA_MODER, 9);

    //  Configure PA5 as General Purpose Output (MODER5 = 01)
    SET_BIT(GPIOA_MODER,   10);
    CLEAR_BIT(GPIOA_MODER, 11);

    //  Configure ADC Channel: Select Channel 0 (for PA0)
    ADC_SQR3 = 0; 

    //  Enable ADC conversion (ADON = 1 in ADC_CR2)
    SET_BIT(ADC_CR2, 0);
}

void loop(){
  
    // continuously read each switch's current state (0 or 1), and drive the matching LED to reflect it, live
    GPIO_Write(LED_RED,   GPIO_Read(SW1));
    GPIO_Write(LED_GREEN, GPIO_Read(SW2));
    GPIO_Write(LED_BLUE,  GPIO_Read(SW3));
     //  Read the current value from the ADC
    unsigned int adc_val = Read_ADC_Value();
      //  Compare the value and turn ON the appropriate LEDs
    Process_LED_Ranges(adc_val); 
}


// --- ADC Read Function ---
unsigned int Read_ADC_Value() {
    // Start regular conversion (SWSTART = 1 in ADC_CR2, bit 30)
    SET_BIT(ADC_CR2, 30);
    // Wait for End of Conversion (EOC bit in ADC_SR, bit 1)
    while (!GET_BIT(ADC_SR, 1));
    // Return the converted data (12-bit value from 0 to 4095)
    return ADC_DR;
}

// --- Main Logic Function (Ranges & LEDs) ---
void Process_LED_Ranges(unsigned int adc_value) {
    // Clear all LEDs (PA3, PA4, PA5)
    CLEAR_BIT(GPIOA_ODR, 3);
    CLEAR_BIT(GPIOA_ODR, 4);
    CLEAR_BIT(GPIOA_ODR, 5);

    // Define ranges based on 12-bit ADC (0 to 4095)
    if (adc_value < 1365) {
        // Low voltage: Turn ON 1 LED (D1 on PA3)
        SET_BIT(GPIOA_ODR, 3);
    } 
    else if (adc_value >= 1365 && adc_value < 2730) {
        // Medium voltage: Turn ON 2 LEDs (D1 & D2 on PA3, PA4)
        SET_BIT(GPIOA_ODR, 3);
        SET_BIT(GPIOA_ODR, 4);
    } 
    else {
        // High voltage: Turn ON 3 LEDs (D1, D2 & D3 on PA3, PA4, PA5)
        SET_BIT(GPIOA_ODR, 3);
        SET_BIT(GPIOA_ODR, 4);
        SET_BIT(GPIOA_ODR, 5);

    }
}
int main()
{
    setup();  // As you know, setup is called only once

    while (1)
    {
        loop();   // Loop is called continuously so it's in the while(1)
    }
}
  
