#ifndef BSP_BM2166_H
#define BSP_BM2166_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"


#define CharBuffer1 	0x01
#define CharBuffer2 	0x02

#define PS_MAXNUM   240          /*ָ��ģ�����洢ָ��ģ����*/


extern uint32_t BM2166_Addr;
extern UART_HandleTypeDef huart2;

#define USART2_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		400					//����ͻ����ֽ���


typedef struct  
	{
		uint16_t pageID;//ָ��ID
		uint16_t mathscore;//ƥ��÷�
	}SearchResult;
	
typedef struct
	{
		uint16_t	PS_max;//ָ���������
		uint8_t		PS_level;//��ȫ�ȼ�
		uint32_t	PS_addr;
		uint8_t		PS_size;//ͨѶ���ݰ���С
		uint8_t		PS_N;//�����ʻ���N
	}SysPara;

	void BM2166_SendData(uint8_t data);
	
	void BM2166_PackHead(void);
	
	void SendFlag(uint8_t flag);
	
	void SendLength(uint16_t length);
	
	void Sendcmd(uint8_t cmd);
	
	void SendCheck(uint16_t check);
	
	uint8_t *ReturnFlag(uint16_t waittime);
	
	void ShowErrMessage(uint16_t ensure);
	
/*����ָ���*/
	uint16_t PS_GetEnrollImage(void);//ע���û�ȡͼ��
	
	uint16_t PS_GetImage(void);	//��֤��¼��ͼ��
	
	uint16_t PS_GenChar(uint8_t BufferID);//�������� 
	
	uint16_t PS_Match(void);//��ȷ�ȶ���öָ������
	
	uint16_t PS_RegModel(void);//�ϲ�����������ģ�壩
	
	uint16_t PS_StoreChar(uint8_t BufferID,uint16_t PageID);//����ģ�� 
	
	uint16_t PS_DeletChar(uint16_t PageID,uint16_t N);//ɾ��ģ��
	
	uint16_t PS_Empty(void);//���ָ�ƿ�

	uint16_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,uint16_t *p);//��������
	
	uint16_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, uint16_t *p);//����
	
	
	
#endif
