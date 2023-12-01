//proteus仿真论坛：proteus.5d6d.com
/*****************************************************
Project : 汽车空调控制系统
Version : 1
Date    : 2005-12-13
Author  : Benny 
Blog    : http://bennylo.mcublog.com                         
Company : 509                             
Comments: 


Chip type           : ATmega16L
Program type        : Application
Clock frequency     : 8.000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
*****************************************************/

#include <mega16.h> 
#include <stdlib.h> 
#include "Pt1000Tab.h"
#include "inc.h"

#asm
   .equ __lcd_port=0x18 ;PORTB
#endasm
#include <lcd.h>

bit boolean;   

uchar ventilator_state; 
uchar fan;  
uchar blow; 
uchar run_mode;
uchar temp;
uchar setting_value;
interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
//产生PWM,控制步进电机
if (fan==1)
{
   OCR2=64;
}
else if(fan==2)
{
   OCR2=128;
}
}

#define FIRST_ADC_INPUT 0   //第一通道
#define LAST_ADC_INPUT 1    //最后一通道 ，最大值为7,共8个通道
unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
#define ADC_VREF_TYPE 0x40
// ADC中断服务程序
// 自动扫描模拟量输入端口，
interrupt [ADC_INT] void adc_isr(void)
{
register static unsigned char input_index=0;
// 读取转换结果
adc_data[input_index]=ADCW;
// 选择转换通道
if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
   input_index=0;
ADMUX=(FIRST_ADC_INPUT|ADC_VREF_TYPE)+input_index;
//启动AD转换
ADCSRA|=0x40;
}


void main(void)
{
float current_temp;//保存当前温度
// Port A 初始化
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B 初始化
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C 初始化
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x38;
DDRC=0x38;

// Port D 初始化
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x1f;
DDRD=0xF8;


//用T2产生PWM,控制风向步进电机
// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x64;
TCNT2=0x00;
OCR2=0x00;


// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x80;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 125.000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: Free Running
ADMUX=FIRST_ADC_INPUT|ADC_VREF_TYPE;
ADCSRA=0xEE;
SFIOR&=0x1F;

// LCD module初始化
lcd_init(16);

// 开启全局中断
#asm("sei")
dis_character(); //在LCD上显示字符

setting_value=25; //默认设定温度为25
run_mode=0;       //默认关闭
ventilator_state=0;
lcd_gotoxy(12,3); 
lcd_putsf("OFF");  
while (1)
      {    
        scan_key();  
        blow_mode();
        display();
        if(run_mode==2)
        {   
            current_temp=Pt1000Tab[adc_data[0]];
            if (current_temp<setting_value)
            {       
              stop_compressor(); 
              //lcd_gotoxy(10,1); 
              //lcd_putsf("Blast");          
            }
            else
            {
              start_compressor();   
              //lcd_gotoxy(10,1); 
              //lcd_putsf("Cool");     
            }        
             
       }

      }                         
}
void start_compressor(void){
  //Start
  PORTD|=0x18;
 
  PORTD&=~0x10; 
 
  PORTD|=0x18; 
}
void stop_compressor(void){
  //Stop
  PORTD|=0x18; 
  PORTD&=~0x08; 
  PORTD|=0x18; 

}
void start_ventilator(void) {
  PORTD|=0x40;//换气风机运行
}
void stop_ventilator(void){
  PORTD&=~0x40;//换气风机停止
} 
void start_evaporator_fan(void){
  PORTD|=0x20;//蒸发器风机运行
}
void stop_evaporator_fan(void){
  PORTD&=~0x20;//蒸发器风机停止
} 
/*-----------------------------------------------键盘扫描-------------------------------------------


-----------------------------------------------键盘扫描-------------------------------------------*/
void scan_key(void){
/* 
   K11    K12     K13
   K21    K22     K23 
   K31    K32     K33 
 */
 
 //K13 K23 K33  
PORTC&=~0x20; 
if((PIND&0x07)!=0x07) 
{   
   delay();
   if((PIND&0x07)!=0x07) 
   {    
        if((PIND&0x07)==0x06) 
        {  //Key 3-3  
          //uchar key_num[]="K33"; 
          while((PIND&0x07)==0x06);
          switch(blow)
          {
            case 0:
            {
              blow=1;
              lcd_gotoxy(10,2); 
              lcd_putsf("Mode0");
              break;        
            }
            case 1:
            {
              blow=2; 
              lcd_gotoxy(10,2); 
              lcd_putsf("Mode1");
              break;        
            } 
            case 2:
            {
              blow=3;
              lcd_gotoxy(10,2); 
              lcd_putsf("Mode2");              
              break;        
            } 
            case 3:
            {
              blow=0;
              lcd_gotoxy(10,2); 
              lcd_putsf("Mode3");
              break;        
            }                                        
          }
          return;
        }
        if((PIND&0x07)==0x05) 
        {  //Key 2-3   
           //uchar key_num[]="K23";           
           while((PIND&0x07)==0x05); 
                       
           if(ventilator_state==1)
           {
              ventilator_state=0;
              stop_ventilator();
              lcd_gotoxy(12,3); 
              lcd_putsf("OFF");             
           }
           else
           {
              ventilator_state=1;
              start_ventilator(); 
              lcd_gotoxy(12,3); 
              lcd_putsf("Run");                
           }           
           return;
        }
        if((PIND&0x07)==0x03) 
        {  //Key 1-3           
          //uchar key_num[]="K13"; 
          while((PIND&0x07)==0x03); 
          switch(run_mode)
          {
            case 2:
            { //关闭模式 
              stop_evaporator_fan();
              stop_compressor();  
              lcd_gotoxy(10,1); 
              lcd_putsf("OFF  "); 
              run_mode=0;
              break; 
            }
            case 0:
            {
              //送风模式  
              start_evaporator_fan();
              stop_compressor(); 
              lcd_gotoxy(10,1); 
              lcd_putsf("Blast");
              run_mode=1; 
              break;       
            }        
            case 1:
            {
              //制冷模式
              start_evaporator_fan();
              start_compressor(); 
              lcd_gotoxy(10,1); 
              lcd_putsf("Cool ");              
              run_mode=2;
              break;           
            }
          } 
 
          return;
        }   
   }      

}

PORTC|=0x20;

 //K12 K22 K32
PORTC&=~0x10; 
if((PIND&0x07)!=0x07)
{
        if((PIND&0x07)==0x06) 
        {  //Key 3-2
           //uchar key_num[]="K32";
           while((PIND&0x07)==0x06);
           return;
        }
        if((PIND&0x07)==0x05) 
        {  //Key 2-2
           //uchar key_num[]="K22";
           while((PIND&0x07)==0x05);
           if (setting_value<=18)
           {
              setting_value=18;
           } 
           else
           {
              setting_value--;
           }
 
           return;
        }
        if((PIND&0x07)==0x03) 
        {  //Key 1-2
           //uchar key_num[]="K12";
           while((PIND&0x07)==0x03);
           if (setting_value>=28)
           {
              setting_value=28;
           } 
           else
           {
              setting_value++;
           }
           return;
        }
}
PORTC|=0x10;
 //K11 K21 K31
PORTC&=~0x08; 
if((PIND&0x07)!=0x07) 
{   
   delay();
   if((PIND&0x07)!=0x07) 
   {    
        if((PIND&0x07)==0x06) 
        {  //Key 3-1  
          //uchar key_num[]="K31"; 
          while((PIND&0x07)==0x06);
          return;
        }
        if((PIND&0x07)==0x05) 
        {  //Key 2-1   
           //uchar key_num[]="K21";           
           while((PIND&0x07)==0x05); 
           return;
        }
        if((PIND&0x07)==0x03) 
        {  //Key 1-1           
           //uchar key_num[]="K11"; 
           while((PIND&0x07)==0x03);  
          return;
        }   
   }      

}
PORTC|=0x08;

}

void blow_mode(void){
switch(blow)
{
  case 0:
  { 
    fan=0;
    break;
  }     
  case 1:
  {
    fan=1;
    break;
  } 
  case 2:
  { 
    fan=2;
    break;
  }  
  case 3:
  {
    temp++;
    if(temp==5)
    {
      boolean=~boolean;          
    }
    if (boolean==1)
    {
      fan=1;
    }
    else
    {
      fan=2;
    }  
    break;
  }      
}
} 
void delay(void){
uint i,j;
for (i=0;i<10;i++)
  { 
  for (j=0;j<10;j++)
     {}
  }
}
void dis(uchar channel,uchar x,uchar y){
   uchar arr[5];  
   ftoa(Pt1000Tab[adc_data[channel]],1,arr);
   //itoa(adc_data[channel],arr);
   lcd_gotoxy(x,y); 
   lcd_puts(arr); 
   //lcd_putchar(0);   
} 
void display(void){  
   uchar setting[3];
   dis(0,2,0);
   itoa(setting_value,setting);
   lcd_gotoxy(12,0); 
   lcd_puts(setting); 
}
void dis_character(void){   
   lcd_gotoxy(0,0); 
   lcd_putsf("T:"); 
   
   lcd_gotoxy(8,0); 
   lcd_putsf("Set:");    
   
   lcd_gotoxy(0,1); 
   lcd_putsf("Run  Mode:");  
   lcd_gotoxy(10,1); 
   lcd_putsf("OFF  "); 

   lcd_gotoxy(0,2); 
   lcd_putsf("Blow Mode:"); 
    
   lcd_gotoxy(10,2); 
   lcd_putsf("Mode0");  
   
   lcd_gotoxy(0,3); 
   lcd_putsf("Ventilator:");     
}









