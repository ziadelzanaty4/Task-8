#ifndef BIT_MATH_H  //what is this 
#define BIT_MATH_H


// void set_bit(unsigned char REG, unsigned char BIT){
    // whhy char -> 
    //sizeof(char) equals 8 bits -> at some mcus this is the register size at others the register size is bigger 
    // int -> 32 bit or 64 bit ->  8 registers at the same time 
    // as I need to point to one register I use the smallest data type -> char

    // why unsigned ->
    // unsigned means no -ve , signed char or char -> expression of 8 bits will be devided into half +ve , -ve
    // 8 bits will be 255 -> signed: -127 , 0 ,126 = 255 -> this is not the hardware registers case 
    // unsigned because hardware registers or (peripherals registers) work with +ve nums only


    // BIT -> X so the value of its place is 2^X 
    // what if the register already has 1 in the 7th bit ????
    // how I will get 2^X  ???

    //Using bit wise operations  | , & , ^ , <<
    // the operation will be | with 1
    // set at a bit that equals 1 -> 1 | 1 =1
    // set at a bit that equals 0 -> 0 | 1 = 1

    // | or 

    // 2nd problem how I will get the shift -> 
    // x = (1<<4); 0b0000 1000
    // x = (2<<4);  10 -> shift to the left by 4 0x0001 0000
    // I want to set the bit number X with 1
    // REG = REG | (1<<BIT);    //0b000(1)  0010  I will or operation (|) with 0b000(1)  0000
    // REG |= (1<<BIT);  // tottally like the operation above
    // x=x+4 == x+=4

// }

// void clear_bit(unsigned char REG, unsigned char BIT){
    // we will use a set of 2 bitwise operations 
    // REG = REG & (~(1<<BIT));
    //suppose the BIT was the 3rd one
    // 1<<BIT -> 0b0000 0100
    // ~ this is the not in bitwise operations
    //~(1<<BIT) -> 0b1111 1(0)11
    // above term & with the register:
    // suppose the and operation is done with a zero (orignal case was zero) -> 1 & 0 =0 (didn't change)
    // ---------------------------------------a one (orignal case was one) -> 1 & 1 = 1 (didn't change)
    // the rest of the register kept its value 
    // the target bit will be X & 0(the one I made) -> output will be zero always
    // cleared the target bit && (logical operator) kept the value of the rest of the register
    // REG &= (~(1<<BIT));
// }


// unsigned char get_bit(unsigned char REG, unsigned char BIT){   
    // return value is 1 or 0 based on the bit value 
    // unsigned char reg_shifterd = REG>>BIT; // 0b111(0) 1111  -> >>5  0b0000 111(0)
    // get rid of unused bits
    // return reg_shifterd & 1; //0b0000 111(X) & 0b0000 0001  -> X

    // return ((REG>>BIT)&1);
// }

// unsigned char toggle_bit(){
    // to be implemnted by trainees (think and search)

// }

/*----------------------------------------------------------------------------------
*ANY FUNCTION WILL GET STORED IN A CERTAIN ADDRESS AFTER COMPILATION
*ANY FUNCTION called of them will get called from the stored address 
*resource managment problem: first these functions are core functions 
*these functions all of them can be written as one line ? 
* c gave me an extension called macros -> #define how can be used here
* 
*/
#define SET_BIT(REG,BIT)        REG |= (1<<BIT)
#define CLEAR_BIT(REG,BIT)      REG &= (~(1<<BIT))
#define GET_BIT(REG,BIT)        ((REG>>BIT)&1)
// #define TOGGLE_BIT(REG,BIT)   //will be implemnted by trainees     




#endif
