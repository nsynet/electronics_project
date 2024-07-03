/****************************************************************************
     ucgui版本： 3.90/3.98a
	   ucos版本： 2.86a
 ST固件库库版本： 3.50

************************************************************/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "stm32f10x.h"
#include "demo.h"


#define LED1_OFF GPIO_SetBits(GPIOA, GPIO_Pin_0);  
#define LED1_ON GPIO_ResetBits(GPIOA, GPIO_Pin_0); 

#define LED2_OFF GPIO_SetBits(GPIOA, GPIO_Pin_1);  
#define LED2_ON GPIO_ResetBits(GPIOA, GPIO_Pin_1); 

#define LED3_OFF GPIO_SetBits(GPIOA, GPIO_Pin_2);  
#define LED3_ON GPIO_ResetBits(GPIOA, GPIO_Pin_2);

#define LED4_OFF GPIO_SetBits(GPIOA, GPIO_Pin_3);  
#define LED4_ON GPIO_ResetBits(GPIOA, GPIO_Pin_3);

#define LED5_OFF GPIO_SetBits(GPIOA, GPIO_Pin_4);  
#define LED5_ON GPIO_ResetBits(GPIOA, GPIO_Pin_4); 

#define LED6_OFF GPIO_SetBits(GPIOA, GPIO_Pin_5);  
#define LED6_ON GPIO_ResetBits(GPIOA, GPIO_Pin_5); 
		   
#define LED7_OFF GPIO_SetBits(GPIOA, GPIO_Pin_6);  
#define LED7_ON GPIO_ResetBits(GPIOA, GPIO_Pin_6);  

#define LED8_OFF GPIO_SetBits(GPIOA, GPIO_Pin_7);  
#define LED8_ON GPIO_ResetBits(GPIOA, GPIO_Pin_7);     

#define LED9_ON GPIO_SetBits(GPIOA, GPIO_Pin_9);  
#define LED9_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_9);     
#define LED10_ON GPIO_SetBits(GPIOA, GPIO_Pin_10);  
#define LED10_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_10);     
#define LED11_ON GPIO_SetBits(GPIOA, GPIO_Pin_11);  
#define LED11_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_11);     
#define LED12_ON GPIO_SetBits(GPIOA, GPIO_Pin_12);  
#define LED12_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_12);     


#define LED_OFF_ALL GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //关闭所有LED  


int test=0;
int data0=0,data1=1000,data2=1000,data3=0,data4=0;
int shexiangtou=0;
GUI_COLOR DesktopColorOld;

const GUI_FONT* pFont = &GUI_FontHZ_SimSun_13; 





static const GUI_WIDGET_CREATE_INFO aDialogCreate2[] = {
    { FRAMEWIN_CreateIndirect,  "主菜单",          0,                       0,  0,  400,240,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,    "抽水",                GUI_ID_BUTTON0,          15, 182,52, 27, 0,0},
    { BUTTON_CreateIndirect,    "测试",              GUI_ID_BUTTON1,          91,182,52, 27, 0,0},
    { BUTTON_CreateIndirect,    "水质监测",              GUI_ID_BUTTON2,          167,182,52, 27, 0,0},
    { BUTTON_CreateIndirect,    "自动灌溉",              GUI_ID_BUTTON3,          246,182,52, 27, 0,0},
	{ BUTTON_CreateIndirect,    "步进电机",              GUI_ID_BUTTON5,          325,182,52, 27, 0,0},
    { TEXT_CreateIndirect,      "水下温度",              GUI_ID_TEXT0,            13, 20, 53, 12, 0,0},
    { TEXT_CreateIndirect,      "光照强度",              GUI_ID_TEXT1,            13, 97, 53, 12, 0,0},
    { TEXT_CreateIndirect,      "农田湿度",              GUI_ID_TEXT2,            13, 135,53, 12, 0,0},
    { TEXT_CreateIndirect,      "水质评估",              GUI_ID_TEXT3,            138,21, 53, 12, 0,0},
    { TEXT_CreateIndirect,      "水量报告",              GUI_ID_TEXT4,            13, 59, 53, 12, 0,0},
    { TEXT_CreateIndirect,      "沼气浓度",              GUI_ID_TEXT5,            263,59, 53, 12, 0,0},
    { EDIT_CreateIndirect,      "21.5℃ ",                 GUI_ID_EDIT0,            76, 17, 51, 18, 0,0},
    { TEXT_CreateIndirect,      "鱼塘监控",              GUI_ID_TEXT6,            138,59, 53, 12, 0,0},
    { TEXT_CreateIndirect,      "农场监控",              GUI_ID_TEXT7,            263,21, 53, 12, 0,0},
    { TEXT_CreateIndirect,      "自动",                GUI_ID_TEXT8,            280,110, 29, 12, 0,0},
    { TEXT_CreateIndirect,      "手动",                GUI_ID_TEXT9,            280,130,29, 12, 0,0},
    { TEXT_CreateIndirect,      "水面高度",              GUI_ID_TEXT10,           323,95, 53, 12, 0,0},
    { EDIT_CreateIndirect,      "L",                 GUI_ID_EDIT1,            76, 56, 51, 18, 0,0},
    { EDIT_CreateIndirect,      "0",                 GUI_ID_EDIT2,            76, 95, 51, 18, 0,0},
    { EDIT_CreateIndirect,      "优",                 GUI_ID_EDIT3,            195,17, 51, 18, 0,0},
    { EDIT_CreateIndirect,      "良好",                 GUI_ID_EDIT4,            322,14, 51, 18, 0,0},
    { EDIT_CreateIndirect,      "优",                 GUI_ID_EDIT5,            195,56, 51, 18, 0,0},
    { EDIT_CreateIndirect,      "0",                 GUI_ID_EDIT6,            322,59, 51, 18, 0,0},
    { EDIT_CreateIndirect,      "50%rh",                 GUI_ID_EDIT7,            76, 134,51, 18, 0,0},
  
    { EDIT_CreateIndirect,      "0",                 GUI_ID_EDIT9,            320,120, 41, 18, 0,0},
    { TEXT_CreateIndirect,      "mm",                GUI_ID_TEXT10,           365,125, 17, 12, 0,0},
    { RADIO_CreateIndirect,      NULL,               GUI_ID_RADIO0,           260,110, 20, 39, 0,2},
    { CHECKBOX_CreateIndirect,  "摄像头",               GUI_ID_CHECK0,           138,134,60, 16, 0,0},
    { CHECKBOX_CreateIndirect,  "排气扇",               GUI_ID_CHECK1,           198,134,60, 16, 0,0},
 
  
   
};


    
static const GUI_WIDGET_CREATE_INFO aDialogCreate4[] = {
    { FRAMEWIN_CreateIndirect,  "",            0,                       0,  0,  400,240,0,0},
	{ SLIDER_CreateIndirect,     NULL,               GUI_ID_SLIDER0,          90,130,200, 34, 0,0},
	{ SLIDER_CreateIndirect,     NULL,               GUI_ID_SLIDER1,          90,80,200, 34, 0,0},
	{ SLIDER_CreateIndirect,     NULL,               GUI_ID_SLIDER2,          90,30,200, 34, 0,0},
	{ SLIDER_CreateIndirect,     NULL,               GUI_ID_SLIDER4,          90,180,200, 34, 0,0},
	{ TEXT_CreateIndirect,      "抽水",              GUI_ID_TEXT11,           40,30,53, 12, 0,0},
	{ TEXT_CreateIndirect,      "抽水",              GUI_ID_TEXT12,           40,80,53, 12, 0,0},
	{ TEXT_CreateIndirect,      "1",              GUI_ID_TEXT13,           83,30,6, 12, 0,0},
	{ TEXT_CreateIndirect,      "2",              GUI_ID_TEXT14,           83,80,6, 12, 0,0},
	{ TEXT_CreateIndirect,      "排气扇",              GUI_ID_TEXT15,           40,130,53, 12, 0,0},
	{ TEXT_CreateIndirect,      "光线",              GUI_ID_TEXT16,           40,180,53, 12, 0,0},
	{ BUTTON_CreateIndirect,    "主菜单",                GUI_ID_BUTTON4,           303,182,72, 27, 0,0},
};

       
static const GUI_WIDGET_CREATE_INFO aDialogCreate3[] = {
	{ FRAMEWIN_CreateIndirect,  "抽水",            0,                       0,  0,  400,240,0,0},
	{ SLIDER_CreateIndirect,     NULL,               GUI_ID_SLIDER3,          50,80,200, 64, 0,0},
	{ BUTTON_CreateIndirect,    "主菜单",                GUI_ID_BUTTON6,          303,182,72, 27, 0,0},

	  
};

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintDialog(WM_MESSAGE * pMsg)
{
   WM_HWIN hWin = pMsg->hWin;
}


/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    //
    //GUI_ID_BUTTON0
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),BUTTON_CI_UNPRESSED,0x00c000);
    //
    //GUI_ID_BUTTON1
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),BUTTON_CI_UNPRESSED,0x00ffff);
    //
    //GUI_ID_BUTTON2
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),BUTTON_CI_UNPRESSED,0x0000ff);
	   //
    //GUI_ID_BUTTON0
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON3),BUTTON_CI_UNPRESSED,0xee82ee);
    //    
    //GUI_ID_BUTTON0
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON4),BUTTON_CI_UNPRESSED,0xee82ee);
	     //GUI_ID_BUTTON0
    //
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON5),BUTTON_CI_UNPRESSED,0xFF0000);
    //GUI_ID_RADIO0
    //

    RADIO_SetGroupId(WM_GetDialogItem(hWin,GUI_ID_RADIO0),0);
    RADIO_SetText(WM_GetDialogItem(hWin,GUI_ID_RADIO1),"",0);
    RADIO_SetText(WM_GetDialogItem(hWin,GUI_ID_RADIO2),"",1);
    //
  
  	// GUI_ID_SLIDER0
	  

	SLIDER_SetRange(WM_GetDialogItem(hWin,GUI_ID_SLIDER0),0,1000);
	SLIDER_SetValue(WM_GetDialogItem(hWin,GUI_ID_SLIDER0),data0);
	SLIDER_SetWidth(WM_GetDialogItem(hWin,GUI_ID_SLIDER0),20);
	
	SLIDER_SetRange(WM_GetDialogItem(hWin,GUI_ID_SLIDER1),0,1000);
	SLIDER_SetValue(WM_GetDialogItem(hWin,GUI_ID_SLIDER1),1000-data1);
	SLIDER_SetWidth(WM_GetDialogItem(hWin,GUI_ID_SLIDER1),20);
	
	SLIDER_SetRange(WM_GetDialogItem(hWin,GUI_ID_SLIDER2),0,1000);
	SLIDER_SetValue(WM_GetDialogItem(hWin,GUI_ID_SLIDER2),1000-data2);
	SLIDER_SetWidth(WM_GetDialogItem(hWin,GUI_ID_SLIDER2),20);
	
	SLIDER_SetRange(WM_GetDialogItem(hWin,GUI_ID_SLIDER3),0,1000);
	SLIDER_SetValue(WM_GetDialogItem(hWin,GUI_ID_SLIDER3),data3);
	SLIDER_SetWidth(WM_GetDialogItem(hWin,GUI_ID_SLIDER3),20);
	
	SLIDER_SetRange(WM_GetDialogItem(hWin,GUI_ID_SLIDER4),0,1000);
	SLIDER_SetValue(WM_GetDialogItem(hWin,GUI_ID_SLIDER4),data4);
	SLIDER_SetWidth(WM_GetDialogItem(hWin,GUI_ID_SLIDER4),20);
	
	EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),"21.5℃ ");
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT0),&GUI_FontHZ_NewSimSun_13);
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT1),"L");
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT2),adctempchar0);
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT3),"优");
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT4),"良好");
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT5),"良");
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT6),adctempchar1);
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT7),"50%rh");
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT8),"0");
   //
    //GUI_ID_CHECK0
	   EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT9),"0");
    //
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK0),"摄像头");
    //
    //GUI_ID_CHECK1
    //
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK1),"排气扇");
    //
    //GUI_ID_PROGBAR0
    //
}

/****************************************************************************
* 名    称：static void _cbCallback(WM_MESSAGE * pMsg)  
* 功    能：窗体回调函数 
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
	int NCode, Id;
	float Temp=26.1;
	static char NUM=0;
	WM_HWIN hWin2,hWin3;  
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);


				/* 设置窗体字体 */
			FRAMEWIN_SetFont(hWin, pFont);
			  /* 获得TEXT 部件的句柄 */	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT0);			   
			TEXT_SetFont(hWin2,pFont);
			  	  
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT1);			   
			TEXT_SetFont(hWin2,pFont);							    //设置对话框里文本框的字体
			
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT2);			   
			TEXT_SetFont(hWin2,pFont);	
			
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT3);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT4);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT5);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT6);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT7);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT8);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT9);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT10);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT11);			   
			TEXT_SetFont(hWin2,pFont);	
			  hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT12);			   
			TEXT_SetFont(hWin2,pFont);	
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT15);			   
			TEXT_SetFont(hWin2,pFont);	
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_TEXT16);			   
			TEXT_SetFont(hWin2,pFont);	

 		
     
/******************************按钮字体设置********************************/
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_BUTTON0);			   
			BUTTON_SetFont(hWin2,pFont);							   
			
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_BUTTON1);			   
			BUTTON_SetFont(hWin2,pFont);							   
			
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_BUTTON2);			   
			BUTTON_SetFont(hWin2,pFont);
		
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_BUTTON3);			   
			BUTTON_SetFont(hWin2,pFont);
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_BUTTON4);			   
			BUTTON_SetFont(hWin2,pFont);
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_BUTTON5);			   
			BUTTON_SetFont(hWin2,pFont);
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_BUTTON6);			   
			BUTTON_SetFont(hWin2,pFont);
		
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_EDIT3);
			EDIT_SetFont(hWin2,pFont);
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_EDIT4);
			EDIT_SetFont(hWin2,pFont);
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_EDIT5);
			EDIT_SetFont(hWin2,pFont);
		
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_CHECK0);
			CHECKBOX_SetFont(hWin2,pFont);	
			hWin2 = WM_GetDialogItem(hWin, GUI_ID_CHECK1);
			CHECKBOX_SetFont(hWin2,pFont);

	  
            break;
			
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
			    int dianjispeed;
			   	Delay(0xFFFF);
	
			   case GUI_ID_CHECK0:
					hWin2 = WM_GetDialogItem(hWin, GUI_ID_CHECK0);	
					if(CHECKBOX_IsChecked(hWin2)==1)
					{
						SXTtable=1;
					}
					else
					{
						SXTtable=3;
					}
			case  GUI_ID_BUTTON1 :	      
					test=1;  
					break;        		
			case  GUI_ID_BUTTON4 :  
					test=2;
					break;           
	      
		 	case  GUI_ID_BUTTON5 :  
			      	 test=3;
					break;           
	        
		 	case  GUI_ID_BUTTON6 :  
					test=2;
					break;    
	
			case GUI_ID_SLIDER0: /*窗体部件的值被改变 */  
					hWin2 = WM_GetDialogItem(hWin,GUI_ID_SLIDER0);
					if ((Id == GUI_ID_SLIDER0)) 
					{							 //slider0 的值被改变
						data0=SLIDER_GetValue(hWin2);					 //获得slider0的值
						TIM_SetCompare2(TIM3,data0*48);
					}		 
					break;
		 	case GUI_ID_SLIDER1: /*窗体部件的值被改变 */	 
					hWin2 = WM_GetDialogItem(hWin,GUI_ID_SLIDER1);
					if ((Id == GUI_ID_SLIDER1)) 
					{							 //slider0 的值被改变
						data1=SLIDER_GetValue(hWin2);					 //获得slider0的值
						data1=1000-data1;
						TIM_SetCompare3(TIM3,data1*48);
				
					}		 
					break;
	
		 	case GUI_ID_SLIDER2: /*窗体部件的值被改变 */ 
					hWin2 = WM_GetDialogItem(hWin,GUI_ID_SLIDER2);
					if ((Id == GUI_ID_SLIDER2)) 
					{							 //slider0 的值被改变
						data2=SLIDER_GetValue(hWin2);					 //获得slider0的值
						data2=1000-data2;
						TIM_SetCompare4(TIM3,data2*48);
					}		
					break;
	
		 	case GUI_ID_SLIDER3: /*窗体部件的值被改变 */ 
					hWin2 = WM_GetDialogItem(hWin,GUI_ID_SLIDER3);
					if ((Id == GUI_ID_SLIDER3)) 
					{							 //slider0 的值被改变
					data3=SLIDER_GetValue(hWin2);	 //获得slider0的值
					break;
					case GUI_ID_SLIDER4: /*窗体部件的值被改变 */ 
					hWin2 = WM_GetDialogItem(hWin,GUI_ID_SLIDER4);
					if ((Id == GUI_ID_SLIDER4)) 
					{							 //slider0 的值被改变
						data4=SLIDER_GetValue(hWin2);					 //获得slider0的值
						TIM_SetCompare1(TIM4,data4*48);
					}		
					break;
			default:
				    WM_DefaultProc(pMsg);
				 	 break;
			}
		    break;
		}
	}
}

/****************************************************************************
* 名    称：void Fun(void)
* 功    能：显示及处理界面
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void Fun(void)
{ 
	int dianchi;
	int bujin=0;

	WM_HWIN hWin1,hWin2,hWin3,hWin4;  
	DesktopColorOld = WM_SetDesktopColor(GUI_RED);  //背景颜色
	GUI_CURSOR_Show();
	WM_SetCreateFlags(WM_CF_MEMDEV);       
 
	WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
	WM_SetCreateFlags(WM_CF_MEMDEV); 


   hWin4=GUI_CreateDialogBox(aDialogCreate4, GUI_COUNTOF(aDialogCreate4), _cbCallback, 0, 0, 0);
   hWin3=GUI_CreateDialogBox(aDialogCreate3, GUI_COUNTOF(aDialogCreate3), _cbCallback, 0, 0, 0);
   hWin2=GUI_CreateDialogBox(aDialogCreate2, GUI_COUNTOF(aDialogCreate2), _cbCallback, 0, 0, 0);

	Delay(10000);
	LED2_OFF;
	LED3_OFF;


	while(1)
	{
		uartPut("abcdefgh");
		OSTimeDlyHMSM(0, 0, 0, 30);		
	
	
	/**************************************************************************/
	/******************************电池容量显示********************************/
	/**************************************************************************/
		EDIT_SetText(WM_GetDialogItem(hWin4,GUI_ID_EDIT2),adctempchar0);		
		EDIT_SetText(WM_GetDialogItem(hWin4,GUI_ID_EDIT6),adctempchar1);
				GUI_Clear();														   			  
		switch(test)
		{
			case 1:	 
					GUI_EndDialog(hWin2,0);
					GUI_CreateDialogBox(aDialogCreate4, GUI_COUNTOF(aDialogCreate4), _cbCallback, 0, 0, 0);
				    test=0;
					break;
			case 2:   
					GUI_EndDialog(hWin2,0);
					GUI_CreateDialogBox(aDialogCreate2, GUI_COUNTOF(aDialogCreate2), _cbCallback, 0, 0, 0);
					test=0; 
					break;
			case 3:   
					GUI_EndDialog(hWin2,0);
					GUI_CreateDialogBox(aDialogCreate3, GUI_COUNTOF(aDialogCreate3), _cbCallback, 0, 0, 0);
					test=0;
					break;
									 
			default:  test=0;
					break;
		}
		WM_Exec();												    //显示刷新   
   }	 
}
