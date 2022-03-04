#include "LED_Ctrl.h"
#include "bsp_BM2166.h"

/**
  * @brief  ����LED����
  * @param  control_code:������
  * @param  speed				:LED�ٶ� (range: 0-0xFF)
  * @param  color				:LED��ɫ
  * @param  cycle				:ѭ������	(range: 0-255, 0Ϊ����ѭ��)
  * @retval	ȷ����=00H ��ʾ������֤��ȷ��
						ȷ����=01H ��ʾ�հ��д�

  */
uint16_t PS_AuraLedConfig(uint8_t control_code, uint8_t speed, uint8_t color, uint8_t cycle)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
	BM2166_PackHead();
	SendFlag(0x01);								//�������ʶ
	SendLength(0x07);
	Sendcmd(0x3C);                /*LED����*/
	
  BM2166_SendData(0x00);
	BM2166_SendData(0x00);
	BM2166_SendData(color);
	BM2166_SendData(0x00);
	
	temp = 0x01+0x07+0x3C
	+control_code+speed+color+cycle;
	SendCheck(temp);
	
	HAL_Delay(200);
 
	
	BM2166_PackHead();
	SendFlag(0x01);								//�������ʶ
	SendLength(0x07);
	Sendcmd(0x3C);                /*�⻷LED����*/
	
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

