#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "led.h"
#include "includes.h"
#include "esp8266.h"
#include "adc.h"
#include "DS18B20.h"

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define LED0_TASK_PRIO		4
//�����ջ��С	
#define LED0_STK_SIZE 		256
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//�������ȼ�
#define LED1_TASK_PRIO		5
//�����ջ��С	
#define LED1_STK_SIZE       256
//������ƿ�
OS_TCB Led1TaskTCB;
//�����ջ	
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *p_arg);

//�������ȼ�
#define FLOAT_TASK_PRIO		6
//�����ջ��С
#define FLOAT_STK_SIZE		256
//������ƿ�
OS_TCB	FloatTaskTCB;
//�����ջ
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//������
void float_task(void *p_arg);

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	HAL_Init();                     //��ʼ��HAL��
	Stm32_Clock_Init(20,1,RCC_PLLP_DIV7,RCC_PLLQ_DIV2,RCC_PLLR_DIV2);
	delay_init(80);                //��ʼ����ʱ����
	uart_init(115200);
	uart2_init(2400);              //��ʼ��USART
	LED_Init();                     //��ʼ��LED 
	MY_ADC_Init();
//		TIM7->CR1|=1<<0;
	
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //����ѡ��,Ϊ�˱���������������񶼱��渡��Ĵ�����ֵ
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
	while(1)
	{
	} 
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����s
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���

	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);				
				 
	OSTaskCreate((OS_TCB 	* )&Led1TaskTCB,		
				 (CPU_CHAR	* )"led1 task", 		
                 (OS_TASK_PTR )led1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     	
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
				 
	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
				 (CPU_CHAR	* )"float test task", 		
                 (OS_TASK_PTR )float_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���			 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
}

//��������������
void led0_task(void *p_arg)
{
	OS_ERR err;
//	p_arg = p_arg;
	while(1)
	{
		GPIOB->ODR ^= 1<<3;
		OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err); 
		
		Control();
	}
}

//���ݲɼ�����������
void led1_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
//	p_arg = p_arg;
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	DS18B20_Init();
	OS_CRITICAL_EXIT();		//�˳��ٽ���
	
	esp8266_init();  //esp8266ģ���ʼ��
	while(1)
	{
		//�ɼ�MQ2��MQ135�����ݵȼ�
		MQ2_Air_Quality=MQ2_Collect(20);
		MQ135_Air_Quality=MQ135_Collect(20);
		
		//DS18B20�ɼ�ʱʱ���ܶϣ�����Ҫ�����ٽ���
		OS_CRITICAL_ENTER();	//�����ٽ���
		DS18B20_Temperature=DS18B20_Get_Temp();
		OS_CRITICAL_EXIT();		//�˳��ٽ�
		
		//�ж�PM2.5�Ƿ�ɼ����
		if(GP2Y1050_Times>=GP2Y1050_COLLECT_TIMES)
		{
			if(GP2Y1050_Quality<100) PM2_5_Quality='A';
			else if(GP2Y1050_Quality<150) PM2_5_Quality='B';
			else PM2_5_Quality='C';		
//			printf("GP2Y1050_Quality:%f\r\n",GP2Y1050_Quality);
			GP2Y1050_Times=0;
			__HAL_UART_ENABLE_IT(&UART2_Handler,UART_IT_RXNE);
		}
		
		//��ϸ�������������Զ�̷�������֮���Ը���ô������Ϊ�ֻ�APP�����Ǳ��������Ƶģ���Щ��0�Լ���APP
		sprintf((char*)ESP8266_TX_BUF,"{0,%c,%c,0,0,%2.1f��C,%d,%d,0,%1d,%1d,%1d,0,0,0,0,%c}",
			MQ2_Air_Quality?MQ2_Air_Quality:'0',MQ135_Air_Quality?MQ135_Air_Quality:'0',DS18B20_Temperature,Manual_STA,AUTO_STA,
		(GPIOA->ODR&(1<<8))?0:1,(GPIOA->ODR&(1<<11))?1:0,(GPIOB->ODR&(1<<4))?128:0,PM2_5_Quality?PM2_5_Quality:'0');
		Send_NetworkData(0,ESP8266_TX_BUF);//���ݷ�����������
		
		OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ2s
	}
}

//Զ�������������
void float_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
//	static float float_num=0.00;
	while(1)
	{
		
		
		//����ɨ��
		if((ESP8266_STA&7)==5)
		{
//			printf("ESP8266_CMD:%s\r\n",ESP8266_Cmd);
			Cmd_Scan();
			ESP8266_STA&=(1<<7);
		}
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ20ms
	}
}
