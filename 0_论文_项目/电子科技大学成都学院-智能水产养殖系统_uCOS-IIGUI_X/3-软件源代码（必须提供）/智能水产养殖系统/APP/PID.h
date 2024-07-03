#ifndef __STM32F10x_PID_H
#define __STM32F10x_PID_H

/** 
  * @brief  PID control structure definition  
  */
typedef struct 
{
        int SetPoint;   // �趨Ŀ��ֵr(t) Desired Value

        int Proportion; // ��������k(p) Proportional Const
        int Integral;   // ���ֳ���k(i) Integral Const
        int Derivative; // ΢�ֳ���k(d) Derivative Const

        int LastError; // ΢�ֲ��ֵ� ����ƫ��e(k)    Error[-1]
        int PrevError; // ΢�ֲ��ֵ� ����ƫ��e(k-1)  Error[-2]

        int SumError;  // ���ֲ��ֵ� �ۼ�ƫ��   Sums of Errors
}PID;


void PIDConfig(PID *sPID);
void PIDtiaozheng(PID *sPID);




#endif /* __STM32F10x_pwmzhiliu_H */



