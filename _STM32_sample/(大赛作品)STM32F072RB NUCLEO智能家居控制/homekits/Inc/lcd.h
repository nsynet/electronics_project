#ifndef __LCD_H
#define __LCD_H

#include "stm32f0xx_hal.h"

#define LCD_RESET_CLEAR HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define LCD_RESET_SET HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)

#define LCD_CS_CLEAR HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
#define LCD_CS_SET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)

#define LCD_DC_CLEAR HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)
#define LCD_DC_SET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0

#define FOREGROUND	BLACK
#define BACKGROUND	WHITE

void LCD_Write_Cmd(uint8_t cmd);
void LCD_Write_Dat(uint8_t dat);
void LCD_Write_Color(uint16_t color);
void LCD_Init(void);
void LCD_Set_Cursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Set_Dir(uint8_t dir);
void LCD_GRAM_Prepare(void);
void LCD_Clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void LCD_Set_Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void LCD_Draw_Char_En(uint16_t x, uint16_t y, char ch, uint16_t color);
void LCD_Draw_String_En(uint16_t x, uint16_t y, char *str, uint16_t color);

void LCD_Draw_Char_Ch(uint16_t x, uint16_t y, char *code, uint16_t color);
void LCD_Draw_String_Ch(uint16_t x, uint16_t y, char *str, uint16_t color);
void LCD_Draw_String(uint16_t x, uint16_t y, char *str, uint16_t color);

void LCD_Draw_Image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *pData);

	
void LCD_DMA_Test(void);
void SPI_Set_Data_Size(uint8_t bits_width);
void SPI_Set_Data_Inc(uint16_t inc);


#endif

