#ifndef FP_TASK_H
#define FP_TASK_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"

static uint8_t FP_ID = 0x00;

void FR_Task(void);//指纹模块工作模式选择
void Add_FR(void);//录指纹
void Compare_FR(void);//比对指纹
void Clean_FR(void) ;//清空指纹库


#endif
