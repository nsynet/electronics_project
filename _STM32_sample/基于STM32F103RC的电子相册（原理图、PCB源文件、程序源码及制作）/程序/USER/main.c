#include "delay.h"
#include "sys.h"
#include "LCD.h"
#include "malloc.h"  
#include "SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "piclib.h"	
#include "string.h"	
#include "stm32f10x.h"
#include "key.h"
//ALIENTEK Mini STM32开发板范例代码31
//图片显示 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
    							   	 
//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO tfileinfo;	//临时文件信息	
	u8 *fn;	 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); 	//打开目录
  	tfileinfo.lfsize=_MAX_LFN*2+1;				//长文件名最大长度
	tfileinfo.lfname=mymalloc(tfileinfo.lfsize);//为长文件缓存区分配内存
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//查询总的有效文件数
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件	
			{
				rval++;//有效文件数增加1
			}	    
		}  
	} 
	return rval;
}
void PA0_Init(void) //PWM
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period = 99; 
	TIM_TimeBaseInitStruct.TIM_Prescaler = 35;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	
    TIM_OCInitStruct.TIM_Pulse = 49;	
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);	
	
	TIM_Cmd(TIM2, ENABLE);
}
void Start()
{
	LCD_Clear(0x3666);
	delay_ms(500);
	LCD_ShowString(36,110,450,24,24,"Never give up what you really want",Black,White);	 
	LCD_ShowString(60,160,400,24,24,"to,Because you will regret it.",Black,White);	 
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	LCD_Clear(Black);
}
u8 SD_Check()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)==0)return 0;
	else return 1;
}
 int main(void)
 { 
	u8 res;
 	DIR picdir;	 		//图片目录
	FILINFO picfileinfo;//文件信息
	u8 *fn;   			//长文件名
	u8 *pname;			//带路径的文件名
	u16 totpicnum; 		//图片文件总数
	u16 curindex;		//图片当前索引
	u8 key;				//键值
	u8 pause=0;			//暂停标记
	u16 t;
	u16 temp;
	u16 *picindextbl;	//图片索引表 	
  u8 light=49;
	 
	delay_init();	    	 //延时函数初始化
	PA0_Init();
	LCD_GPIO_Init();	 
	LCD_Init();			//初始化液晶
	KEY_Init();			//按键初始化	  													    
 	mem_init();			//初始化内部内存池	 
 	exfuns_init();		//为fatfs相关变量申请内存  
	Start();
	LCD_Clear(Black);
	while(SD_Check())
	{
		LCD_ShowString(80,160,300,24,24,"No SD Card!!!",Black,Red);	  
	}
  f_mount(fs[0],"0:",1);
	
 	while(f_opendir(&picdir,"0:/PICTURE"))//打开图片文件夹
 	{	    
		LCD_ShowString(80,160,300,24,24,"No 'PICTURE' Folder!!!",Black,Red);	  
	}  
	totpicnum=pic_get_tnum("0:/PICTURE"); //得到总有效文件数
  	while(totpicnum==NULL)//图片文件为0		
 	{	    
		LCD_ShowString(80,160,300,24,24,"No Pictures!!!",Black,Red);	  			  
	}
  	picfileinfo.lfsize=_MAX_LFN*2+1;						//长文件名最大长度
	picfileinfo.lfname=mymalloc(picfileinfo.lfsize);	//为长文件缓存区分配内存
 	pname=mymalloc(picfileinfo.lfsize);				//为带路径的文件名分配内存
 	picindextbl=mymalloc(2*totpicnum);				//申请2*totpicnum个字节的内存,用于存放图片索引
 	while(picfileinfo.lfname==NULL||pname==NULL||picindextbl==NULL)//内存分配出错
 	{	    
		LCD_ShowString(80,160,300,24,24,"Memory Allocation Failed!!!",Black,Red);				  
	}  	
	//记录索引
    res=f_opendir(&picdir,"0:/PICTURE"); //打开目录
	if(res==FR_OK)
	{
		curindex=0;//当前索引为0
		while(1)//全部查询一遍
		{
			temp=picdir.index;								//记录当前index
	        res=f_readdir(&picdir,&picfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||picfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
     		fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件	
			{
				picindextbl[curindex]=temp;//记录索引
				curindex++;
			}	    
		} 
	}   
	delay_ms(1500);
	piclib_init();										//初始化画图	   	   
	curindex=0;											//从0开始显示
   	res=f_opendir(&picdir,(const TCHAR*)"0:/PICTURE"); 	//打开目录
	while(res==FR_OK)//打开成功
	{	
		dir_sdi(&picdir,picindextbl[curindex]);			//改变当前目录索引	   
        res=f_readdir(&picdir,&picfileinfo);       		//读取目录下的一个文件
        if(res!=FR_OK||picfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出
     	fn=(u8*)(*picfileinfo.lfname?picfileinfo.lfname:picfileinfo.fname);			 
		strcpy((char*)pname,"0:/PICTURE/");				//复制路径(目录)
		strcat((char*)pname,(const char*)fn);  			//将文件名接在后面
 		LCD_Clear(Black);
 		ai_load_picfile(pname,0,0,480,320,1);//显示图片    
		t=0;
		while(1) 
		{
			key=KEY_Scan();		//扫描按键
			if(t>500)
			{
				t=0;
				if(pause==0)
					key=1;
			}
			if(key==2)		//上一张
			{
				if(curindex)curindex--;
				else curindex=totpicnum-1;
				break;
			}else if(key==1)//下一张
			{
				curindex++;		   	
				if(curindex>=totpicnum)curindex=0;//到末尾的时候,自动从头开始
				break;
			}else if(key==3)//暂停
			{
				pause=!pause;		   	
			}else if(key==4)//调亮度
			{
				light=light+15;
				if(light>100)light=10;
				TIM_SetCompare1(TIM2,light-1);
			}
			t++;
			delay_ms(20); 
		}					    
		res=0;  
	} 											  
	myfree(picfileinfo.lfname);	//释放内存			    
	myfree(pname);				//释放内存			    
	myfree(picindextbl);		//释放内存			    
}






