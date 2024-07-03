#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
} 
//按键处理函数
//返回按键值
u8 KEY_Scan(void)
{	 
	u8 key=0;
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==0)
	{
		delay_ms(10);//去抖动 
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==0);
			key=1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==0)
	{
		delay_ms(10);//去抖动 
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)==0);
			key=2;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)==0)
	{
		delay_ms(10);//去抖动 
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)==0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)==0);
			key=3;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)==0)
	{
		delay_ms(10);//去抖动 
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)==0)
		{
			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)==0);
			key=4;
		}
	}
	return key;	
}
