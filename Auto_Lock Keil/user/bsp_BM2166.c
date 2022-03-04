#include "bsp_BM2166.h"
#include "LED_Ctrl.h"
#include <stdio.h>
#include <string.h>

uint32_t BM2166_Addr = 0xFFFFFFFF;             /*ָ��ʶ��ģ��Ĭ�ϵ�ַ*/


//���ڽ��ջ����� 	
uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
uint8_t USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			  //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�


/**
  * @brief  BM2166_SendData_USART������ָ��ģ�鴫������ 
  * @param  data;���������
  */
void BM2166_SendData(uint8_t data)
{
  while((USART2->SR & 0X40) == 0);
  USART2->DR = data;
}

/**
  * @brief  BM2166_USART������ָ��ģ�������ͷ��ʽ 
  * @param  ��
  */
void BM2166_PackHead(void)
{ 
  /*��ͷ*/
  BM2166_SendData(0xEF);
  BM2166_SendData(0x01);	
  
  /*ָ��ģ���ַ*/
  BM2166_SendData(BM2166_Addr>>24);
  BM2166_SendData(BM2166_Addr>>16);	
  BM2166_SendData(BM2166_Addr>>8);
  BM2166_SendData(BM2166_Addr);	
}

/**
  * @brief  ���Ͱ���ʶ
  * @param  flag:����ʶλ
  * @retval ��
  */
void SendFlag(uint8_t flag)
{
  BM2166_SendData(flag);
}

/**
  * @brief  ���Ͱ�����
  * @param  length:������
  * @retval ��
  */
void SendLength(uint16_t length)
{
	BM2166_SendData(length>>8);
  BM2166_SendData(length);
}

/**
  * @brief  ����ָ����
  * @param  cmd;ָ����
  * @retval ��
  */
void Sendcmd(uint8_t cmd)
{
	BM2166_SendData(cmd);
}

/**
  * @brief  ����У���
  * @param  check:���λ
  * @retval ��
  */
void SendCheck(uint16_t check)
{
	BM2166_SendData(check>>8);
	BM2166_SendData(check);
}


/**
  * @brief  �ӻ���������ȷ����
	* @param  waittime: �ȴ��жϽ������ݵ�ʱ�䣨��λ1ms��
  * @retval ���ݰ��׵�ַ
  */
uint8_t *ReturnFlag(uint16_t waittime)
{
  char *data;
  uint8_t str[8];
  str[0] = 0xef;
  str[1] = 0x01;
  str[2] = BM2166_Addr >> 24;
  str[3] = BM2166_Addr >> 16;
  str[4] = BM2166_Addr >> 8;
  str[5] = BM2166_Addr;
  str[6] = 0x07;
  str[7] = '\0';
//  USART2_RX_STA = 0;
		//HAL���д��
	HAL_UART_Receive(&huart2,(uint8_t *)USART2_RX_BUF,USART2_MAX_RECV_LEN,waittime/4);//��������������
	if(!memcmp(str,USART2_RX_BUF,7))//�ȶ�����
	{
		  data = strstr((const char*)USART2_RX_BUF, (const char*)str);
      if(data)
        return (uint8_t*)data;
	}
	
	return 0;
}


/**
  * @brief  ��ʾ������Ϣ
  * @param  ensure:ȷ����
  * @retval ��
  */
void  ShowErrMessage(uint16_t ensure) 
{
	PS_AuraLedConfig(breath_LED, 0x01, seven_color, 0);/*�߲���˸����������*/
	HAL_Delay(2000);
  switch(ensure)
	{
    case  0x00:
			//AS608_INFO("OK\r\n");
		break;
		
	  case  0x01:
			//AS608_INFO("���ݰ����մ���\r\n");
		break;
		
	  case  0x02:
	    //AS608_INFO("ָ��ģ��û�м�⵽ָ�ƣ�\r\n");
		break;
		
	  case  0x03:
	    //AS608_INFO("¼��ָ��ͼ��ʧ��\r\n\r\n");
		break;
		
	  case  0x04:
	    //AS608_INFO("ָ��ͼ��̫�ɡ�̫��������������\r\n\r\n");
		break;
	  
		case  0x05:
	    //AS608_INFO("ָ��ͼ��̫ʪ��̫��������������\r\n\r\n");
		break;
		
	  case  0x06:
	    //AS608_INFO("ָ��ͼ��̫�Ҷ�����������\r\n\r\n");
		break;
		
	  case  0x07:
	    //AS608_INFO("ָ��ͼ����������������̫�٣������̫С��������������\r\n");
		break;
		
	  case  0x08:
	    //AS608_INFO("ָ�Ʋ�ƥ��\r\n\r\n");
		break;
		
	  case  0x09:
      //AS608_INFO("�Ա�ָ��ʧ�ܣ�ָ�ƿⲻ���ڴ�ָ�ƣ�\r\n\r\n");
		break;
		
		case  0x0a:
	    //AS608_INFO("�����ϲ�ʧ��\r\n");
		break;
		
		case  0x0b:
      //AS608_INFO("����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ\r\n");
		break;
		
		case  0x10:
	    //AS608_INFO("ɾ��ģ��ʧ��\r\n");
		break;
		
		case  0x11:
      //AS608_INFO("���ָ�ƿ�ʧ��\r\n");
		break;	
		
		case  0x15:
		  //AS608_INFO("��������û����Чԭʼͼ��������ͼ��\r\n");
		break;
		
		case  0x18:
		  //AS608_INFO("��д FLASH ����\r\n");
		break;
		
		case  0x19:
	    //AS608_INFO("δ�������\r\n");
		break;
		
		case  0x1a:
	    //AS608_INFO("��Ч�Ĵ�����\r\n");
		break;
		
		case  0x1b:
			//AS608_INFO("�Ĵ����趨���ݴ���\r\n");
		break;
		
		case  0x1c:
		  //AS608_INFO("���±�ҳ��ָ������\r\n");
		break;
		
		case  0x1f:
      //AS608_INFO("ָ�ƿ���\r\n");
		break;
		
		case  0x20:
      //AS608_INFO("��ַ����\r\n");
		break;
		
		default :
      //AS608_INFO("ģ�鷵��ȷ��������\r\n");
		break;	
  }

}


//////////////////////////////////////////////////////////////////////////
/*����ָ���*/

/**
  * @brief   ע��ָ��ʱ̽����ָ��̽�⵽��¼��ָ��ͼ����� ImageBuffer
  * @param   ��
  * @retval  ȷ����=00H ��ʾ��ȡͼ��ȷ����=01H ��ʾ�հ��д�
             ȷ����=02H ��ʾ������������ָ��ȷ����=03H ��ʾ¼�벻�ɹ� 
  */
uint16_t PS_GetEnrollImage(void)
{
	uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	//AS608_DELAY_MS(3000);  �Ȳ�����ʱ���ˣ�������ָ��ģ�黽�ѿڻ�������ָ��     /*��ָ�����붯��Ԥ��ʱ��*/
	HAL_Delay(2500);
	
	BM2166_PackHead();
	SendFlag(0x01);             /*�������ʶ*/
	SendLength(0x03);
	Sendcmd(0x29);              /*¼ָ��ָ��*/
  temp=0x01+0x03+0x29;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}

/**
  * @brief   ��ָ֤��ʱ̽����ָ��̽�⵽��¼��ָ��ͼ����� ImageBuffer
  * @param   ��
  * @retval  ȷ����=00H ��ʾ��ȡͼ��ȷ����=01H ��ʾ�հ��д�
             ȷ����=02H ��ʾ������������ָ��ȷ����=03H ��ʾ¼�벻�ɹ� 
  */
uint16_t PS_GetImage(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	//AS608_DELAY_MS(3000);  �Ȳ�����ʱ���ˣ�������ָ��ģ�黽�ѿڻ�������ָ��     /*��ָ�����붯��Ԥ��ʱ��*/
	HAL_Delay(2500);
	
	BM2166_PackHead();
	SendFlag(0x01);             /*�������ʶ*/
	SendLength(0x03);
	Sendcmd(0x01);              /*¼ָ��ָ��*/
  temp=0x01+0x03+0x01;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}


/**
  * @brief  ��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CHARBUFFER1��CHARBUFFER2
  * @param  BufferID(������������)
  * @retval ȷ����=00H ��ʾ���������ɹ���ȷ����=01H ��ʾ�հ��д�
            ȷ����=06H ��ʾָ��ͼ��̫�Ҷ�������������ȷ����=07H ��ʾָ��ͼ����������������̫�ٶ�������������
            ȷ����=15H ��ʾͼ�񻺳�����û����Чԭʼͼ��������ͼ��
  */
uint16_t PS_GenChar(uint8_t BufferID)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
	BM2166_PackHead();
	SendFlag(0x01);             /*�������ʶ*/
	SendLength(0x04);
	Sendcmd(0x02);              /*��������ָ��*/
	BM2166_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}

/**
  * @brief  ��ȷ�ȶ� CHARBUFFER1��CHARBUFFER2�е������ļ�
  * @param  ��
  * @retval ȷ����=00H ��ʾָ��ƥ�䣻ȷ����=01H ��ʾ�հ��д�ȷ����=08H ��ʾָ�Ʋ�ƥ��
  */
uint16_t PS_Match(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);             /*�������ʶ*/
	SendLength(0x03);
	Sendcmd(0x03);              /*��ȷ�ȶ�ָ��*/
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}


/**
  * @brief  ��CHARBUFFER1��CHARBUFFER2 �е������ļ��ϲ�����ģ�壬
            �������CHARBUFFER1��CHARBUFFER2��
  * @param  ��
  * @retval ȷ����=00H ��ʾ�ϲ��ɹ���ȷ����=01H ��ʾ�հ��д�
            ȷ����=0aH ��ʾ�ϲ�ʧ�ܣ���öָ�Ʋ�����ͬһ��ָ��
  */
uint16_t PS_RegModel(void)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);             /*�������ʶ*/
	SendLength(0x03);
	Sendcmd(0x05);              /*�ϲ�����ָ��*/
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}


/**
  * @brief  �� CHARBUFFER1 �� CHARBUFFER2 �е�ģ���ļ��浽 PageID ��flash ���ݿ�λ�á�
  * @param  BufferID:�������ţ�
  * @param 	PageID:ָ�ƿ�λ�ú�
  * @retval ȷ����=00H ��ʾ����ɹ���ȷ����=01H ��ʾ�հ��д�
            ȷ����=0bH ��ʾ PageID ����ָ�ƿⷶΧ��ȷ����=18H ��ʾд FLASH ����
  */
uint16_t PS_StoreChar(uint8_t BufferID,uint16_t PageID)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);								/*�������ʶ*/
	SendLength(0x06);
	Sendcmd(0x06);                /*�洢ģ��ָ��*/
	BM2166_SendData(BufferID);
  BM2166_SendData(PageID>>8);
	BM2166_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	       +(PageID>>8)+(uint8_t)PageID;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}


/**
  * @brief  ɾ�� flash ���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
  * @param  PageID:ָ�ƿ�ģ���
  * @param  N:ɾ����ģ�����
  * @retval ȷ����=00H ��ʾɾ��ģ��ɹ���ȷ����=01H ��ʾ�հ��д�
            ȷ����=10H ��ʾɾ��ģ��ʧ��
  */
uint16_t PS_DeletChar(uint16_t PageID,uint16_t N)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);								//�������ʶ
	SendLength(0x07);
	Sendcmd(0x0C);                /*ɾ��ָ��ָ��ģ��ָ��*/
	BM2166_SendData(PageID>>8);
  BM2166_SendData(PageID);
	BM2166_SendData(N>>8);
	BM2166_SendData(N);
	temp = 0x01+0x07+0x0C
	       +(PageID>>8)+(uint8_t)PageID
	       +(N>>8)+(uint8_t)N;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}


/**
  * @brief  ɾ�� flash ���ݿ�������ָ��ģ��
  * @param  ��
  * @retval ȷ����=00H ��ʾ��ճɹ���ȷ����=01H ��ʾ�հ��д�ȷ����=11H ��ʾ���ʧ��
  */
uint16_t  PS_Empty(void)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);				//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0D);				/*ɾ��ȫ��ָ��ģ��ָ��*/
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	
  data = ReturnFlag(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xFF;
  return ensure;
}



/**
  * @brief  ��CHARBUFFER1��CHARBUFFER2�е������ļ��������������򲿷�ָ�ƿ�
  * @param  BufferID:��������
  * @param  StartPage:��ʼҳ
  * @param  PageNum:ҳ��
  * @param  p:����ָ��
  * @retval ensure:ȷ����,*p:ҳ�루����ָ��ģ�壩
  */
uint16_t PS_HighSpeedSearch(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, uint16_t *p)
{
  uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
  BM2166_PackHead();
	SendFlag(0x01);
	SendLength(0x08);
	Sendcmd(0x1b);                 /*��������ָ�ƿ�ָ��*/
	BM2166_SendData(BufferID);
	BM2166_SendData(StartPage>>8);
	BM2166_SendData(StartPage);
	BM2166_SendData(PageNum>>8);
	BM2166_SendData(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
	       +(StartPage>>8)+(uint8_t)StartPage
	       +(PageNum>>8)+(uint8_t)PageNum;
	SendCheck(temp);
	
	
  data = ReturnFlag(2000);
  if(data)
  {
    ensure = data[9];
    *p = (data[10] << 8) + data[11];
		//p->pageID 	= (data[10] << 8) + data[11];
    //p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}



//����ָ�� PS_Search
//����:��CharBuffer1��CharBuffer2�е������ļ����������򲿷�ָ�ƿ�.�����������򷵻�ҳ�롣
//����:  BufferID @ref CharBuffer1	CharBuffer2
//˵��:  ģ�鷵��ȷ���֣�ҳ�루����ָ��ģ�壩
uint16_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, uint16_t *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
  BM2166_PackHead();
	SendFlag(0x01);
	SendLength(0x08);
	Sendcmd(0x04);                 /*����ָ�ƿ�ָ��*/
  BM2166_SendData(BufferID);
  BM2166_SendData(StartPage >> 8);
  BM2166_SendData(StartPage);
  BM2166_SendData(PageNum >> 8);
  BM2166_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x04 + BufferID
         + (StartPage >> 8) + (uint8_t)StartPage
         + (PageNum >> 8) + (uint8_t)PageNum;
  SendCheck(temp);
	
	
  data = ReturnFlag(2000);
  if(data)
  {
    ensure = data[9];
		*p = (data[10] << 8) + data[11];
    //p->pageID   = (data[10] << 8) + data[11];
    //p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}






