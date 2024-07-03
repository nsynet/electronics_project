/**
  * @file    ov7670.c 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2015.6.23
  * @brief   OV7670 Camera Driver
  */
#include "ov7670.h"
#include "sccb_bus.h"
#include "CamConfig.h"
#include "ov7670_table.h"
#include "stm32f429i_lcd.h"
uint32_t LCD_BUF[LCD_PIXEL_WIDTH*LCD_PIXEL_HEIGHT*2] __attribute__((at(0xD0000000)));

/**
  * @brief  
  * @param  None
  * @retval None
  */
static void _delay(uint32_t dl)
{
	unsigned int i,y;
	for(i = 0; i < 1000; i++)
	{
		for(y = 0; y < dl; y++);
	}
}
/**
  * @brief  DMA set
  * @param  None
  * @retval None
  */
static void port_dma_init(void)
{
	DCMI_InitTypeDef DCMI_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//DCMI 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
	RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOI|
	RCC_AHB1Periph_GPIOH, ENABLE);//ʹ��DCMI��GPIOʱ��
	

	
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_DCMI);//DCMI_HSYNC 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_PIXCLK  
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_D5 	  
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_DCMI);//DCMI_VSYNC 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);//DCMI_D6 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D7 			  
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_DCMI);//DCMI_D0 
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_DCMI);//DCMI_D1 				
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_DCMI);//DCMI_D2 
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_DCMI);//DCMI_D3 
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_DCMI);//DCMI_D4 

	/* DCMI GPIO configuration ************************************************* */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10 | GPIO_Pin_11|
	                                  GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_Init(GPIOH, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOI, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
	DCMI_Init(&DCMI_InitStructure);

	/* DCMI Interrupts config ***************************************************/
	//DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE);	
  DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//����֡�ж� 
	
	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure); 


	DMA_DeInit(DMA2_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)LCD_BUF;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 38400;                                 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //ʧ�������ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                   //ʹ���ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;       //���ִ���
	DMA_InitStructure.DMA_Mode =DMA_Mode_Circular;                            //ѭ���ɼ�ģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                       //�����ж����ȼ�Ϊ��
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);                             //��������ж�
	/* Enable the DMA Stream IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

/**
  * @brief  дOV7670�Ĵ���
  * @param  reg:Ҫд��ļĴ���;data:д�������
  * @retval 0:success other:error
  */
uint8_t ov7670_write_reg(uint8_t reg, uint8_t data)
{
	sccb_bus_start();                                    //����SCCB ���߿�ʼ��������
	if(sccb_bus_write_byte(0x42) == 0)                   //д��ַ
	{
		sccb_bus_stop();                                   //����SCCB ����ֹͣ��������
		return 1;                                         //���󷵻�
	}
	systick_delay_us(10);
	if(sccb_bus_write_byte(reg) == 0)//ID
	{
		sccb_bus_stop();                                    //����SCCB ����ֹͣ��������
		return 2;                                          //���󷵻�
	}
	systick_delay_us(10);
	if(sccb_bus_write_byte(data)==0)                     //д���ݵ�������
	{
		sccb_bus_stop();                                   //����SCCB ����ֹͣ��������
		return 3;                                          //���󷵻�
	}
	sccb_bus_stop();                                     //����SCCB ����ֹͣ��������	
	return 0;                                            //�ɹ�����
}
/**
  * @brief  ��OV7670�Ĵ���
  * @param  reg:Ҫ��ȡ�ļĴ���;ret_reg:ָ���ȡ������
  * @retval 0:success other:error          
  */
uint8_t ov7660_read_reg(uint8_t reg, uint8_t *ret_reg)
{
	//ͨ��д�������üĴ�����ַ
	sccb_bus_start();
	if(sccb_bus_write_byte(0x42) == 0)              //д��ַ
	{
		sccb_bus_stop();                              //����SCCB ����ֹͣ��������
		return 1;                                     //���󷵻�
	}
	systick_delay_us(10);
	if(sccb_bus_write_byte(reg)==0)//ID
	{
		sccb_bus_stop();                             //����SCCB ����ֹͣ��������
		return 2;                                    //���󷵻�
	}
	sccb_bus_stop();                               //����SCCB ����ֹͣ��������	
	systick_delay_us(10);	
	                                                //���üĴ�����ַ�󣬲��Ƕ�
	sccb_bus_start();
	if(sccb_bus_write_byte(0x43)==0)                //����ַ
	{
		sccb_bus_stop();                              //����SCCB ����ֹͣ��������
		return 3;                                     //���󷵻�
	}
	systick_delay_us(10);
	*ret_reg = sccb_bus_read_byte();                //���ض�����ֵ
	sccb_bus_send_noack();                          //����NACK����
	sccb_bus_stop();                                //����SCCB ����ֹͣ��������
	return 0;                                       //�ɹ�����
}
/**
  * @brief  software reset
  * @param  None
  * @retval None
  */
void ov7670_reset(void)
{
	ov7670_write_reg(0xFF, 0x01);
	ov7670_write_reg(0x12,0x80);
	_delay(1000);
}

/**
  * @brief  ��ȡID
  * @param  None
  * @retval temp
           -Camera����ID
  */
uint16_t ov7660_get_id(void)
{
	
	uint8_t pidh=0,pidl=0;
	uint16_t temp=0;
	ov7670_write_reg(0xff, 0x01);
	ov7660_read_reg(0x0a,&pidh);
	ov7660_read_reg(0x0b,&pidl);
	temp=(uint16_t)(((uint16_t)(pidh<<8)) | pidl);

	return temp;
}

/**
  * @brief  MCO����(PA8���24Mhz��Camera)
  * @param  None
  * @retval None
  */
static void mco_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);       //Enable SYSCFG clock 

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                    // MCO configure 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);
    RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
}

/**
  * @brief  ��ȡID
  * @param  None
  * @retval ovid:����ID
  */
uint16_t ov7670_check(void)
{
	uint16_t ovid;
#ifdef USE_MCO
	mco_config();                                           //output 24MHz
#endif                               
	port_dma_init();                                        //DCMI to LCD_BUF
	sccb_bus_init();
	ov7670_reset();
	systick_delay_ms(5);
	ovid = ov7660_get_id();
	
	return ovid; 
}

/**
  * @brief  ��ʼ����
  * @param  None
  * @retval None
  */
void cam_start(void)
{	
	DMA_Cmd(DMA2_Stream1, ENABLE); 
	DCMI_Cmd(ENABLE); 
	DCMI_CaptureCmd(ENABLE); 

}

/**
  * @brief  ֹͣ����
  * @param  None
  * @retval None
  */
void cam_stop(void)
{	
	DMA_Cmd(DMA2_Stream1, DISABLE); 
	DCMI_Cmd(DISABLE); 
	DCMI_CaptureCmd(DISABLE); 

}

/**
  * @brief  ��ʼ��7670�Ĵ���
  * @param  None
  * @retval 0-success;  1-error
  */
uint8_t ov7670_init(void)
{
	uint32_t i;

	for(i = 0; i < OV7670_REG_NUM; i++)
	{
		if(ov7670_write_reg(OV7670_reg[i][0],OV7670_reg[i][1]))
		{
			return 1;
		}
	}
	return 0;	
}


