
#define GLOBALS	  					   
#include "includes.h"
#include "demo.h"
#include "adc.h"
#define LED8_OFF GPIO_SetBits(GPIOA, GPIO_Pin_7);  
#define LED8_ON GPIO_ResetBits(GPIOA, GPIO_Pin_7); 
extern void Fun(void);		
extern void Delay(__IO uint32_t nCount);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(int value, char *string, int radix);

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE];
static  OS_STK AppTaskKbdStk[APP_TASK_KBD_STK_SIZE];
static  OS_STK Task_AdcStk[Task_Adc_STK_SIZE];
static  OS_STK Task_SXTStk[Task_SXT_STK_SIZE];
static  OS_STK Task_DJStk[Task_DJ_STK_SIZE];

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void App_TaskCreate(void);	  
static  void App_TaskStart(void* p_arg);
static  void AppTaskUserIF (void *p_arg);
static  void Task_ADC(void *p_arg);	   
static  void AppTaskKbd(void* p_arg);
static  void Task_SXT(void *p_arg);
static  void Task_DJ(void *p_arg);
  void uartPut(u8  *s)
{	
     USART_ClearFlag(USART1,USART_FLAG_TC);
		   while((*s != '\0'))
       {
              USART_SendData(USART1,*(s++));
              //等待数据发送完毕
              while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
       }
}

/****************************************************************************
* 名    称：int main(void)
* 功    能：主函数入口
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
int main(void)
{
   CPU_INT08U os_err; 
   /* 禁止所有中断 */

   CPU_IntDis();
   
   /* ucosII 初始化 */
   OSInit();                                                  

   /* 硬件平台初始化 */
   BSP_Init();                               
   	 
   //建立主任务， 优先级最高  建立这个任务另外一个用途是为了以后使用统计任务
   os_err = OSTaskCreate((void (*) (void *)) App_TaskStart,               		    //指向任务代码的指针
                          (void *) 0,												//任务开始执行时，传递给任务的参数的指针
               		     (OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],	//分配给任务的堆栈的栈顶指针   从顶向下递减
                         (INT8U) APP_TASK_START_PRIO);								//分配给任务的优先级
             
   OSTimeSet(0);			 //ucosII的节拍计数器清0    节拍计数器是0-4294967295  
   OSStart();                //启动ucosII内核   
   return (0);
}




/****************************************************************************
* 名    称：static  void App_TaskStart(void* p_arg)
* 功    能：开始任务建立
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void App_TaskStart(void* p_arg)
{
 
  (void) p_arg;
   //初始化ucosII时钟节拍
   OS_CPU_SysTickInit();
                               
   //使能ucos 的统计任务
#if (OS_TASK_STAT_EN > 0)
   
   OSStatInit();                			//----统计任务初始化函数                                 
#endif

   App_TaskCreate();						//建立其他的任务

   while (1)
   {  
   	  
	  OSTimeDlyHMSM(0, 0, 0, 1000);
   }
}

/****************************************************************************
* 名    称：static  void App_TaskCreate(void)
* 功    能：建立其余任务的函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void App_TaskCreate(void)
{ 
   	
   /*  建立用户界面任务 */
   OSTaskCreateExt(AppTaskUserIF,											   //指向任务代码的指针
   					(void *)0,												   //任务开始执行时，传递给任务的参数的指针
   					(OS_STK *)&AppTaskUserIFStk[APP_TASK_USER_IF_STK_SIZE-1],  //分配给任务的堆栈的栈顶指针   从顶向下递减
					APP_TASK_USER_IF_PRIO,									   //分配给任务的优先级
					APP_TASK_USER_IF_PRIO,									   //预备给以后版本的特殊标识符，在现行版本同任务优先级
					(OS_STK *)&AppTaskUserIFStk[0],							   //指向任务堆栈栈底的指针，用于堆栈的检验
                    APP_TASK_USER_IF_STK_SIZE,								   //指定堆栈的容量，用于堆栈的检验
                    (void *)0,												   //指向用户附加的数据域的指针，用来扩展任务的任务控制块
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);				   //选项，指定是否允许堆栈检验，是否将堆栈清0,任务是否要
					                                                           //进行浮点运算等等。
                    
   /*   建立触摸驱动任务 */
   OSTaskCreateExt(AppTaskKbd,
   					(void *)0,
					(OS_STK *)&AppTaskKbdStk[APP_TASK_KBD_STK_SIZE-1],
					APP_TASK_KBD_PRIO,
					APP_TASK_KBD_PRIO,
					(OS_STK *)&AppTaskKbdStk[0],
                    APP_TASK_KBD_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);

	   //ADC采样任务------------------------------------------------------
   OSTaskCreateExt(Task_ADC,
   					(void *)0,
					(OS_STK *)&Task_AdcStk[Task_Adc_STK_SIZE-1],
					Task_Adc_PRIO,
					Task_Adc_PRIO,
					(OS_STK *)&Task_AdcStk[0],
                    Task_Adc_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);				   
   OSTaskCreateExt(Task_SXT,
   					(void *)0,
					(OS_STK *)&Task_SXTStk[Task_SXT_STK_SIZE-1],
					Task_SXT_PRIO,
					Task_SXT_PRIO,
					(OS_STK *)&Task_SXTStk[0],
                    Task_SXT_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
   OSTaskCreateExt(Task_DJ,
   					(void *)0,
					(OS_STK *)&Task_DJStk[Task_DJ_STK_SIZE-1],
					Task_DJ_PRIO,
					Task_DJ_PRIO,
					(OS_STK *)&Task_DJStk[0],
                    Task_DJ_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);					
}


static  void Task_ADC(void *p_arg){    
  // INT8U err;
  int i;
   (void)p_arg;	  
   while(1)
   {
   OSTimeDlyHMSM(0,0,1,20); 
   
    for(i=0; i<4; i++)
   {	
	adc_read();
	AD_value[i]=(int)AD_value[i];
	}
   itoa(AD_value[0], adctempchar0,10);
   itoa(AD_value[1], adctempchar1,10);
   itoa(AD_value[2], adctempchar2,10);
   itoa(AD_value[3], adctempchar3,10);

   } 
}
/****************************************************************************
* 名    称：static  void  AppTaskUserIF (void *p_arg)
* 功    能：用户界面任务
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void  AppTaskUserIF (void *p_arg)
{											   
 (void)p_arg;								    
  GUI_Init();	                //ucgui初始化 
  while(1) 
  {				 
     Fun();						//界面程序
  }
}
/****************************************************************************
* 名    称：static  void  AppTaskKbd (void *p_arg)
* 功    能：触摸屏坐标获取任务
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static  void  AppTaskKbd (void *p_arg)
{
  (void)p_arg;   
   while(1) 
   { 
   	  /* 延时10ms会读取一次触摸坐标	*/
      OSTimeDlyHMSM(0,0,0,10); 	               
	  GUI_TOUCH_Exec();    
   }
}

/****************************************************************************
* 名    称：static  void  AppTaskSXT(void *p_arg)
* 功    能：摄像头开关
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/



static  void  Task_SXT (void *p_arg)
{
  (void)p_arg;   
   while(1) 
   { 
   	  /* 延时10ms会读取一次触摸坐标	*/
     OSTimeDlyHMSM(0,0,1,30); 	               
	 switch(SXTtable)
	 {
	 case 0: break;
	 case 1:GPIO_SetBits(GPIOA,GPIO_Pin_2);		  //开摄像头
	        SXTtable=2;
			break;
	 case 2:GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			SXTtable=0;
	        break;
	 case 3:GPIO_SetBits(GPIOA,GPIO_Pin_3);		 //关摄像头
	        SXTtable=4;
			break; 
	 case 4:GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	        SXTtable=0;
			break;   
     }
   }
}
  /****************************************************************************
* 名    称：static  void  AppTaskDJ(void *p_arg)
* 功    能：步进电机
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/



static  void  Task_DJ (void *p_arg)
{
    int DJtable=0,DJcircle=0; 
   int t=0;
  (void)p_arg;  
 
   while(1) 
   { 
   	  /* 延时10ms会读取一次触摸坐标	*/
     OSTimeDlyHMSM(0,0,0,40); 	               
	 switch(DJtable)
	 {
		case 1: GPIO_SetBits(GPIOA,GPIO_Pin_9);
		        break;
		case 2:	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
		        break;
		case 3:	GPIO_SetBits(GPIOA,GPIO_Pin_10);
		        break;
		case 4:	GPIO_ResetBits(GPIOA,GPIO_Pin_10);
		        break;
		case 5:	GPIO_SetBits(GPIOA,GPIO_Pin_11);
		        break;
		case 6:	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		        break;
		case 7:	GPIO_SetBits(GPIOA,GPIO_Pin_12);
		        break;
		case 8:	GPIO_ResetBits(GPIOA,GPIO_Pin_12); 
		        break;
		case 9: DJcircle++;
		        DJtable=t;
		        break;
     }
    if(DJcircle<15)
	   DJtable++;
	else if(DJcircle==15)
	      t=9;
	else if(DJcircle>15)
	      DJtable--;
	else if(DJcircle>30)
	     DJcircle=0;
		 t=0;  
   }
}
/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument : ptcb   is a pointer to the task control block of the task being created.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskCreateHook(OS_TCB* ptcb)
{
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskDelHook(OS_TCB* ptcb)
{
   (void) ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument : none.
*
* Note     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void App_TaskIdleHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument : none.
*********************************************************************************************************
*/

void App_TaskStatHook(void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument : none.
*
* Note     : 1 Interrupts are disabled during this call.
*
*            2  It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument : ptcb    is a pointer to the TCB of the task being created.
*
* Note     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void App_TCBInitHook(OS_TCB* ptcb)
{
   (void) ptcb;
}
#endif

#endif
 /******************* (C) COPYRIGHT 2013  STM32 *****END OF FILE****/
