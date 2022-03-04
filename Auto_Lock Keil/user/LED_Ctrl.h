#ifndef LED_CTRL_H
#define LED_CTRL_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"


/*LED����*/

//uint8_t control_code 	ģʽ������
#define breath_LED			0x01
#define blink_LED				0x02
#define LED_on					0x03
#define LED_off					0x04
#define LED_slow_on			0x05
#define LED_slow_off		0x06

#define auto_LED_off		0xF5		//�ر��ϵ��Զ�����
#define auto_LED_on			0xF6		//�����ϵ��Զ�����
#define power_default		0xFA		//�ϵ�Ĭ�����Ʋ���
#define success_default	0xFB		//�ɹ�Ĭ�����Ʋ���
#define fail_default		0xFC		//ʧ��Ĭ�����Ʋ���

//uint8_t LED_speed (range: 0-0xFF) �ٶ�(ԽСԽ��)
#define LED_speed				0x0F

//uint8_t LED_color					��ɫ
#define red							0x01
#define green						0x02
#define yellow					0x03
#define blue						0x04
#define pink						0x05
#define sky_blue				0x06
#define white						0x07

#define tri_color				0x20
#define seven_color			0x30		//(�������ֵ�Ч���������ں�����ģʽ)

//uint8_t LED_cycle					(range: 0-255, 0Ϊ����ѭ��)
#define LED_cycle						0x00


uint16_t PS_AuraLedConfig(uint8_t control_code, uint8_t speed, uint8_t color, uint8_t cycle);//�⻷LED����


#endif
