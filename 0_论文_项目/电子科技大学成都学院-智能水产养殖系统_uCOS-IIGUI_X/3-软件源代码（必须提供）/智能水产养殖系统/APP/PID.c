//　　
//　　 标准的PID处理例程 
//　　 --- 工业控制中常用算法 ---
//　　
/*====================================================================================================
　　 这是从网上找来的一个比较典型的PID处理程序，在使用单片机作为控制cpu时，请稍作简化，具体的PID
　　参数必须由具体对象通过实验确定。由于单片机的处理速度和ram资源的限制，一般不采用浮点数运算，
　　而将所有参数全部用整数，运算到最后再除以一个2的N次方数据（相当于移位），作类似定点数运算，可
　　大大提高运算速度，根据控制精度的不同要求，当精度要求很高时，注意保留移位引起的“余数”，做好余
　　数补偿。这个程序只是一般常用pid算法的基本架构，没有包含输入输出处理部分。
=====================================================================================================*/
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "pwmzhiliu.h"	
#include "PID.h"
		 

extern volatile  short status2;
extern int status2moto;
 int rOut; // PID Response (Output)
 int NextPoint = 0; // PID Feedback (Input)
 int circle=1;

//void PIDConfig(PID *sPID);
// void PIDtiaozheng(PID *sPID)
/*====================================================================================================
　　 PID Function
　　 
　　 The PID (比例、积分、微分) function is used in mainly
　　 control applications. PIDCalc performs one iteration of the PID
　　 algorithm.
　　
　　 While the PID function works, main is just a dummy program showing
　　 a typical usage.
　　=====================================================================================================*/


/*====================================================================================================
　　 PID计算部分----增量式PID算法
=====================================================================================================*/
int PIDCalc_delt( PID *pp, int NextPoint )
{
        int dError, Error;
        Error = pp->SetPoint - NextPoint;       // 比例部分的  最新一次采样计算的偏差
        dError= pp->Proportion * Error  +  pp->Integral * pp->LastError  +  pp->Derivative * pp->PrevError;
        pp->PrevError = pp->LastError;          // 微分部分的  更新上一次偏差为最新偏差
        pp->LastError = Error; 
        return (dError);
}
/*====================================================================================================
　　 PID计算部分---位置式PID算法
　　=====================================================================================================*/

int PIDCalc( PID *pp, int NextPoint1 )
{
	int dError, Error;
	Error = pp->SetPoint - NextPoint1;       // 比例部分的  最新一次采样计算的偏差
//	pp->SumError += Error;                  // 积分部分的  所有偏差的累加
//	dError = pp->LastError - pp->PrevError; // 微分部分的  最新一次偏差和前一次偏差的减法计算
//	pp->PrevError = pp->LastError;          // 微分部分的  更新上一次偏差为最新偏差
//	pp->LastError = Error;                  // 微分部分的  更新最新偏差为当前偏差
//	return (pp->Proportion * Error  +  pp->Integral * pp->SumError  +  pp->Derivative * dError); 
	return (pp->Proportion * Error  );                                     // 比例项部分 + 积分项部分 + 微分项部分
}

/*====================================================================================================
　　 Initialize PID Structure
=====================================================================================================*/

void PIDInit (PID *pp)
{
 memset ( pp,0,sizeof(PID));
}

/*====================================================================================================
　　 Main Program
　　=====================================================================================================*/

int sensor (void) // Dummy Sensor Function
{
 
return 100.0;
}

void actuator( rDelta) // Dummy Actuator Function
{ 
//	function(30000,rDelta);
//	if(rDelta==0)
//	function(30000,rDelta); 
//	else
//	{
	function(rDelta,rDelta);
//	}
}


void PIDConfig(PID *sPID)
{
	
	// PID sPID; // PID Control Structur
	
	PIDInit (sPID); // Initialize Structure
	sPID->Proportion = 100; // Set PID Coefficients
	sPID->Integral = 0;
	sPID->Derivative = 0;
	sPID->SetPoint =10; // Set PID Setpoint
	//sPID.SumError =0;
}

void PIDtiaozheng(PID *sPID)
{
	// PID sPID; // PID Control Structure
	// while(1)
	// {  
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==1) //判断是否是PA0线变高--表示转换已完成
	{	
//		status2 = SPI_Read(0x07);
//		status2 = 120;
//		if(status2>127)
//		status2=status2-255;	
//		status2moto=status2;
//		NextPoint = status2moto; // Read Inpu
		rOut =  PIDCalc ( sPID, NextPoint ); // Perform PID Interation		 //总是为零
		//rOut=100;     
		actuator ( rOut ); // Effect Needed Changes
	} 
}







