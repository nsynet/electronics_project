/***************************************************************************************
									声明
本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现其他不可
估量的BUG，修远智控不负任何责任。请勿商用！

程序版本号：	2.0
日期：			2017-1-1
作者：			东方萧雨
版权所有：		修远智控N0.1实验室
****************************************************************************************/
#include "stm32f10x.h"
#include "mpu_dmp_useapi.h"
#include "usart.h"
#include "i2c_moni.h"
#include "dmp_exti.h"
#include "nvic.h"
#include "mpu6050_i2cmoni.h"
#include "led.h"
#include "moto_pwm.h"
#include "systick.h"
#include "control.h"
#include "nRF.h"
#include "checksignal.h"


int main(void)
{
	USART_Config();
	printf("usart is ready\r\n");
	
	//各中断优先级配置
	NVIC_PriorityConfig();
	
	SysTick_init();
	
	//点亮LED灯
	LED_Config();
	LED_On(LED1|LED2|LED3|LED4|LED5|LED6|LED7|LED8);
	
	//初始化模拟I2C
	I2C_MoniConfig();
	
	//初始化MPU6050
	mpu_dmp_init();
	
	//设置哪些数据上报给上位机
	SetReportFlag(RESET,RESET);
	
	//初始化电机控制引脚
	MOTO_PwmConfig();
	
	//无线配置
	NRF_Config();
	
	//无线信号通断检查配置
	CheckSignal_Config();
	
	//接收DMP中断完成信号的EXTI初始化
	DMP_EXTIConfig();
 
	while(1){}
}

