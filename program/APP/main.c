#include "../MCAL/RCC/RCC.h"
#include "../MCAL/GPIO/GPIO.h"
#include "./Memory_adresses.h"

// Switch LEDs (PA3, PA4, PA5)
#define LED_RED    3
#define LED_GREEN 4
#define LED_BLUE  5

// Switches (PA6, PA7, PA8)
#define SW1 6
#define SW2 7
#define SW3 8

// ADC LEDs (PA9, PA10, PA11)
#define LED_ADC_1     9
#define LED_ADC_2     10
#define LED_ADC_3     11

unsigned int Read_ADC_Value(void);
void Process_LED_Ranges(unsigned int adc_value);

void setup(void){
    // RCC Init
    RCC_Init();

    // Enable ADC1 Clock explicitly in RCC APB2 peripheral clock register (ADC1 is bit 8)
    SET_BIT(RCC_APB2ENR, 8);

    // Configure each switch LED pin: output
    GPIO_InitOutput_PA3();
    GPIO_InitOutput_PA4();
    GPIO_InitOutput_PA5();

    // Configure each switch pin: input, pulled down
    GPIO_InitInput_PA6();
    GPIO_InitInput_PA7();
    GPIO_InitInput_PA8();

    // Configure ADC LEDs as output (PA9, PA10, PA11) -> MODER = 01
    SET_BIT(GPIOA_MODER, 18); CLEAR_BIT(GPIOA_MODER, 19);
    SET_BIT(GPIOA_MODER, 20); CLEAR_BIT(GPIOA_MODER, 21);
    SET_BIT(GPIOA_MODER, 22); CLEAR_BIT(GPIOA_MODER, 23);

    // Configure PA0 as Analog Input for ADC
    SET_BIT(GPIOA_MODER, 0);
    SET_BIT(GPIOA_MODER, 1);

    // Select ADC Channel 0
    ADC_SQR3 = 0; 

    // Enable Continuous Conversion mode
    SET_BIT(ADC_CR2, 1);

    // Enable ADC
    SET_BIT(ADC_CR2, 0);

    // Start conversion once to kick off continuous cycle
    SET_BIT(ADC_CR2, 30);
}

void loop(void){
    // Read switches and drive matching LEDs using the original reliable logic
    GPIO_Write(LED_RED,   GPIO_Read(SW1));
    GPIO_Write(LED_GREEN, GPIO_Read(SW2));
    GPIO_Write(LED_BLUE,  GPIO_Read(SW3));
    
    // Read current value from ADC
    unsigned int adc_val = Read_ADC_Value();
    
    // Compare and turn ON appropriate ADC LEDs
    Process_LED_Ranges(adc_val);  
}

// --- ADC Read Function ---
unsigned int Read_ADC_Value(void) {
    // Wait for End of Conversion
    while (!GET_BIT(ADC_SR, 1));
    
    // Return ADC data
    return ADC_DR;
}

// --- Main Logic Function for ADC LEDs ---
void Process_LED_Ranges(unsigned int adc_value) {
    // Clear ADC LEDs (PA9, PA10, PA11)
    CLEAR_BIT(GPIOA_ODR, 9);
    CLEAR_BIT(GPIOA_ODR, 10);
    CLEAR_BIT(GPIOA_ODR, 11);

    // Define ranges based on 12-bit ADC (0 to 4095)
    if (adc_value < 1365) {
        SET_BIT(GPIOA_ODR, 9);
    } 
    else if (adc_value >= 1365 && adc_value < 2730) {
        SET_BIT(GPIOA_ODR, 9);
        SET_BIT(GPIOA_ODR, 10);
    } 
    else {
        SET_BIT(GPIOA_ODR, 9);
        SET_BIT(GPIOA_ODR, 10);
        SET_BIT(GPIOA_ODR, 11);
    }
}

int main(void)
{
    setup();

    while (1)
    {
        loop();
    }
}
