/**
  * @file    sccb_bus.c 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2015.11.09
  * @brief   sccb bus
  */
#include "sccb_bus.h"

 uint32_t ntime;

/**
  * @brief  ��ʱnms*1 ms
  * @param  nms:��ʱ����
  * @retval None
  */
void systick_delay_ms(u16 nms)
{	 		  	  
	ntime = nms;
	SysTick_Config(180000);//1ms����һ���ж�
	while(ntime);
	SysTick->CTRL = 0x00;			  	    
}   

/**
  * @brief  ��ʱnus*1 us
  * @param  nus:��ʱ����
  * @retval None
  */
void systick_delay_us(u32 nus)
{		
	ntime = nus;
	SysTick_Config(180);  //1us����һ���ж�
	while(ntime);
	SysTick->CTRL = 0x00;
}

/**
  * @brief  ��ʼ������
  * @param  None
  * @retval None
  */
void sccb_bus_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  //SCCB_SIC:SCCB_SID
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**
  * @brief  ����SCCB_SIDΪ���
  * @param  None
  * @retval None
  */
void sccb_bus_data_out(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;               
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void sccb_bus_data_in(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;               
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void sccb_bus_start(void)
{
    SCCB_SID_H();             //�����߸ߵ�ƽ
    systick_delay_us(50);
    SCCB_SIC_H();	           //��ʱ���߸ߵ�ʱ���������ɸ�����
    systick_delay_us(50);
    SCCB_SID_L();
    systick_delay_us(50);
    SCCB_SIC_L();	           //�����߻ָ��͵�ƽ��������������Ҫ
    systick_delay_us(50);
}


void sccb_bus_stop(void)
{
    SCCB_SID_L();
    systick_delay_us(50);
    SCCB_SIC_H();	
    systick_delay_us(50);  
    SCCB_SID_H();	
    systick_delay_us(50);  
}


void sccb_bus_send_noack(void)
{	
	SCCB_SID_H();	
	systick_delay_us(50);	
	SCCB_SIC_H();	
	systick_delay_us(50);	
	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_SID_L();	
	systick_delay_us(50);
}

void sccb_bus_send_ack(void)
{	
	SCCB_SID_L();	
	systick_delay_us(50);	
	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_SIC_H();	
	systick_delay_us(50);	
	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_SID_L();	
	systick_delay_us(50);
}

uint8_t sccb_bus_write_byte(uint8_t data)
{
	uint32_t i;
	uint8_t tem;

	for(i = 0; i < 8; i++) //ѭ��8�η�������
	{
		if((data<<i) & 0x80)
		{
			SCCB_SID_H();
		}
		else 
		{
			SCCB_SID_L();
		}
		systick_delay_us(50);
		SCCB_SIC_H();	
		systick_delay_us(50);
		SCCB_SIC_L();	
		systick_delay_us(50);
	}
	systick_delay_us(10);
	SCCB_DATA_IN;
	systick_delay_us(50);
	SCCB_SIC_H();	
	systick_delay_us(100);
	if(SCCB_SID_STATE)
	{
		tem = 0;                //SDA=1����ʧ��
	}
	else 
	{
		tem = 1;                //SDA=0���ͳɹ�������1
	}

	SCCB_SIC_L();	
	systick_delay_us(50);	
	SCCB_DATA_OUT;
	return tem;  
}

uint8_t sccb_bus_read_byte(void)
{	
	uint32_t i;
	uint8_t read = 0;
	
	SCCB_DATA_IN;
	systick_delay_us(50);
	for(i = 8; i > 0; i--) //ѭ��8�ν�������
	{		     
		systick_delay_us(50);
		SCCB_SIC_H();
		systick_delay_us(50);
		read = read << 1;
		if(SCCB_SID_STATE)
		{
			read += 1; 
		}
		SCCB_SIC_L();
		systick_delay_us(50);
	}	
    SCCB_DATA_OUT;
	return read;
}

