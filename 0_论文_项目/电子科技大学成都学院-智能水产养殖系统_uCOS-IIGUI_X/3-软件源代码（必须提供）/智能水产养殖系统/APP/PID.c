//����
//���� ��׼��PID�������� 
//���� --- ��ҵ�����г����㷨 ---
//����
/*====================================================================================================
���� ���Ǵ�����������һ���Ƚϵ��͵�PID���������ʹ�õ�Ƭ����Ϊ����cpuʱ���������򻯣������PID
�������������ɾ������ͨ��ʵ��ȷ�������ڵ�Ƭ���Ĵ����ٶȺ�ram��Դ�����ƣ�һ�㲻���ø��������㣬
�����������в���ȫ�������������㵽����ٳ���һ��2��N�η����ݣ��൱����λ���������ƶ��������㣬��
���������������ٶȣ����ݿ��ƾ��ȵĲ�ͬҪ�󣬵�����Ҫ��ܸ�ʱ��ע�Ᵽ����λ����ġ���������������
�������������������ֻ��һ�㳣��pid�㷨�Ļ����ܹ���û�а���������������֡�
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
���� PID Function
���� 
���� The PID (���������֡�΢��) function is used in mainly
���� control applications. PIDCalc performs one iteration of the PID
���� algorithm.
����
���� While the PID function works, main is just a dummy program showing
���� a typical usage.
����=====================================================================================================*/


/*====================================================================================================
���� PID���㲿��----����ʽPID�㷨
=====================================================================================================*/
int PIDCalc_delt( PID *pp, int NextPoint )
{
        int dError, Error;
        Error = pp->SetPoint - NextPoint;       // �������ֵ�  ����һ�β��������ƫ��
        dError= pp->Proportion * Error  +  pp->Integral * pp->LastError  +  pp->Derivative * pp->PrevError;
        pp->PrevError = pp->LastError;          // ΢�ֲ��ֵ�  ������һ��ƫ��Ϊ����ƫ��
        pp->LastError = Error; 
        return (dError);
}
/*====================================================================================================
���� PID���㲿��---λ��ʽPID�㷨
����=====================================================================================================*/

int PIDCalc( PID *pp, int NextPoint1 )
{
	int dError, Error;
	Error = pp->SetPoint - NextPoint1;       // �������ֵ�  ����һ�β��������ƫ��
//	pp->SumError += Error;                  // ���ֲ��ֵ�  ����ƫ����ۼ�
//	dError = pp->LastError - pp->PrevError; // ΢�ֲ��ֵ�  ����һ��ƫ���ǰһ��ƫ��ļ�������
//	pp->PrevError = pp->LastError;          // ΢�ֲ��ֵ�  ������һ��ƫ��Ϊ����ƫ��
//	pp->LastError = Error;                  // ΢�ֲ��ֵ�  ��������ƫ��Ϊ��ǰƫ��
//	return (pp->Proportion * Error  +  pp->Integral * pp->SumError  +  pp->Derivative * dError); 
	return (pp->Proportion * Error  );                                     // ������� + ������� + ΢�����
}

/*====================================================================================================
���� Initialize PID Structure
=====================================================================================================*/

void PIDInit (PID *pp)
{
 memset ( pp,0,sizeof(PID));
}

/*====================================================================================================
���� Main Program
����=====================================================================================================*/

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
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==1) //�ж��Ƿ���PA0�߱��--��ʾת�������
	{	
//		status2 = SPI_Read(0x07);
//		status2 = 120;
//		if(status2>127)
//		status2=status2-255;	
//		status2moto=status2;
//		NextPoint = status2moto; // Read Inpu
		rOut =  PIDCalc ( sPID, NextPoint ); // Perform PID Interation		 //����Ϊ��
		//rOut=100;     
		actuator ( rOut ); // Effect Needed Changes
	} 
}







