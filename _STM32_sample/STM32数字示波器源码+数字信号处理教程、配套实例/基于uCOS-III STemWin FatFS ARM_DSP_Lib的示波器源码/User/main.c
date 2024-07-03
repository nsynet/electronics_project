/*
*********************************************************************************************************
*	                                  
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V3.0
*	˵    �� : uCOS-III����
*	�޸ļ�¼ : 
*		�汾��    ����         ����        ˵��
*		V1.0    2015-01-05    Eric2013     �׷�
*	    V2.0    2015-03-27    Eric2013     1. �����̼��⵽V1.5.0
*                                          2. ����BSP�弶֧�ְ� 
*                                          3. ����fatfs��0.11
*                                          4. ����STemWin��5.26��ע������ʹ�õ��Ƿ��Ż��汾
*                                             �Ż��汾�ͷ��Ż��汾������������ӣ�
*                                             (ʵ�ʲ���ST��˾���������ֿ�STemWin526_CM4_OS_Keil_ot.lib��STemWin526_CM4_OS_Keil.lib������)
*                                             http://bbs.armfly.com/read.php?tid=7442          
*                                          5. ����Ϊ�µ��ĵ㴥��У׼�㷨�����ز���Ŵ����������
*                                          6. ���7��800*480�ֱ��ʵ�����֧�֡�
*	    V3.0    2015-12-18  Eric2013       1. ����BSP�弶֧�ְ� 
*                                          2. ����STemWin��5.28
*                                          3. ���4.3���5�����������֧�֡����Ǵ����̵�ǰ��֧��800*480�ֱ��ʡ�
*                                          4. �޸���bsp_adc.c�ļ����TIM1��ʼ������
*                                          5. �޸���bsp_dac.c�ļ����DAC��ʼ������
*                                             
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include  <includes.h>
#include  "MainTask.h"

/*
*********************************************************************************************************
*                                      �궨��
*********************************************************************************************************
*/  
#define OSTCBSize  sizeof(OS_TCB)/sizeof(CPU_STK) + 1 

/*
*********************************************************************************************************
*                                  ���ļ�ʹ�õ�ȫ�ֱ���
*********************************************************************************************************
*/                                                            
static  OS_TCB   *AppTaskStartTCB;
static  CPU_STK  *AppTaskStartStk;

static  OS_TCB   *AppTaskUpdateTCB;
static  CPU_STK  *AppTaskUpdateStk;

static  OS_TCB   *AppTaskCOMTCB;
static  CPU_STK  *AppTaskCOMStk;

static  OS_TCB   *AppTaskUserIFTCB;
static  CPU_STK  *AppTaskUserIFStk;

static  OS_TCB   *AppTaskGUITCB;
static  CPU_STK  *AppTaskGUIStk;

static  OS_TCB   *AppTaskGUIRefreshTCB;
static  CPU_STK  *AppTaskGUIRefreshStk;

OS_SEM     	SEM_SYNCH;	   /* ���ź�������ʵ�ְ������º������Ļ��ͼ       */

/*
*********************************************************************************************************
*                                        ���ļ��ڲ��ĺ���
*********************************************************************************************************
*/
static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskCreate         (void);
static  void  AppObjCreate          (void);
static void   AppTaskUserIF         (void     *p_arg);
static void   AppTaskGUI            (void     *p_arg);
static void   AppTaskGUIRefresh     (void     *p_arg);
static void   AppTaskCOM			(void 	  *p_arg);

/*
*********************************************************************************************************
*	�� �� ��: Stack_Init
*	����˵��: �����ϵͳ��ջȫ��ʹ��CCM��������ƿ�Ҳʹ��CCM�ռ䡣
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Stack_Init(void)
{
	CPU_STK *STK_Temp;
	OS_TCB *TCB_Temp;

/***********�û�������ƿ飬��6�������TCB*********************/	
	TCB_Temp = (OS_TCB *)0x10000000;
	AppTaskStartTCB = TCB_Temp;
	TCB_Temp ++; 

	AppTaskUpdateTCB = TCB_Temp;
	TCB_Temp ++; 

	AppTaskCOMTCB = TCB_Temp;
	TCB_Temp ++;
	 
	AppTaskUserIFTCB = TCB_Temp;
	TCB_Temp ++; 

	AppTaskGUITCB = TCB_Temp;
	TCB_Temp ++; 

	AppTaskGUIRefreshTCB = TCB_Temp;
	TCB_Temp ++; 

/***********ϵͳ������ƿ�**os.h 1133���޸ı���*******************/
//#if (OS_CFG_TMR_EN > 0u)	
//	OSTmrTaskTCB = TCB_Temp;/***os_tmr***/
//	TCB_Temp ++;
//#endif
//	  
//	OSTickTaskTCB = TCB_Temp;/***os_stick***/
//	TCB_Temp ++; 
//
//#if OS_CFG_STAT_TASK_EN > 0u
//	OSStatTaskTCB = TCB_Temp;/***os_stat***/
//	TCB_Temp ++; 
//#endif
//
//#if OS_CFG_ISR_POST_DEFERRED_EN > 0u
//	OSIntQTaskTCB = TCB_Temp;/***os_int***/
//	TCB_Temp ++; 
//#endif
//
//	OSIdleTaskTCB = TCB_Temp;/***os_core 817��*/ 
//	TCB_Temp ++; 

/***********�û������ջ�ռ�*************************************/
	STK_Temp = (CPU_STK *)TCB_Temp; 
	AppTaskStartStk = STK_Temp;

	STK_Temp = STK_Temp + APP_CFG_TASK_START_STK_SIZE; 	
	AppTaskUpdateStk = STK_Temp;

	STK_Temp = STK_Temp + APP_CFG_TASK_UPDATE_STK_SIZE; 	
	AppTaskCOMStk = STK_Temp;

	STK_Temp = STK_Temp + APP_CFG_TASK_COM_STK_SIZE; 	
	AppTaskUserIFStk = STK_Temp;

	STK_Temp = STK_Temp + APP_CFG_TASK_USER_IF_STK_SIZE; 	
	AppTaskGUIStk = STK_Temp;

	STK_Temp = STK_Temp + APP_CFG_TASK_GUI_STK_SIZE;
	AppTaskGUIRefreshStk = STK_Temp;

/**********�����6����UCOS-III���ں˷������*****************/
	STK_Temp = STK_Temp + APP_CFG_TASK_GUIRefresh_STK_SIZE;
	OSCfg_IdleTaskStk = STK_Temp;
	OSCfg_IdleTaskStkBasePtr = OSCfg_IdleTaskStk;
	STK_Temp = STK_Temp + OS_CFG_IDLE_TASK_STK_SIZE; 

/***************�ж϶�������*********************************/
#if (OS_CFG_ISR_POST_DEFERRED_EN > 0u)
	
	OSCfg_IntQTaskStk = STK_Temp;
	OSCfg_IntQTaskStkBasePtr = OSCfg_IntQTaskStk;
	STK_Temp = STK_Temp + OS_CFG_INT_Q_TASK_STK_SIZE;
#else
	OSCfg_IntQTaskStkBasePtr = (CPU_STK *)0; 
#endif 

/****************ISR��ջ*************************************/
#if (OS_CFG_ISR_STK_SIZE > 0u)
	OSCfg_ISRStk = STK_Temp;
	OSCfg_ISRStkBasePtr = OSCfg_ISRStk;
	STK_Temp = STK_Temp + OS_CFG_ISR_STK_SIZE;
#else
	OSCfg_ISRStkBasePtr = (CPU_STK *)0; 
#endif

/**************ͳ������*************************************/
#if (OS_CFG_STAT_TASK_EN > 0u)
	OSCfg_StatTaskStk = STK_Temp;
	OSCfg_StatTaskStkBasePtr = OSCfg_StatTaskStk;
	STK_Temp = STK_Temp + OS_CFG_STAT_TASK_STK_SIZE;
#else
	OSCfg_StatTaskStkBasePtr = (CPU_STK *)0; 
#endif

/****************�δ�����**********************************/	
	OSCfg_TickTaskStk = STK_Temp;
	OSCfg_TickTaskStkBasePtr = OSCfg_TickTaskStk;
	STK_Temp = STK_Temp + OS_CFG_TICK_TASK_STK_SIZE;

/*****************��ʱ������*******************************/
#if (OS_CFG_TMR_EN > 0u)
	OSCfg_TmrTaskStk = STK_Temp;
    OSCfg_TmrTaskStkBasePtr = OSCfg_TmrTaskStk;
	STK_Temp = STK_Temp + OS_CFG_TMR_TASK_STK_SIZE;
#else
    OSCfg_TmrTaskStkBasePtr = (CPU_STK *)0; 
#endif
}
					       
/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
    OS_ERR  err;
		
	Stack_Init();   /* ��ջ��ʼ�� */
    OSInit(&err);   /* ��ʼ��uC/OS-III */

	OSTaskCreate((OS_TCB       *)AppTaskStartTCB,         /* ������������ */
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )AppTaskStart, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
                 (CPU_STK      *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

    OSStart(&err);         /* ��ʼ�������ִ�� */
    
    (void)&err;
    
    return (0);
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(��BSP_Init��ʵ��)
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 2
*********************************************************************************************************
*/
static  void  AppTaskStart (void *p_arg)
{
	OS_ERR   err;
	uint8_t  ucCount = 0;
	uint8_t  ucCountGT811 = 0;
	uint8_t  ucCountFT = 0;
	
	(void)p_arg;
	CPU_Init();
 	bsp_Init();
	BSP_Tick_Init();                        

#if OS_CFG_STAT_TASK_EN > 0u
     OSStatTaskCPUUsageInit(&err);   
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    AppObjCreate();                                            
    AppTaskCreate(); 
	
    while (1)
	{  
		/* 1msһ�δ���ɨ�裬���败���� */
		if(g_tTP.Enable == 1)
		{
			TOUCH_Scan();	
		}
		/* 10msһ�δ���ɨ�裬���ݴ�����GT811 */
		else if(g_GT811.Enable == 1)
		{
			ucCountGT811++;
			if(ucCountGT811 == 10)
			{
				ucCountGT811 = 0;
				GT811_OnePiontScan();
			}
		}
		/* 10msһ�δ���ɨ�裬���ݴ�����FT5X06 */
		else if(g_tFT5X06.Enable == 1)
		{
			ucCountFT++;
			if(ucCountFT == 10)
			{
				ucCountFT = 0;
				FT5X06_OnePiontScan();
			}
		}
		
		/* 10msһ�ΰ������ */
		ucCount++;
		if(ucCount == 10)
		{
			ucCount = 0;
			bsp_KeyScan();
		}
		BSP_OS_TimeDlyMs(1);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUIUpdate
*	����˵��: ��������Ҫʵ�ֽ�ͼ����.	
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 3
*********************************************************************************************************
*/
static void AppTaskGUIUpdate(void *p_arg)
{
	OS_ERR      err;
	uint8_t		Pic_Name = 0;
	char buf[20];
	CPU_BOOLEAN SemFlag;

	(void)p_arg;
		  
	while(1)
	{	
		SemFlag = BSP_OS_SemWait(&SEM_SYNCH, 0);
		
		if(SemFlag == DEF_OK)
		{	
			sprintf(buf,"0:/PicSave/%d.bmp",Pic_Name);
			OSSchedLock(&err);
			
			/* ���SD����û��PicSave�ļ�������д��� */
			result = f_mkdir("0:/PicSave");
			/* ������ͼ */
			result = f_open(&file,buf, FA_WRITE|FA_CREATE_ALWAYS);
			/* ��SD������BMPͼƬ */
			GUI_BMP_Serialize(_WriteByte2File, &file);
			
			/* ������ɺ�ر�file */
		    result = f_close(&file);
			
			OSSchedUnlock(&err);
			Pic_Name++; 		
		}										  	 	       											  
	}   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCom
*	����˵��: �����Ժ�ʹ��		
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ����3
*********************************************************************************************************
*/
static void AppTaskCOM(void *p_arg)
{	
   (void)p_arg;
	 
	while(1)
	{	
		/* 100sִ��һ�� */
		BSP_OS_TimeDlyMs(1000000);	
	}	 						  	 	       											    
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskUserIF
*	����˵��: �����Ժ�ʹ�á�
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 2
*********************************************************************************************************
*/
static void AppTaskUserIF(void *p_arg)
{
	(void)p_arg;	/* ������������� */

	while (1) 
	{   
		/* 100sִ��һ�� */
        BSP_OS_TimeDlyMs(1000000);	     
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUI
*	����˵��: GUI����			  			  
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ����OS_CFG_PRIO_MAX - 4u
*********************************************************************************************************
*/
static void AppTaskGUI(void *p_arg)
{
    (void)p_arg;		/* ����������澯 */
		
	while (1) 
	{
		MainTask();					 
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUIRefresh
*	����˵��: Led��˸���񣬱�ʾϵͳ����	  			  
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: OS_CFG_PRIO_MAX - 5u
*********************************************************************************************************
*/
static void AppTaskGUIRefresh(void *p_arg)
{
	(void)p_arg;		/* ����������澯 */
	  	
	while (1) 
	{
        BSP_OS_TimeDlyMs(200); 
		bsp_LedToggle(2);
    }						 
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{
	OS_ERR      err;
	
	/**************����UPDATE����*********************/
	OSTaskCreate((OS_TCB       *)AppTaskUpdateTCB,             
                 (CPU_CHAR     *)"App Task Update",
                 (OS_TASK_PTR   )AppTaskGUIUpdate, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_UPDATE_PRIO,
                 (CPU_STK      *)&AppTaskUpdateStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_UPDATE_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_UPDATE_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);

	/**************����COM����*********************/
	OSTaskCreate((OS_TCB       *)AppTaskCOMTCB,            
                 (CPU_CHAR     *)"App Task COM",
                 (OS_TASK_PTR   )AppTaskCOM, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_COM_PRIO,
                 (CPU_STK      *)&AppTaskCOMStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_COM_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	
	/**************����USER IF����*********************/
	OSTaskCreate((OS_TCB       *)AppTaskUserIFTCB,             
                 (CPU_CHAR     *)"App Task UserIF",
                 (OS_TASK_PTR   )AppTaskUserIF, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_USER_IF_PRIO,
                 (CPU_STK      *)&AppTaskUserIFStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_USER_IF_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	
	/**************����GUI����*********************/			 
	OSTaskCreate((OS_TCB       *)AppTaskGUITCB,              
                 (CPU_CHAR     *)"App Task GUI",
                 (OS_TASK_PTR   )AppTaskGUI, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
                 (CPU_STK      *)&AppTaskGUIStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY    )1,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
	
	/**************����GUIˢ������*********************/			 
	OSTaskCreate((OS_TCB       *)AppTaskGUIRefreshTCB,              
                 (CPU_CHAR     *)"App Task GUIRefresh",
                 (OS_TASK_PTR   )AppTaskGUIRefresh, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_GUIRefresh_PRIO,
                 (CPU_STK      *)&AppTaskGUIRefreshStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUIRefresh_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_GUIRefresh_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);			 
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: �����ź���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
	/* ����ͬ���ź��� */ 
   	BSP_OS_SemCreate(&SEM_SYNCH,
					 0,	
					 (CPU_CHAR *)"SEM_SYNCH");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
