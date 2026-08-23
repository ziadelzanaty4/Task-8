#include "../MCAL/RCC/RCC.h"
#include "../MCAL/GPIO/GPIO.h"
#include "./Memory_adresses.h"

// Switch LEDs (PA3, PA4, PA5)
#define LED_SW_RED    3
#define LED_GREEN_SW  4
#define LED_BLUE_SW   5

// Switches (PA6, PA7, PA8)
#define SW1 6
#define SW2 7
#define SW3 8

// ADC LEDs (PA9, PA10, PA11)
#define LED_ADC_1     9
#define LED_ADC_2     10
#define LED_ADC_3     11

unsigned int Read_ADC_Value();
void Process_Switches();
void Process_LED_Ranges(unsigned int adc_value);

void setup(){
    // RCC Init
    RCC_Init();

    // Configure Switch LEDs as output (PA3, PA4, PA5) -> MODER = 01
    SET_BIT(GPIOA_MODER, 6);  CLEAR_BIT(GPIOA_MODER, 7);   // PA3
    SET_BIT(GPIOA_MODER, 8);  CLEAR_BIT(GPIOA_MODER, 9);   // PA4
    SET_BIT(GPIOA_MODER, 10); CLEAR_BIT(GPIOA_MODER, 11);  // PA5

    // Configure Switches as input (PA6, PA7, PA8) -> MODER = 00
    CLEAR_BIT(GPIOA_MODER, 12); CLEAR_BIT(GPIOA_MODER, 13); // PA6
    CLEAR_BIT(GPIOA_MODER, 14); CLEAR_BIT(GPIOA_MODER, 15); // PA7
    CLEAR_BIT(GPIOA_MODER, 16); CLEAR_BIT(GPIOA_MODER, 17); // PA8

    // Configure ADC LEDs as output (PA9, PA10, PA11) -> MODER = 01
    SET_BIT(GPIOA_MODER, 18); CLEAR_BIT(GPIOA_MODER, 19); // PA9
    SET_BIT(GPIOA_MODER, 20); CLEAR_BIT(GPIOA_MODER, 21); // PA10
    SET_BIT(GPIOA_MODER, 22); CLEAR_BIT(GPIOA_MODER, 23); // PA11

    // Configure PA0 as Analog Input for ADC -> MODER = 11
    SET_BIT(GPIOA_MODER, 0);
    SET_BIT(GPIOA_MODER, 1);

    // Select ADC Channel 0
    ADC_SQR3 = 0; 

    // Enable Continuous Conversion mode
    SET_BIT(ADC_CR2, 1);

    // Enable ADC
    SET_BIT(ADC_CR2, 0);

    // Start conversion
    SET_BIT(ADC_CR2, 30);
}

void loop(){
    // Handle switches and their LEDs
    Process_Switches();
    
    // Read ADC value
    unsigned int adc_val = Read_ADC_Value();
    
    // Drive ADC LEDs based on ranges
    Process_LED_Ranges(adc_val);  
}

// --- Switches Logic Function ---
void Process_Switches() {
    if (GET_BIT(GPIOA_IDR, SW1)) { SET_BIT(GPIOA_ODR, LED_SW_RED); } 
    else { CLEAR_BIT(GPIOA_ODR, LED_SW_RED); }

    if (GET_BIT(GPIOA_IDR, SW2)) { SET_BIT(GPIOA_ODR, LED_GREEN_SW); } 
    else { CLEAR_BIT(GPIOA_ODR, LED_GREEN_SW); }

    if (GET_BIT(GPIOA_IDR, SW3)) { SET_BIT(GPIOA_ODR, LED_BLUE_SW); } 
    else { CLEAR_BIT(GPIOA_ODR, LED_BLUE_SW); }
}

// --- ADC Read Function ---
unsigned int Read_ADC_Value() {
    // Wait for End of Conversion
    while (!GET_BIT(ADC_SR, 1));
    
    // Return ADC data
    return ADC_DR;
}

// --- ADC LEDs Logic Function ---
void Process_LED_Ranges(unsigned int adc_value) {
    // Clear ADC LEDs (PA9, PA10, PA11)
    CLEAR_BIT(GPIOA_ODR, 9);
    CLEAR_BIT(GPIOA_ODR, 10);
    CLEAR_BIT(GPIOA_ODR, 11);

    // If ADC value is 0, keep ADC LEDs OFF
    if (adc_value == 0) {
        return; 
    }
    
    // ADC ranges (1 to 4095)
    if (adc_value > 0 && adc_value <= 1365) {
        SET_BIT(GPIOA_ODR, 9);
    } 
    else if (adc_value > 1365 && adc_value <= 2730) {
        SET_BIT(GPIOA_ODR, 9);
        SET_BIT(GPIOA_ODR, 10);
    } 
    else {
        SET_BIT(GPIOA_ODR, 9);
        SET_BIT(GPIOA_ODR, 10);
        SET_BIT(GPIOA_ODR, 11);
    }
}

int main()
{
    setup();

    while (1)
    {
        loop();
    }
}
