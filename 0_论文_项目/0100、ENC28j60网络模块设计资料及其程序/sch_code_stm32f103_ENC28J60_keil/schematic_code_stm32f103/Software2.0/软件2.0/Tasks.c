
#include "RTL.h"
#include "Tasks.h"
#include "enc28j60.h"
#include "stdio.h"
#include "File_Config.h"
#include "ili9320.h"
#include "string.h"
#include "stdlib.h"
#include "STM32_Net_BSL.h"
#include "USART.h"
#include "RTX_CAN.h"                  /* CAN Generic functions & defines     */
#include "CAN_Hw.h"                   /* CAN hw specific functions & defines */

U64 tcp_stack[1024/8];

__task void tick_timer (void) 
{
   os_itv_set (200);
   while (1) {
      os_itv_wait ();
      /* Timer tick every 100 ms */
      timer_tick ();
   }
}

__task void tcp_poll (void) 
{
   while (1) {
      main_TcpNet ();
      os_tsk_pass ();
   }
}

__task void usart_thread (void) 
{
   CAN_msg msg;
   u32 msgcnt = 0;
   while (1) 
   {        
      //USART_RecvPackage(NULL,(u8 *)&msg,0xFFFF);
      USART_SendPackage(-1,(u8 *)&msg,sizeof(msg),0xFFFF); 
//      os_dly_wait (50);
      msgcnt ++; 
   }
}

__task void InitTask (void) 
{    
    init_TcpNet ();
    finit ();
    USART_Initial(19200);

    os_tsk_prio_self (100);
    os_tsk_create (usart_thread, 3);
    os_tsk_create (tick_timer, 2);
    os_tsk_create_user (tcp_poll, 0, &tcp_stack, sizeof(tcp_stack));

    os_tsk_delete_self ();
}

