#include "FP_Task.h"
#include "key.h"
#include "bsp_BM2166.h"
#include "LED_Ctrl.h"

/**
  * @brief  指纹传感器触摸信号中断工作
  * @param  无
  * @retval 无
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	FR_Task();
}


/**
  * @brief  指纹模块操作
  * @param  无
  * @retval 无
  */
void  FR_Task(void)
{	
	
  uint8_t  flag;
  flag = key_scan();
	
  switch(flag)
  {
		case 0:
			break;
		
    case 1:
      Add_FR();                                    /*添加指纹*/
		  
    break;
			
		case 2:
      Compare_FR();                                /*比对指纹*/
		 
    break;

    case 3:
      Clean_FR();                                  /*清空指纹库*/
		  
    break;
			
    case 4:
      //Del_FR();                                    /*删除指定用户指纹*/
		  
    break;
   
    default:
      /* 如果不是指定指令字符，打印提示信息 */
		  //AS608_INFO("请输入合法指令！\r\n\r\n");
      //Show_Message();
    break;      
  }
  return;		
}


/**
  * @brief  录指纹
  * @param  无
  * @retval 无
  */
void  Add_FR(void)
{
  uint16_t  step,count,sure,ID;
 
  step=count=0;
	
  while(1)
  {
    switch (step)
    {
      case 0:                            /*执行第1步*/
        
        count++;
      
        //AS608_INFO("******命令：请按手指******\r\n")
				PS_AuraLedConfig(blink_LED, 0x01, white, 0);/*白色快闪，请按手指*/
			
        
        sure=PS_GetEnrollImage();              /*录入图像*/      
        if(sure == 0x00)
        {
          sure=PS_GenChar(CharBuffer1);  /*生成特征1*/
          if(sure==0x00)
          { 
				    //AS608_INFO("输入指纹1正常！！！\r\n");
						PS_AuraLedConfig(blink_LED, 0x01, green, 2);/*绿色快闪两次，输入指纹1正常*/
            
            //sure=PS_HighSpeedSearch(CharBuffer1,0,PS_MAXNUM,&ID);
						sure = PS_Search(CharBuffer1,0x00,PS_MAXNUM,&ID);
						
            if(sure==0x00)
            {
             //AS608_INFO ("指纹已存在，指纹ID：%d！！！\r\n\r\n",ID);
             PS_AuraLedConfig(LED_on, 0x01, red, 0);//红色常亮，指纹已存在
             return ;
            }              
            else 
            {
							HAL_Delay(1000);
              count=0;
              step=1;                   //跳转到第2步
             }
						
           }
				 else
				 {
					 ShowErrMessage(sure);	       
				 }
        }
			  else 
			  {
				  ShowErrMessage(sure);	
			  }
      break;


      case 1:
        
        count++;
      
        //AS608_INFO("******命令：请再按一次手指******\r\n");
				PS_AuraLedConfig(blink_LED,0x01, white, 0);/*白色快闪，请按手指*/
      
        sure=PS_GetEnrollImage();
        if(sure==0x00)
        { 
          sure=PS_GenChar(CharBuffer2);  /*生成特征2*/
          if(sure==0x00)
          { 
				    //AS608_INFO("输入指纹2正常！！！\r\n");
						PS_AuraLedConfig(blink_LED, 0x01, green, 2);/*绿色快闪两次，输入指纹2正常*/
						HAL_Delay(1000);
            count=0;
            step=2;                   /*跳转到第3步*/
          }
				  else 
				  {
					  ShowErrMessage(sure);
				  }

         }
			   else 
			   {
			   ShowErrMessage(sure);
         }				
       break;

			
       case 2:
         
         //AS608_INFO("正在对比两次输入的指纹\r\n");
					PS_AuraLedConfig(breath_LED,0x0F, sky_blue, 0);/*天蓝色循环，正在比对*/
       
					sure=PS_Match();                /*精确比对两枚指纹特征*/
			 
					if(sure==0x00) 
						{
							//AS608_INFO("输入指纹对比成功！！！\r\n");
           
							step=3;                         /*跳转到第4步*/
						}
					else
						{
							PS_AuraLedConfig(blink_LED, 0x01, red, 3);/*红色闪烁三次，对比失败*/
							//AS608_INFO("对比失败，请重新录入指纹！！！\r\n");
							HAL_Delay(1000);
							ShowErrMessage(sure);
							count=0;
							step=0;
						}
        
       break;
         
         
       case 3:
         
         //AS608_INFO("正在生成指纹模块\r\n");
					//继续天蓝色呼吸，表示正在处理
       
         sure=PS_RegModel();            /*合并特征（生成模板）*/
         if(sure==0x00) 	
         {
           //AS608_INFO("生成指纹模块成功！！！\r\n");
					 PS_AuraLedConfig(LED_on, 0x0F, sky_blue, 0);/*天蓝色常亮，生成指纹模板成功*/
					 HAL_Delay(1000);
					 
           step=4;                        /*跳转到第5步*/
         } 
         else 
        {
         step =0;
         ShowErrMessage(sure);
        }
        
       break;
        

       case 4:
        
         sure=PS_StoreChar(CharBuffer2,FP_ID);  /*储存模板*/
         if(sure==0x00) 	
         {
           //AS608_INFO("录入指纹模板成功！！！\r\n\r\n");
					 PS_AuraLedConfig(blink_LED, 0x01, pink, 3);/*粉色闪烁3次，储存模板成功*/
					 FP_ID++;
					 HAL_Delay(1000);
      
           return ;
         } 
         else 
         {
           step =0;
           ShowErrMessage(sure);
         }
       break;
      }
    
      HAL_Delay(1000);
      
      if(count==4)                       /*超过4次没有按手指则退出*/
      { 
        //AS608_INFO("录指纹失败！\r\n\r\n\r\n");
				PS_AuraLedConfig(LED_on, 0x01, yellow, 3);
				HAL_Delay(1000);

        break;
      }
  }
}






/**
  * @brief  比对指纹（识别指纹）
  * @param  无
  * @retval 无
  */
void Compare_FR(void)  
{  
  uint16_t  ID=0;                    /*初始化ID值*/  
  uint16_t  sure;
	
  //AS608_INFO("******命令：对比指纹，请按手指******\r\n");
	PS_AuraLedConfig(blink_LED, 0x01, white, 0);/*白色快闪，请按手指*/
	

  sure=PS_GetImage();
  if(sure == 0x00)
  {
    sure=PS_GenChar(CharBuffer1);    /*生成特征1*/
    if(sure==0x00)
    {  
			/*高速搜索指纹库*/
      //sure=PS_HighSpeedSearch(CharBuffer1,0,PS_MAXNUM,&ID);
			sure=PS_Search(CharBuffer1,0,PS_MAXNUM,&ID);
			
      if(sure==0x00)
      {
        //AS608_INFO ("对比指纹成功，指纹ID：%d！！！\r\n\r\n",ID);
				PS_AuraLedConfig(LED_on, 0x01, green, 0);/*绿色常亮，验证成功*/
				
      }
      else 
      {
			ShowErrMessage(sure);
			}
    }
    else 
    {   
      ShowErrMessage(sure);	
    }
  }
	else  
	{
	  ShowErrMessage(sure);
	}
 
}



/**
  * @brief  清空指纹库
  * @param  无
  * @retval 无
  */
void Clean_FR(void) 
{
  uint16_t  sure;
  
	//AS608_INFO("清空指纹库\r\n");
	PS_AuraLedConfig(LED_on, 0x01, yellow, 3);
  
	sure=PS_Empty();     
	if(sure == 0x00)
	{
		//AS608_INFO("清空指纹库成功！！！\r\n\r\n");
		PS_AuraLedConfig(blink_LED, 0x01, pink, 3);
	}
	else 
	{
		ShowErrMessage(sure);
	}

}
