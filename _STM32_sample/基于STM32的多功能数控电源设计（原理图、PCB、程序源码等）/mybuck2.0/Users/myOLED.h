/************************************************************************************
*  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: OLED.h
* Project  : HelTec.uvprij
* Processor: STM32F103C8T6
* Compiler : MDK fo ARM
* 
* Author : С��
* Version: 1.00
* Date   : 2014.2.20
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64�����OLED��ʾ�������ļ����������ڻ����Զ���(heltec.taobao.com)��SD1306����SPIͨ�ŷ�ʽ��ʾ��
*
* Others: none;
*
* Function List:
*
* 2. void OLED_WrDat(unsigned char dat) -- ��OLEDд����
* 3. void OLED_WrCmd(unsigned char cmd) -- ��OLEDд����
* 4. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
* 5. void OLED_Fill(unsigned char bmp_dat) -- ȫ�����(0x00��������������0xff��������ȫ������)
* 6. void OLED_CLS(void) -- ����
* 7. void OLED_Init(void) -- OLED��ʾ����ʼ��
* 8. void OLED_6x8Str(unsigned char x, y,unsigned char ch[]) -- ��ʾ6x8��ASCII�ַ�
* 9. void OLED_8x16Str(unsigned char x, y,unsigned char ch[]) -- ��ʾ8x16��ASCII�ַ�
* 10.void OLED_16x16CN(unsigned char x, y, N) -- ��ʾ16x16���ĺ���,����Ҫ����ȡģ�����ȡģ
* 11.void OLED_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- ȫ����ʾ128*64��BMPͼƬ
*
* History: none;
*
*************************************************************************************/

#ifndef __MYOLED_H
#define __MYOLED_H
//////////////////////////////////////////////////////////////////////////////////	 
//imodule	
//////////////////////////////////////////////////////////////////////////////////	

#define XLevelL			0x00
#define XLevelH			0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row			64
#define	Brightness	0xCF 
#define X_WIDTH 		128
#define Y_WIDTH 		64

//-----------------OLED�˿ڶ���----------------
#define OLED_SCL_Clr() 			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET))	//SCLK D0
#define OLED_SCL_Set() 			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET))

#define OLED_SDA_Clr() 			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET))	//SDIN D1
#define OLED_SDA_Set() 			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET))

#define OLED_RST_Clr()				(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET))	//RST(RES)
#define OLED_RST_Set()				(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET))

#define OLED_DC_Clr()				(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET))	//DC
#define OLED_DC_Set()				(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET))

#define OLED_CS_Clr()				(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET))	//CS
#define OLED_CS_Set()				(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET))

//#define OLED_CMD  0	//д����
//#define OLED_DATA 1	//д����
#include "mybsp.h"


//OLED�����ú���
void OLED_WrDat(unsigned char dat);//д����
void OLED_WrCmd(unsigned char cmd);//д����
void OLED_SetPos(unsigned char x, unsigned char y);//������ʼ������
void OLED_Fill(unsigned char bmp_dat);//ȫ�����
void OLED_CLS(void);//����
void OLED_Init(void);//��ʼ��
void OLED_6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_8x16Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_16x16CN(unsigned char x, unsigned char y, unsigned char N);
void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);

#endif
