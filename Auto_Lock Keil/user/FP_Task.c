#include "FP_Task.h"
#include "key.h"
#include "bsp_BM2166.h"
#include "LED_Ctrl.h"

/**
  * @brief  ָ�ƴ����������ź��жϹ���
  * @param  ��
  * @retval ��
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	FR_Task();
}


/**
  * @brief  ָ��ģ�����
  * @param  ��
  * @retval ��
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
      Add_FR();                                    /*���ָ��*/
		  
    break;
			
		case 2:
      Compare_FR();                                /*�ȶ�ָ��*/
		 
    break;

    case 3:
      Clean_FR();                                  /*���ָ�ƿ�*/
		  
    break;
			
    case 4:
      //Del_FR();                                    /*ɾ��ָ���û�ָ��*/
		  
    break;
   
    default:
      /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
		  //AS608_INFO("������Ϸ�ָ�\r\n\r\n");
      //Show_Message();
    break;      
  }
  return;		
}


/**
  * @brief  ¼ָ��
  * @param  ��
  * @retval ��
  */
void  Add_FR(void)
{
  uint16_t  step,count,sure,ID;
 
  step=count=0;
	
  while(1)
  {
    switch (step)
    {
      case 0:                            /*ִ�е�1��*/
        
        count++;
      
        //AS608_INFO("******����밴��ָ******\r\n")
				PS_AuraLedConfig(blink_LED, 0x01, white, 0);/*��ɫ�������밴��ָ*/
			
        
        sure=PS_GetEnrollImage();              /*¼��ͼ��*/      
        if(sure == 0x00)
        {
          sure=PS_GenChar(CharBuffer1);  /*��������1*/
          if(sure==0x00)
          { 
				    //AS608_INFO("����ָ��1����������\r\n");
						PS_AuraLedConfig(blink_LED, 0x01, green, 2);/*��ɫ�������Σ�����ָ��1����*/
            
            //sure=PS_HighSpeedSearch(CharBuffer1,0,PS_MAXNUM,&ID);
						sure = PS_Search(CharBuffer1,0x00,PS_MAXNUM,&ID);
						
            if(sure==0x00)
            {
             //AS608_INFO ("ָ���Ѵ��ڣ�ָ��ID��%d������\r\n\r\n",ID);
             PS_AuraLedConfig(LED_on, 0x01, red, 0);//��ɫ������ָ���Ѵ���
             return ;
            }              
            else 
            {
							HAL_Delay(1000);
              count=0;
              step=1;                   //��ת����2��
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
      
        //AS608_INFO("******������ٰ�һ����ָ******\r\n");
				PS_AuraLedConfig(blink_LED,0x01, white, 0);/*��ɫ�������밴��ָ*/
      
        sure=PS_GetEnrollImage();
        if(sure==0x00)
        { 
          sure=PS_GenChar(CharBuffer2);  /*��������2*/
          if(sure==0x00)
          { 
				    //AS608_INFO("����ָ��2����������\r\n");
						PS_AuraLedConfig(blink_LED, 0x01, green, 2);/*��ɫ�������Σ�����ָ��2����*/
						HAL_Delay(1000);
            count=0;
            step=2;                   /*��ת����3��*/
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
         
         //AS608_INFO("���ڶԱ����������ָ��\r\n");
					PS_AuraLedConfig(breath_LED,0x0F, sky_blue, 0);/*����ɫѭ�������ڱȶ�*/
       
					sure=PS_Match();                /*��ȷ�ȶ���öָ������*/
			 
					if(sure==0x00) 
						{
							//AS608_INFO("����ָ�ƶԱȳɹ�������\r\n");
           
							step=3;                         /*��ת����4��*/
						}
					else
						{
							PS_AuraLedConfig(blink_LED, 0x01, red, 3);/*��ɫ��˸���Σ��Ա�ʧ��*/
							//AS608_INFO("�Ա�ʧ�ܣ�������¼��ָ�ƣ�����\r\n");
							HAL_Delay(1000);
							ShowErrMessage(sure);
							count=0;
							step=0;
						}
        
       break;
         
         
       case 3:
         
         //AS608_INFO("��������ָ��ģ��\r\n");
					//��������ɫ��������ʾ���ڴ���
       
         sure=PS_RegModel();            /*�ϲ�����������ģ�壩*/
         if(sure==0x00) 	
         {
           //AS608_INFO("����ָ��ģ��ɹ�������\r\n");
					 PS_AuraLedConfig(LED_on, 0x0F, sky_blue, 0);/*����ɫ����������ָ��ģ��ɹ�*/
					 HAL_Delay(1000);
					 
           step=4;                        /*��ת����5��*/
         } 
         else 
        {
         step =0;
         ShowErrMessage(sure);
        }
        
       break;
        

       case 4:
        
         sure=PS_StoreChar(CharBuffer2,FP_ID);  /*����ģ��*/
         if(sure==0x00) 	
         {
           //AS608_INFO("¼��ָ��ģ��ɹ�������\r\n\r\n");
					 PS_AuraLedConfig(blink_LED, 0x01, pink, 3);/*��ɫ��˸3�Σ�����ģ��ɹ�*/
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
      
      if(count==4)                       /*����4��û�а���ָ���˳�*/
      { 
        //AS608_INFO("¼ָ��ʧ�ܣ�\r\n\r\n\r\n");
				PS_AuraLedConfig(LED_on, 0x01, yellow, 3);
				HAL_Delay(1000);

        break;
      }
  }
}






/**
  * @brief  �ȶ�ָ�ƣ�ʶ��ָ�ƣ�
  * @param  ��
  * @retval ��
  */
void Compare_FR(void)  
{  
  uint16_t  ID=0;                    /*��ʼ��IDֵ*/  
  uint16_t  sure;
	
  //AS608_INFO("******����Ա�ָ�ƣ��밴��ָ******\r\n");
	PS_AuraLedConfig(blink_LED, 0x01, white, 0);/*��ɫ�������밴��ָ*/
	

  sure=PS_GetImage();
  if(sure == 0x00)
  {
    sure=PS_GenChar(CharBuffer1);    /*��������1*/
    if(sure==0x00)
    {  
			/*��������ָ�ƿ�*/
      //sure=PS_HighSpeedSearch(CharBuffer1,0,PS_MAXNUM,&ID);
			sure=PS_Search(CharBuffer1,0,PS_MAXNUM,&ID);
			
      if(sure==0x00)
      {
        //AS608_INFO ("�Ա�ָ�Ƴɹ���ָ��ID��%d������\r\n\r\n",ID);
				PS_AuraLedConfig(LED_on, 0x01, green, 0);/*��ɫ��������֤�ɹ�*/
				
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
  * @brief  ���ָ�ƿ�
  * @param  ��
  * @retval ��
  */
void Clean_FR(void) 
{
  uint16_t  sure;
  
	//AS608_INFO("���ָ�ƿ�\r\n");
	PS_AuraLedConfig(LED_on, 0x01, yellow, 3);
  
	sure=PS_Empty();     
	if(sure == 0x00)
	{
		//AS608_INFO("���ָ�ƿ�ɹ�������\r\n\r\n");
		PS_AuraLedConfig(blink_LED, 0x01, pink, 3);
	}
	else 
	{
		ShowErrMessage(sure);
	}

}
