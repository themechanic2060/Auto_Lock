#include "key.h"

uint8_t key_scan(void)
{
	if(KEY1==0||KEY2==0||KEY3==0)//�жϰ���
	{
		if(KEY1==0)
		{
			return 1;
		}
		
		else if(KEY2==0)
		{
			return 2;
		}
		
		else if(KEY3==0)
		{
			return 3;
		}
	}
	
	else if(KEY1==1&&KEY2==1&&KEY3==1)//û�а�������
	{
		return 0;
	}
}
