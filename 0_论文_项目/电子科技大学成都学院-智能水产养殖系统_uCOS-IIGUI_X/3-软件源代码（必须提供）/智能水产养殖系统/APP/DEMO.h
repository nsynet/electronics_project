#include "stm32f10x.h"
#include "..\..\APP\includes.h"


#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif


#define TP_CS()  GPIO_ResetBits(GPIOB,GPIO_Pin_7)	  
#define TP_DCS() GPIO_SetBits(GPIOB,GPIO_Pin_7)	 


EXT int AD_value[4];
//EXT volatile unsigned  short int  ADC_ConvertedValue[100],ADC_ConvertedValue1[2],ADC_TIMEOUT; 
EXT char adctempchar0[5];		//ADC1 通道11的整数转字符串变量。 
EXT char adctempchar1[5];
EXT char adctempchar2[5];
EXT char adctempchar3[5];
EXT float adctemp;
EXT	u8 txbuffer1[]; 
EXT	u8 txbuffer2[]; 
EXT	u8 txbuffer3[]; 
EXT int SXTtable; 
