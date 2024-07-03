#include "keyboard.h"

static u8 LastKey=0x00;

void KBD_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //---------------------------------------------
    //KBD GPIO ÅäÖÃ
    //K1,K2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    //K3 K4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u8 KBD_GetKey(void)
{
    u8 rKey=0x00;

    if ((GPIOA->IDR & GPIO_Pin_0) == 0) rKey|= KEY_MENU;
    if ((GPIOA->IDR & GPIO_Pin_1) == 0) rKey|= KEY_UP;
    if ((GPIOB->IDR & GPIO_Pin_10) == 0) rKey|= KEY_DOWN;
    if ((GPIOB->IDR & GPIO_Pin_11) == 0) rKey|= KEY_ENTER;

    if (rKey==LastKey) rKey=0x00;
    else (LastKey=rKey) ;   

    return rKey;
}
