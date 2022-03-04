#ifndef KEY_H
#define KEY_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"

#define KEY1        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)  //KEY1°´¼üPB4
#define KEY2        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)  //KEY2°´¼üPB6
#define KEY3        HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7) 	//KEY3°´¼üPB7

uint8_t key_scan(void);


#endif
