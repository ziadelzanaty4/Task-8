#ifndef GPIO_CFG_H
#define GPIO_CFG_H

#define OPEN_DRAIN_MODE        1
#define PUSH_PULL_MODE         0

#define OP_MODE                PUSH_PULL_MODE

#define LOW_SPEED_MODE               0b00
#define MEDIUM_SPEED_MODE            0b01
#define HIGH_SPEED_MODE              0b10
#define VERY_HIGH_SPEED_MODE         0b11 


#define SPEED_MODE   VERY_HIGH_SPEED_MODE


#define NO_PULL                         0b00
#define PULLUP                          0b01
#define PULLDOWN                        0b10
#define PUPD_RESERVED                   0b11

#define PULL_TYPE                   PULLDOWN




#endif