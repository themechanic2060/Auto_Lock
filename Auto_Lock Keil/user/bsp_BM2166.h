#ifndef BSP_BM2166_H
#define BSP_BM2166_H

#include "struct_typedef.h"
#include "stm32f1xx_hal.h"


#define CharBuffer1 	0x01
#define CharBuffer2 	0x02

#define PS_MAXNUM   240          /*指纹模块最大存储指纹模板数*/


extern uint32_t BM2166_Addr;
extern UART_HandleTypeDef huart2;

#define USART2_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		400					//最大发送缓存字节数


typedef struct  
	{
		uint16_t pageID;//指纹ID
		uint16_t mathscore;//匹配得分
	}SearchResult;
	
typedef struct
	{
		uint16_t	PS_max;//指纹最大容量
		uint8_t		PS_level;//安全等级
		uint32_t	PS_addr;
		uint8_t		PS_size;//通讯数据包大小
		uint8_t		PS_N;//波特率基数N
	}SysPara;

	void BM2166_SendData(uint8_t data);
	
	void BM2166_PackHead(void);
	
	void SendFlag(uint8_t flag);
	
	void SendLength(uint16_t length);
	
	void Sendcmd(uint8_t cmd);
	
	void SendCheck(uint16_t check);
	
	uint8_t *ReturnFlag(uint16_t waittime);
	
	void ShowErrMessage(uint16_t ensure);
	
/*操作指令部分*/
	uint16_t PS_GetEnrollImage(void);//注册用获取图像
	
	uint16_t PS_GetImage(void);	//验证用录入图像
	
	uint16_t PS_GenChar(uint8_t BufferID);//生成特征 
	
	uint16_t PS_Match(void);//精确比对两枚指纹特征
	
	uint16_t PS_RegModel(void);//合并特征（生成模板）
	
	uint16_t PS_StoreChar(uint8_t BufferID,uint16_t PageID);//储存模板 
	
	uint16_t PS_DeletChar(uint16_t PageID,uint16_t N);//删除模板
	
	uint16_t PS_Empty(void);//清空指纹库

	uint16_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,uint16_t *p);//高速搜索
	
	uint16_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, uint16_t *p);//搜索
	
	
	
#endif
