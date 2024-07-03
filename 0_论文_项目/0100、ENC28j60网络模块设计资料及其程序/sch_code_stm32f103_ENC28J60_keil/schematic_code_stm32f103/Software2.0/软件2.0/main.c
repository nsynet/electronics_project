
#include "enc28j60.h"
#include "STM32_Net_BSL.h"
#include "ili9320.h"

#include "RTL.h"
#include "Tasks.h"

#include <stdio.h>


//struct FILE { int handle; /* Add whatever you need here */ };
FILE *file;

int main()
{
    RCC_Configuration();
    NVIC_Configuration();
    
    Lcd_Configuration();
    ili9320_Initializtion();

    
    //init_uip();
    
    //printf("系统正在初始化...\n系统初始化完成!\n");
    printf("System Initail Finished!\n");
    //for(i = 0 ; i < 500 ; i ++)
    //    printf(teststr);
     
    os_sys_init(InitTask);      
}

