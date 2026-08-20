#include "../MCAL/RCC/RCC.h"
#include "../LIB/BIT_MATH.h"


#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
//drive GPIO -> // search for GPIO in data sheet

//GPIO_MODER -> input or output 
//32bit -> set or clear in this register 
//GPIOA_MODER   0x4000 0000

// what is a pointer -> pointer is a c tool (points to the address of the variable direct)
// I have a variable x -> gets stored -> pointer accesses the value x by getting it from it's stored 

//register size in MCU of stm32 is 32 bits

//when I poin with pointer I will need to point to 32 bits

// int may be 32 or 64 bits depends on compiler 
// unsigned int* GPIOA_MODER = unsigned int*(0x4000 0000);
// 32 bit -> hexa = 0x04 how ? 32 bits = ? bytes == 4 bytes 
// offset of 32 bit register is 0x04 bytes in address
#define GPIOA_BASE_ADDR     0x40020000
#define GPIO_MODER_OFFSET   0x00
#define GPIO_OTYPER_OFFSET         0x04
#define GPIO_OSPEEDR_OFFSET        0x08
#define GPIO_PUPDR_OFFSET        0x0C
#define GPIO_ODR_OFFSET         0x14


#define GPIOA_MODER      *((volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_MODER_OFFSET))     //gets the data inside 
#define GPIOA_OTYPER     *((volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_OTYPER_OFFSET))
#define GPOIOA_OSPEEDR   *((volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_OSPEEDR_OFFSET))
#define GPIOA_PUPDR   *((volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_PUPDR_OFFSET))  
#define GPIOA_ODR      *((volatile unsigned int*)(GPIOA_BASE_ADDR + GPIO_ODR_OFFSET)) 
// #define GPIOA_MODER      (unsigned long int*)(GPIOA_MODER)   //points to 64 bits instead
// offset -> difference of base address 
// offset of moder is 0x04 
// moder address = base address + offset 


static void delay(volatile unsigned int count)
{
    while (count--)
    {
        __asm volatile ("nop");
    }
}

void setup(){
    MCAL_void_RCC_Init();
    // set mode to output
    SET_BIT(GPIOA_MODER,0);
    CLEAR_BIT(GPIOA_MODER,1);
    // set output type to push pull
    CLEAR_BIT(GPIOA_OTYPER,0);
    // set output speed to LOWSPEED
    CLEAR_BIT(GPOIOA_OSPEEDR,0);
    CLEAR_BIT(GPOIOA_OSPEEDR,1);
    // set pin to be pulled down
    CLEAR_BIT(GPIOA_PUPDR,0);
    SET_BIT(GPIOA_PUPDR,1);


}

void loop(){
    SET_BIT(GPIOA_ODR,0);
    delay(3000000);
    CLEAR_BIT(GPIOA_ODR,0);
    delay(3000000);
    
}

int main(void)
{
    setup();  //as you know the setups is called only once


    while (1)
    {
        loop();   //loop is called continously so it's in the while(1)
    }
}