#include "include.h"
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )              // PB5: LED D2

#define BP2             0x2000                     // PC13: BP2
#define BP3             0x0001                     // PA0 : BP3

#define UP              0x0800                     // PB11: UP
#define RIGHT           0x1000                     // PB12: RIGHT
#define LEFT            0x2000                     // PB13: LEFT
#define DOWN            0x4000                     // PB14: DOWN
#define OK              0x8000                     // PB15: OK

#define JOYSTICK        0xF800                     // JOYSTICK ALL KEYS
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/
/*----------------------------------------------------------*\
 |  Delay                                                   |
 |  延时 Inserts a delay time.                              |
 |  nCount: 延时时间                                        |
 |  nCount: specifies the delay time length.                |
\*----------------------------------------------------------*/
void Delay(vu32 nCount)
{
   for (; nCount != 0; nCount--)
      ;
}
/*----------------------------------------------------------*\
 | SendChar                                                 |
 | Write character to Serial Port.                          |
\*----------------------------------------------------------*/
int SendChar(int ch)
{
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (ch & 0x1FF);

   return (ch);
}
/*----------------------------------------------------------*\
 | GetKey                                                   |
 | Read character to Serial Port.                           |
\*----------------------------------------------------------*/
int GetKey(void)
{
   while (!(USART1->SR & USART_FLAG_RXNE));

   return ((int) (USART1->DR & 0x1FF));
}


/*----------------------------------------------------------
	1, 默认IP地址： 192. 168. 1.100	 --修改位置：simple_server.c,的数组：myip初始值
	2, 默认SPI1端口,--定义位置：spi.c
	3, EN28J60 定义设置位置：enc28j60.c

 //| MIAN ENTRY                                               |
*----------------------------------------------------------*/
int main(void)
{
   
   stm32_Init();
  // PC8 = 1; PC9 = 1; //PC10 = 1; PC11 = 1;
   PE5= PE6 = 1;	    
   
   SPInet_Init();


   simple_server();


   return 0;


}
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
