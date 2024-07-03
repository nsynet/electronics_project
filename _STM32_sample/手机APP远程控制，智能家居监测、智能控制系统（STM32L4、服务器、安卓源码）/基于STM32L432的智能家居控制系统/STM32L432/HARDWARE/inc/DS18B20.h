#ifndef __DS18B20_H
#define __DS18B20_H 

#include "sys.h"
#include "delay.h"	  
#include "usart.h"

#define DS18B20_DQ_OUT_SET  GPIOB->ODR|=(1<<5)
#define DS18B20_DQ_OUT_RESET  GPIOB->ODR&=~(1<<5)
#define DS18B20_DQ_IN  (GPIOB->IDR&=(1<<5))

#define DS18B20_Clock RCC_AHB1Periph_GPIOB
#define DS18B20_GPIO  GPIOB
#define DS18B20_Pin		GPIO_Pin_5


//IO��������
#define DS18B20_IO_IN()  {DS18B20_GPIO->MODER&=~(3<<(5*2));DS18B20_GPIO->MODER|=0<<5*2;}	//PB5����ģʽ
#define DS18B20_IO_OUT() {DS18B20_GPIO->MODER&=~(3<<(5*2));DS18B20_GPIO->MODER|=1<<5*2;} 	//PB5���ģʽ
 
extern float DS18B20_Temperature; 

u8 DS18B20_Init(void);			//��ʼ��DS18B20
double DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    

#endif















