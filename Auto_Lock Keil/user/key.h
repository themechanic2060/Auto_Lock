#ifndef KEY_H
#define KEY_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"

#define KEY1        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)  //KEY1����PB4
#define KEY2        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)  //KEY2����PB6
#define KEY3        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7) 	//KEY3����PB7

uint8_t key_scan(void);


#endif
