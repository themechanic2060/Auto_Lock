#ifndef FP_TASK_H
#define FP_TASK_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"

static uint8_t FP_ID = 0x00;

void FR_Task(void);//ָ��ģ�鹤��ģʽѡ��
void Add_FR(void);//¼ָ��
void Compare_FR(void);//�ȶ�ָ��
void Clean_FR(void) ;//���ָ�ƿ�


#endif
