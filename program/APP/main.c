#include <stdint.h>

#define RCC_BASE        0x40023800UL
#define GPIOC_BASE      0x40020800UL

#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOC_MODER     (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_OTYPER    (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_OSPEEDR   (*(volatile uint32_t *)(GPIOC_BASE + 0x08))
#define GPIOC_PUPDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))
#define GPIOC_ODR       (*(volatile uint32_t *)(GPIOC_BASE + 0x14))

#define LED_PIN         13   // PC13 on many STM32F401 "Black Pill" boards

static void delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm volatile ("nop");
    }
}

int main(void)
{
    // Enable GPIOC clock
    RCC_AHB1ENR |= (1U << 2);

    // Set PC13 as general purpose output
    GPIOC_MODER &= ~(3U << (LED_PIN * 2));
    GPIOC_MODER |=  (1U << (LED_PIN * 2));

    // Push-pull
    GPIOC_OTYPER &= ~(1U << LED_PIN);

    // Low speed is fine
    GPIOC_OSPEEDR &= ~(3U << (LED_PIN * 2));

    // No pull-up / pull-down
    GPIOC_PUPDR &= ~(3U << (LED_PIN * 2));

    while (1)
    {
        GPIOC_ODR ^= (1U << LED_PIN);   // toggle LED
        delay(300000);
    }
}