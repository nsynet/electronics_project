#ifndef _USART_INCLUDED_
#define _USART_INCLUDED_

void USART_Init(void);
void SendASC(unsigned char d);
void SendString(unsigned char *str);

#endif     