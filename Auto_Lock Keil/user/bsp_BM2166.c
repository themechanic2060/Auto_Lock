#include "bsp_BM2166.h"
#include "LED_Ctrl.h"
#include <stdio.h>
#include <string.h>

uint32_t BM2166_Addr = 0xFFFFFFFF;             /*指纹识别模块默认地址*/


//串口接收缓存区 	
uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
uint8_t USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节


/**
  * @brief  BM2166_SendData_USART串口向指纹模块传递数据 
  * @param  data;传输的数据
  */
void BM2166_SendData(uint8_t data)
{
  while((USART2->SR & 0X40) == 0);
  USART2->DR = data;
}

/**
  * @brief  BM2166_USART串口向指纹模块命令包头格式 
  * @param  无
  */
void BM2166_PackHead(void)
{ 
  /*包头*/
  BM2166_SendData(0xEF);
  BM2166_SendData(0x01);	
  
  /*指纹模块地址*/
  BM2166_SendData(BM2166_Addr>>24);
  BM2166_SendData(BM2166_Addr>>16);	
  BM2166_SendData(BM2166_Addr>>8);
  BM2166_SendData(BM2166_Addr);	
}

/**
  * @brief  发送包标识
  * @param  flag:包标识位
  * @retval 无
  */
void SendFlag(uint8_t flag)
{
  BM2166_SendData(flag);
}

/**
  * @brief  发送包长度
  * @param  length:包长度
  * @retval 无
  */
void SendLength(uint16_t length)
{
	BM2166_SendData(length>>8);
  BM2166_SendData(length);
}

/**
  * @brief  发送指令码
  * @param  cmd;指令码
  * @retval 无
  */
void Sendcmd(uint8_t cmd)
{
	BM2166_SendData(cmd);
}

/**
  * @brief  发送校验和
  * @param  check:检查位
  * @retval 无
  */
void SendCheck(uint16_t check)
{
	BM2166_SendData(check>>8);
	BM2166_SendData(check);
}


/**
  * @brief  从缓冲区读出确认码
	* @param  waittime: 等待中断接收数据的时间（单位1ms）
  * @retval 数据包首地址
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
		//HAL库的写法
	HAL_UART_Receive(&huart2,(uint8_t *)USART2_RX_BUF,USART2_MAX_RECV_LEN,waittime/4);//串口三接收数据
	if(!memcmp(str,USART2_RX_BUF,7))//比对数据
	{
		  data = strstr((const char*)USART2_RX_BUF, (const char*)str);
      if(data)
        return (uint8_t*)data;
	}
	
	return 0;
}


/**
  * @brief  显示错误信息
  * @param  ensure:确认码
  * @retval 无
  */
void  ShowErrMessage(uint16_t ensure) 
{
	PS_AuraLedConfig(breath_LED, 0x01, seven_color, 0);/*七彩闪烁，出现问题*/
	HAL_Delay(2000);
  switch(ensure)
	{
    case  0x00:
			//AS608_INFO("OK\r\n");
		break;
		
	  case  0x01:
			//AS608_INFO("数据包接收错误\r\n");
		break;
		
	  case  0x02:
	    //AS608_INFO("指纹模块没有检测到指纹！\r\n");
		break;
		
	  case  0x03:
	    //AS608_INFO("录入指纹图像失败\r\n\r\n");
		break;
		
	  case  0x04:
	    //AS608_INFO("指纹图像太干、太淡而生不成特征\r\n\r\n");
		break;
	  
		case  0x05:
	    //AS608_INFO("指纹图像太湿、太糊而生不成特征\r\n\r\n");
		break;
		
	  case  0x06:
	    //AS608_INFO("指纹图像太乱而生不成特征\r\n\r\n");
		break;
		
	  case  0x07:
	    //AS608_INFO("指纹图像正常，但特征点太少（或面积太小）而生不成特征\r\n");
		break;
		
	  case  0x08:
	    //AS608_INFO("指纹不匹配\r\n\r\n");
		break;
		
	  case  0x09:
      //AS608_INFO("对比指纹失败，指纹库不存在此指纹！\r\n\r\n");
		break;
		
		case  0x0a:
	    //AS608_INFO("特征合并失败\r\n");
		break;
		
		case  0x0b:
      //AS608_INFO("访问指纹库时地址序号超出指纹库范围\r\n");
		break;
		
		case  0x10:
	    //AS608_INFO("删除模板失败\r\n");
		break;
		
		case  0x11:
      //AS608_INFO("清空指纹库失败\r\n");
		break;	
		
		case  0x15:
		  //AS608_INFO("缓冲区内没有有效原始图而生不成图像\r\n");
		break;
		
		case  0x18:
		  //AS608_INFO("读写 FLASH 出错\r\n");
		break;
		
		case  0x19:
	    //AS608_INFO("未定义错误\r\n");
		break;
		
		case  0x1a:
	    //AS608_INFO("无效寄存器号\r\n");
		break;
		
		case  0x1b:
			//AS608_INFO("寄存器设定内容错误\r\n");
		break;
		
		case  0x1c:
		  //AS608_INFO("记事本页码指定错误\r\n");
		break;
		
		case  0x1f:
      //AS608_INFO("指纹库满\r\n");
		break;
		
		case  0x20:
      //AS608_INFO("地址错误\r\n");
		break;
		
		default :
      //AS608_INFO("模块返回确认码有误\r\n");
		break;	
  }

}


//////////////////////////////////////////////////////////////////////////
/*操作指令部分*/

/**
  * @brief   注册指纹时探测手指，探测到后录入指纹图像存于 ImageBuffer
  * @param   无
  * @retval  确认码=00H 表示获取图像；确认码=01H 表示收包有错；
             确认码=02H 表示传感器上无手指；确认码=03H 表示录入不成功 
  */
uint16_t PS_GetEnrollImage(void)
{
	uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	//AS608_DELAY_MS(3000);  先不留了时间了，期望是指纹模块唤醒口唤起输入指纹     /*给指纹输入动作预留时间*/
	HAL_Delay(2500);
	
	BM2166_PackHead();
	SendFlag(0x01);             /*命令包标识*/
	SendLength(0x03);
	Sendcmd(0x29);              /*录指纹指令*/
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
  * @brief   验证指纹时探测手指，探测到后录入指纹图像存于 ImageBuffer
  * @param   无
  * @retval  确认码=00H 表示获取图像；确认码=01H 表示收包有错；
             确认码=02H 表示传感器上无手指；确认码=03H 表示录入不成功 
  */
uint16_t PS_GetImage(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	//AS608_DELAY_MS(3000);  先不留了时间了，期望是指纹模块唤醒口唤起输入指纹     /*给指纹输入动作预留时间*/
	HAL_Delay(2500);
	
	BM2166_PackHead();
	SendFlag(0x01);             /*命令包标识*/
	SendLength(0x03);
	Sendcmd(0x01);              /*录指纹指令*/
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
  * @brief  将ImageBuffer中的原始图像生成指纹特征文件存于CHARBUFFER1或CHARBUFFER2
  * @param  BufferID(特征缓冲区号)
  * @retval 确认码=00H 表示生成特征成功；确认码=01H 表示收包有错；
            确认码=06H 表示指纹图像太乱而生不成特征；确认码=07H 表示指纹图像正常，但特征点太少而生不成特征；
            确认码=15H 表示图像缓冲区内没有有效原始图而生不成图像
  */
uint16_t PS_GenChar(uint8_t BufferID)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
	BM2166_PackHead();
	SendFlag(0x01);             /*命令包标识*/
	SendLength(0x04);
	Sendcmd(0x02);              /*生成特征指令*/
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
  * @brief  精确比对 CHARBUFFER1与CHARBUFFER2中的特征文件
  * @param  无
  * @retval 确认码=00H 表示指纹匹配；确认码=01H 表示收包有错；确认码=08H 表示指纹不匹配
  */
uint16_t PS_Match(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);             /*命令包标识*/
	SendLength(0x03);
	Sendcmd(0x03);              /*精确比对指令*/
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
  * @brief  将CHARBUFFER1与CHARBUFFER2 中的特征文件合并生成模板，
            结果存于CHARBUFFER1与CHARBUFFER2。
  * @param  无
  * @retval 确认码=00H 表示合并成功；确认码=01H 表示收包有错；
            确认码=0aH 表示合并失败（两枚指纹不属于同一手指）
  */
uint16_t PS_RegModel(void)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);             /*命令包标识*/
	SendLength(0x03);
	Sendcmd(0x05);              /*合并特征指令*/
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
  * @brief  将 CHARBUFFER1 或 CHARBUFFER2 中的模板文件存到 PageID 号flash 数据库位置。
  * @param  BufferID:缓冲区号，
  * @param 	PageID:指纹库位置号
  * @retval 确认码=00H 表示储存成功；确认码=01H 表示收包有错；
            确认码=0bH 表示 PageID 超出指纹库范围；确认码=18H 表示写 FLASH 出错
  */
uint16_t PS_StoreChar(uint8_t BufferID,uint16_t PageID)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);								/*命令包标识*/
	SendLength(0x06);
	Sendcmd(0x06);                /*存储模板指令*/
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
  * @brief  删除 flash 数据库中指定ID号开始的N个指纹模板
  * @param  PageID:指纹库模板号
  * @param  N:删除的模板个数
  * @retval 确认码=00H 表示删除模板成功；确认码=01H 表示收包有错；
            确认码=10H 表示删除模板失败
  */
uint16_t PS_DeletChar(uint16_t PageID,uint16_t N)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);								//命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);                /*删除指定指纹模板指令*/
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
  * @brief  删除 flash 数据库中所有指纹模板
  * @param  无
  * @retval 确认码=00H 表示清空成功；确认码=01H 表示收包有错；确认码=11H 表示清空失败
  */
uint16_t  PS_Empty(void)
{
	uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	BM2166_PackHead();
	SendFlag(0x01);				//命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);				/*删除全部指纹模板指令*/
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
  * @brief  以CHARBUFFER1或CHARBUFFER2中的特征文件高速搜索整个或部分指纹库
  * @param  BufferID:缓冲区号
  * @param  StartPage:起始页
  * @param  PageNum:页数
  * @param  p:传递指针
  * @retval ensure:确认字,*p:页码（相配指纹模板）
  */
uint16_t PS_HighSpeedSearch(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, uint16_t *p)
{
  uint16_t 	temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
  BM2166_PackHead();
	SendFlag(0x01);
	SendLength(0x08);
	Sendcmd(0x1b);                 /*高速搜索指纹库指令*/
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



//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
uint16_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, uint16_t *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
	
	
  BM2166_PackHead();
	SendFlag(0x01);
	SendLength(0x08);
	Sendcmd(0x04);                 /*搜索指纹库指令*/
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






