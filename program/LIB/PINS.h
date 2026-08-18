#ifndef PIN_HEADER
#define PIN_HEADER


//gpio -> 16 pin 

//pin number = gpio_index * 16 + pin_number in this gpio
//PA0 = 0*16 + 0 -> 
//PB3 = 1*16 + 3 ->19

#define PA0       0
#define PA1       1
//continue PAs
#define PB0       16
#define PB1       17
//continue PBs
#define PC13      44

#define INPUT    0b00
#define OUTPUT   0b01
#define AF       0b10
#define ANALOG   0b11

#define HIGH      1
#define LOW       0

#endif