#ifndef BIT_MATH_H
#define BIT_MATH_H

#define set_bit(reg,number)   	(reg|=(1<<number))
#define clear_bit(reg,number)   (reg&=~(1<<number))
#define get_bit(reg, number)    (((reg) >> (number)) & 1)
#define toggle_bit(reg,number)  (reg^=(1<<number))

#define set_byte(reg)           reg=0xFF
#define clear_byte(reg)         reg=0x00


#define write_bit_double(reg,number,mode)  (reg=((reg&(~(0b11<<(number*2))))|(mode<<(number*2))))


#endif
