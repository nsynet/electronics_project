#include "ds18b20.h"
#include "misc.h"

void DS18B20_PortConfig(uint8_t dir)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(dir == 1)
	{
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}
	else
	{
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}
	
	
	
}

void DS18B20_Rst(void)
{
	DS18B20_PortConfig(1);
	DS18B20_PIN_H;
	DS18B20_PIN_L;
	
	delay(600);
	
	DS18B20_PIN_H;
	
	delay(15);
	
	
}


uint8_t DS18B20_Check(void)
{
	uint16_t retry = 0;
	
	DS18B20_Rst();
	
	DS18B20_PortConfig(0);
	
	while(DS18B20_IN && retry < 500)
		retry ++;
	
	if(retry >= 500)
		return 1;
	
	retry = 0;
	
	while(!DS18B20_IN && retry < 500)
		retry ++;
	
	if(retry >= 500)
		return 1;
	
	return 0;
}

uint8_t DS18B20_ReadBit(void)
{
	uint8_t data;
	
	DS18B20_PortConfig(1);
	
	DS18B20_PIN_L;
	delay(2);
	DS18B20_PIN_H;
	
	DS18B20_PortConfig(0);
	delay(12);
	
	if(DS18B20_IN)
		data = 1;
	else
		data = 0;
	
	delay(50);
	
	return data;
	
}

uint8_t DS18B20_ReadByte(void)
{
	uint8_t i, data = 0;
	
	DS18B20_PortConfig(1);
	
	for(i = 1;i <= 8;i++) 
	{
			DS18B20_PIN_L;
			data >>= 1;
			delay(2);
			DS18B20_PIN_H;
			delay(2);
			DS18B20_PortConfig(0);
			if(DS18B20_IN)
				data|=0x80;
			delay(50);
			DS18B20_PortConfig(1);
			DS18B20_PIN_H; 
	
	}						    
	
	return data;
}

void DS18B20_WriteByte(uint8_t Data)     
{             
uint8_t j;
uint8_t testb;
DS18B20_PortConfig(1);//SET PA0 OUTPUT;

for (j=1;j<=8;j++) 
	{
	testb=Data&0x01;
	Data=Data>>1;
	if (testb) 
		{
		DS18B20_PIN_L;
		delay(5);                            
		DS18B20_PIN_H;
		delay(60);             
		}
	else 
		{
		DS18B20_PIN_L;
		delay(5);             
		DS18B20_PIN_L;
		delay(60);                          
		}
	 DS18B20_PIN_H;
			}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_WriteByte(0xCC);
	DS18B20_WriteByte(0x44);
	
	
}

float DS18B20_GetTemp(void)
	{
	uint8_t temp;
	uint8_t TL,TH;
	int16_t tem;
	DS18B20_Start();                    // ds1820 start convert
	DS18B20_Rst();
	DS18B20_Check();	 
	DS18B20_WriteByte(0xcc);// skip rom
	DS18B20_WriteByte(0xbe);// convert	    
	TL=DS18B20_ReadByte(); // LSB   
	TH=DS18B20_ReadByte(); // MSB  
	
	if(TH>7)
		{
		TH=~TH;
		TL=~TL; 
		temp=0;//温度为负  
		}
	else temp=1;//温度为正	  	  
	tem=TH; //获得高八位
	tem<<=8;    
	tem+=TL;//获得底八位
//	tem=tem*0.0625;//转换 (float) 
//    tem=(TH<<8+TL)*0.0625;
	if(temp)return ((float)tem*0.0625); //返回温度值
	else return -((float)tem*0.0625);    

	}





