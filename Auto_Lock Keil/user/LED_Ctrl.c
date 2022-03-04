#include "LED_Ctrl.h"
#include "bsp_BM2166.h"

/**
  * @brief  更改LED参数
  * @param  control_code:控制码
  * @param  speed				:LED速度 (range: 0-0xFF)
  * @param  color				:LED颜色
  * @param  cycle				:循环次数	(range: 0-255, 0为无限循环)
  * @retval	确认码=00H 表示口令验证正确；
						确认码=01H 表示收包有错；

  */
uint16_t PS_AuraLedConfig(uint8_t control_code, uint8_t speed, uint8_t color, uint8_t cycle)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
	BM2166_PackHead();
	SendFlag(0x01);								//命令包标识
	SendLength(0x07);
	Sendcmd(0x3C);                /*LED清零*/
	
  BM2166_SendData(0x00);
	BM2166_SendData(0x00);
	BM2166_SendData(color);
	BM2166_SendData(0x00);
	
	temp = 0x01+0x07+0x3C
	+control_code+speed+color+cycle;
	SendCheck(temp);
	
	HAL_Delay(200);
 
	
	BM2166_PackHead();
	SendFlag(0x01);								//命令包标识
	SendLength(0x07);
	Sendcmd(0x3C);                /*光环LED控制*/
	
  BM2166_SendData(control_code);
	BM2166_SendData(speed);
	BM2166_SendData(color);
	BM2166_SendData(cycle);
	
	temp = 0x01+0x07+0x3C
	+control_code+speed+color+cycle;
	SendCheck(temp);
	
	HAL_Delay(200);
	
	data = ReturnFlag(2000);
	
	if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

