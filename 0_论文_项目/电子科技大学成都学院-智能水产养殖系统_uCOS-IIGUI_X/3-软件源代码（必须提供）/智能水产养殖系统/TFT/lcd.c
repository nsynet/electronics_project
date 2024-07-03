#include "stm32f10x.h"
#include "lcd.h"
#include "lcd_font.h"
#include "lcdconf.h"

extern void LCD_WR_Data(unsigned int val);
extern unsigned int LCD_RD_data(void);
extern void LCD_Init1(void);
extern unsigned short DeviceID;


/****************************************************************************
* ��    �ƣ�void lcd_Initializtion()
* ��    �ܣ���ʼ�� lcd ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����lcd_Initializtion();
****************************************************************************/
void lcd_Initializtion()
{ 
   LCD_Init1();
}

/****************************************************************************
* ��    �ƣ�u16 lcd_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
* ˵    ����
* ���÷�����i=lcd_GetPoint(10,10);
****************************************************************************/
u16 lcd_GetPoint(u16 x,u16 y)
{ u16 temp1=0;
  /*  HX8347D  */	   
  	 *(__IO uint16_t *) (Bank1_LCD_C)= 2;	
  	 *(__IO uint16_t *) (Bank1_LCD_D)= (u8)(x>>8);	  //������ʼX������ֽ�

  	 *(__IO uint16_t *) (Bank1_LCD_C)= 3;	
  	 *(__IO uint16_t *) (Bank1_LCD_D)= (u8)(x);		  //������ʼX������ֽ�

  	 *(__IO uint16_t *) (Bank1_LCD_C)= 6;	
  	 *(__IO uint16_t *) (Bank1_LCD_D)= (u8)(y>>8);	  //������ʼY������ֽ�

  	 *(__IO uint16_t *) (Bank1_LCD_C)= 7;	
  	 *(__IO uint16_t *) (Bank1_LCD_D)= (u8)(y);		  //������ʼX������ֽ�
 
   	 *(__IO uint16_t *) (Bank1_LCD_C)= 0x22;	
	 temp1=LCD_RD_data();
    //return (lcd_BGR2RGB(lcd_ReadData()));
	 return (temp1);
}

/****************************************************************************
* ��    �ƣ�void lcd_SetPoint(u16 x,u16 y,u16 point)
* ��    �ܣ���ָ�����껭��
* ��ڲ�����x      ������
*           y      ������
*           point  �����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����lcd_SetPoint(10,10,0x0fe0);
****************************************************************************/
void lcd_SetPoint(u16 x,u16 y,u16 point)
{

  	 *(__IO uint16_t *) (Bank1_LCD_C)= 2;				
  	 *(__IO uint16_t *) (Bank1_LCD_D)= x>>8;			//������ʼX������ֽ�

  	 *(__IO uint16_t *) (Bank1_LCD_C)= 3;	
  	 *(__IO uint16_t *) (Bank1_LCD_D)= (u8)(x);			//������ʼX������ֽ�

  	 *(__IO uint16_t *) (Bank1_LCD_C)= 6;				
  	 *(__IO uint16_t *) (Bank1_LCD_D)= (y)>>8;			//������ʼY������ֽ�

  	 *(__IO uint16_t *) (Bank1_LCD_C)= 7;	
  	 *(__IO uint16_t *) (Bank1_LCD_D)= (u8)(y);			//������ʼX������ֽ�
  
  	 *(__IO uint16_t *) (Bank1_LCD_C)= 0x22;			//GRAMд����

  	 *(__IO uint16_t *) (Bank1_LCD_D)= point;			//д��һ������		 
   
}



/****************************************************************************
* ��    �ƣ�void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����lcd_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void lcd_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          lcd_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
        }
        else
        {
          lcd_SetPoint(x+j,y+i,bkColor); // ������ɫ
        }
    }
  }
}


/****************************************************************************
* ��    �ƣ�u16 lcd_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 lcd_BGR2RGB(u16 c)
{
  u16  r, g, b;
  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  return( (b<<11) + (g<<5) + (r<<0) );
}
