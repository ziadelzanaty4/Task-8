#include "../MCAL/RCC/RCC.h"

void setup(){
    //  Open clocks for all peripherals at once
    RCC_Init();

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

void loop() {
    //  Read the current value from the ADC
    unsigned int adc_val = Read_ADC_Value(); 
    
    //  Compare the value and turn ON the appropriate LEDs
    Process_LED_Ranges(adc_val);             
}

int main()
{
    setup();  // As you know, setup is called only once

    while (1)
    {
        loop();   // Loop is called continuously so it's in the while(1)
    }
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
