#ifndef __STM32F10x_PID_H
#define __STM32F10x_PID_H

/** 
  * @brief  PID control structure definition  
  */
typedef struct 
{
        int SetPoint;   // 设定目标值r(t) Desired Value

        int Proportion; // 比例常数k(p) Proportional Const
        int Integral;   // 积分常数k(i) Integral Const
        int Derivative; // 微分常数k(d) Derivative Const

        int LastError; // 微分部分的 最新偏差e(k)    Error[-1]
        int PrevError; // 微分部分的 最新偏差e(k-1)  Error[-2]

        int SumError;  // 积分部分的 累加偏差   Sums of Errors
}PID;


void PIDConfig(PID *sPID);
void PIDtiaozheng(PID *sPID);




#endif /* __STM32F10x_pwmzhiliu_H */



