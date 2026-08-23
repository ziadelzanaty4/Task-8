#ifndef Memory_Adresses_H
#define Memory_Adresses_H

// defining main ADC phripherals 
#define ADC_BASE_ADD    0x40012000U // ACD1 Base_address

//defining CR2 REG
#define ADC_CR2_offset     0x08
#define ADC_CR2           *((volatile unsigned int*)(ADC_CR2_offset + ADC_BASE_ADD))


// defining channel selection REG
#define ADC_SQR3_offset    0x34U
#define ADC_SQR3         *((volatile unsigned int*)(ADC_SQR3_offset + ADC_BASE_ADD)) 

// defining End of conversion REG
#define ADC_SR_offset      0x00U
#define ADC_SR      *((volatile unsigned int*)(ADC_SR_offset + ADC_BASE_ADD))

//defining CONVERSION RESULT REG
#define ADC_DR_offset      0x4CU
#define ADC_DR           *((volatile unsigned int*)(ADC_DR_offset + ADC_BASE_ADD)) 

#endif //Memory_Adresses_H
