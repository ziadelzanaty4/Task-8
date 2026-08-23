#include "../MCAL/RCC/RCC.h"
#include "../MCAL/GPIO/GPIO.h"

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
}

void loop(){
    // continuously read each switch's current state (0 or 1), and drive the matching LED to reflect it, live
    GPIO_Write(LED_RED,   GPIO_Read(SW1));
    GPIO_Write(LED_GREEN, GPIO_Read(SW2));
    GPIO_Write(LED_BLUE,  GPIO_Read(SW3));
}

int main(void)
{
    setup();   // all the reg config happens here
    while (1)
    {
        loop();   // the switch-reading/LED-driving happens here
    }
}