//����һ��ͨ��delphi��λ�����ƿ�����12864
//Һ����ʾ��demo

#include "example1.h"
#include "uip.h"
//#include "AT45DB321.h"

#include <string.h>
//#include "lcd_app.h"
#include "USART.h"
#include "stdio.h"

void example1_init(void) 
{
	uip_listen(HTONS(8001));
}

void example1_app(void) 
{
	u16_t i;
	idata u8_t RxdBuf[100];
//	u16_t sector;
	if(uip_connected())
	{
		SendString("example1 is connected!\r\n");
	}
//	if(uip_stopped())
	{
//		printu("example1 is stopped!\r\n");
	}
	if(uip_acked())
	{
		SendString("example1 is acked!\r\n");
	}
	if(uip_closed())
	{
		SendString("example1 is closed!\r\n");
	}
	if(uip_timedout())
	{
		printf("example1 is timedout!\r\n");
	}
	if(uip_newdata() || uip_rexmit()) 
	{
		memcpy(RxdBuf, uip_appdata, uip_len);
		printf(RxdBuf);
		//printuf("\r\n�յ�0x%x �ֽ�\r\n", uip_len);
		//printf("\r\n�յ�0x%x �ֽ�\r\n", uip_len);
		SendString("\r\n�յ��ֽ�\r\n");
		RxdBuf[uip_len] = '\0';
		//ClearScreen(0);
		//putstring(6,0, &RxdBuf[0]);	

//		printu("LCD������ʾ\r\n");			
//		ClearScreen(0);


//		LCD_print12(4, 2 * uip_appdata[1], "                                             ");
//		LCD_print12(4, 2 * uip_appdata[1], &uip_appdata[2]);
	//	memcpy(&uip_appdata[0], "LCD������ʾ\r\n", 11);
	//	uip_send(&uip_appdata[0], 11);

		
	}
}