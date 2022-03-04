#ifndef LED_CTRL_H
#define LED_CTRL_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"


/*LED定义*/

//uint8_t control_code 	模式控制码
#define breath_LED			0x01
#define blink_LED				0x02
#define LED_on					0x03
#define LED_off					0x04
#define LED_slow_on			0x05
#define LED_slow_off		0x06

#define auto_LED_off		0xF5		//关闭上电自动亮灯
#define auto_LED_on			0xF6		//开启上电自动亮灯
#define power_default		0xFA		//上电默认亮灯参数
#define success_default	0xFB		//成功默认亮灯参数
#define fail_default		0xFC		//失败默认亮灯参数

//uint8_t LED_speed (range: 0-0xFF) 速度(越小越快)
#define LED_speed				0x0F

//uint8_t LED_color					颜色
#define red							0x01
#define green						0x02
#define yellow					0x03
#define blue						0x04
#define pink						0x05
#define sky_blue				0x06
#define white						0x07

#define tri_color				0x20
#define seven_color			0x30		//(以上两种灯效仅可以用于呼吸灯模式)

//uint8_t LED_cycle					(range: 0-255, 0为无限循环)
#define LED_cycle						0x00


uint16_t PS_AuraLedConfig(uint8_t control_code, uint8_t speed, uint8_t color, uint8_t cycle);//光环LED控制


#endif
