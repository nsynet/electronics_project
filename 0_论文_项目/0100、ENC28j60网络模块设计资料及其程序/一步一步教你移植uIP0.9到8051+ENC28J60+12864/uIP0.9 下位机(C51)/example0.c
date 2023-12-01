#include "example0.h"
#include "uip.h"
//#include "AT45DB321.h"
#include "mcu_uart.h"
#include <string.h>

void example0_init(void) 
{
	uip_listen(HTONS(8000));
}

void example0_app(void) 
{
	u16_t i;
//	u16_t sector;
	if(uip_connected())
	{
		printu("example0 is connected!\r\n");
	}
	if(uip_newdata() || uip_rexmit()) 
	{
		printu("example0 is uip_newdata!\r\n");		
		//memcpy(RxdBuf, uip_appdata, uip_len);

		/*
		printuf("\r\n收到0x%x 字节\r\n", uip_len);
		for(i=0;i<uip_len;i++)
		{
			printuf("%x ", uip_appdata[i]);
		}
		*/
		if (uip_len == 2)
		{
			if(uip_appdata[0] == 0x02)
				if(uip_appdata[1] == 0x04)
				{
					printu("正在切换P1_4\r\n");			
					P1_4 = !P1_4;

					memcpy(&uip_appdata[0], "开发板发送:P1_4已经点亮\r\n", 14 + 12);
					uip_send(&uip_appdata[0], 14 + 12);
				}
				else if(uip_appdata[1] == 0x05)
				{
					printu("正在切换P1_5\r\n");			
					P1_5 = !P1_5;

					memcpy(&uip_appdata[0], "开发板发送:P1_5已经点亮\r\n", 14 + 12);
					uip_send(&uip_appdata[0], 14 + 12);
				}
		}
	}
}