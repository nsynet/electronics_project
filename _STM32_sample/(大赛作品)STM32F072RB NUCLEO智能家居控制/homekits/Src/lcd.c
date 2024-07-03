#include "lcd.h"
#include "ascii.h"
#include "ch.h"
#include "string.h"

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;


typedef struct
{
	uint16_t id;
	uint16_t width;
	uint16_t height;
	uint8_t setxcmd;
	uint8_t setycmd;
	uint8_t wramcmd;
	uint16_t currentx;
	uint16_t currenty;
	
} LCD_TypeDef;

LCD_TypeDef lcd;

void LCD_Write_Cmd(uint8_t cmd)
{
	LCD_CS_CLEAR;
	LCD_DC_CLEAR;
	
	
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 50);
	
	LCD_CS_SET;
	
}


void LCD_Write_Dat(uint8_t dat)
{
	LCD_CS_CLEAR;
	LCD_DC_SET;
	
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	
	LCD_CS_SET;
		
	
}

void LCD_Write_Color(uint16_t color)
{
	uint8_t dat;
	
	LCD_CS_CLEAR;
	LCD_DC_SET;
	
	dat = color >> 8;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	
	dat = color & 0xFF;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 50);
	
	LCD_CS_SET;
		
	
}

void LCD_Set_Dir(uint8_t dir)
{
	LCD_Write_Cmd(0x36);
	
	if(dir == 0)
	{
		LCD_Write_Dat(0x48);
		lcd.width = 240;
		lcd.height = 320;
		
	}
	else
	{
		LCD_Write_Dat(0xE8);
		lcd.width = 320;
		lcd.height = 240;
	}
	
	
}


void LCD_Init(void)
{
	lcd.id = 0x9341;
	lcd.width = 240;
	lcd.height = 320;
	lcd.setxcmd = 0x2A;
	lcd.setycmd = 0x2B;
	lcd.wramcmd = 0x2C;
	lcd.currentx = 0;
	lcd.currentx = 0;


	LCD_RESET_CLEAR;
 	HAL_Delay(50);

	LCD_RESET_SET;
	HAL_Delay(50);


	LCD_Write_Cmd(0xCB);  
	LCD_Write_Dat(0x39); 
	LCD_Write_Dat(0x2C); 
	LCD_Write_Dat(0x00); 
	LCD_Write_Dat(0x34); 
	LCD_Write_Dat(0x02); 

	LCD_Write_Cmd(0xCF);  
	LCD_Write_Dat(0x00); 
	LCD_Write_Dat(0XC1); 
	LCD_Write_Dat(0X30); 

	LCD_Write_Cmd(0xE8);  
	LCD_Write_Dat(0x85); 
	LCD_Write_Dat(0x00); 
	LCD_Write_Dat(0x78); 

	LCD_Write_Cmd(0xEA);  
	LCD_Write_Dat(0x00); 
	LCD_Write_Dat(0x00); 

	LCD_Write_Cmd(0xED);  
	LCD_Write_Dat(0x64); 
	LCD_Write_Dat(0x03); 
	LCD_Write_Dat(0X12); 
	LCD_Write_Dat(0X81); 

	LCD_Write_Cmd(0xF7);  
	LCD_Write_Dat(0x20); 

	LCD_Write_Cmd(0xC0);    //Power control 
	LCD_Write_Dat(0x23);   //VRH[5:0] 

	LCD_Write_Cmd(0xC1);    //Power control 
	LCD_Write_Dat(0x10);   //SAP[2:0];BT[3:0] 

	LCD_Write_Cmd(0xC5);    //VCM control 
	LCD_Write_Dat(0x3e); //对比度调节
	LCD_Write_Dat(0x28); 

	LCD_Write_Cmd(0xC7);    //VCM control2 
	LCD_Write_Dat(0x86);  //--

	LCD_Write_Cmd(0x36);    // Memory Access Control 
	LCD_Write_Dat(0x48); //C8	

	LCD_Write_Cmd(0x3A);    
	LCD_Write_Dat(0x55); 

	LCD_Write_Cmd(0xB1);    
	LCD_Write_Dat(0x00);  
	LCD_Write_Dat(0x18); 

	LCD_Write_Cmd(0xB6);    // Display Function Control 
	LCD_Write_Dat(0x08); 
	LCD_Write_Dat(0x82);
	LCD_Write_Dat(0x27);  

	LCD_Write_Cmd(0xF2);    // 3Gamma Function Disable 
	LCD_Write_Dat(0x00); 

	LCD_Write_Cmd(0x26);    //Gamma curve selected 
	LCD_Write_Dat(0x01); 

	LCD_Write_Cmd(0xE0);    //Set Gamma 
	LCD_Write_Dat(0x0F); 
	LCD_Write_Dat(0x31); 
	LCD_Write_Dat(0x2B); 
	LCD_Write_Dat(0x0C); 
	LCD_Write_Dat(0x0E); 
	LCD_Write_Dat(0x08); 
	LCD_Write_Dat(0x4E); 
	LCD_Write_Dat(0xF1); 
	LCD_Write_Dat(0x37); 
	LCD_Write_Dat(0x07); 
	LCD_Write_Dat(0x10); 
	LCD_Write_Dat(0x03); 
	LCD_Write_Dat(0x0E); 
	LCD_Write_Dat(0x09); 
	LCD_Write_Dat(0x00); 

	LCD_Write_Cmd(0XE1);    //Set Gamma 
	LCD_Write_Dat(0x00); 
	LCD_Write_Dat(0x0E); 
	LCD_Write_Dat(0x14); 
	LCD_Write_Dat(0x03); 
	LCD_Write_Dat(0x11); 
	LCD_Write_Dat(0x07); 
	LCD_Write_Dat(0x31); 
	LCD_Write_Dat(0xC1); 
	LCD_Write_Dat(0x48); 
	LCD_Write_Dat(0x08); 
	LCD_Write_Dat(0x0F); 
	LCD_Write_Dat(0x0C); 
	LCD_Write_Dat(0x31); 
	LCD_Write_Dat(0x36); 
	LCD_Write_Dat(0x0F); 

	LCD_Write_Cmd(0x11);    //Exit Sleep 
	//HAL_Delay(120);
	
	LCD_Write_Cmd(0x29);    //Display on 
	
	LCD_Set_Dir(1);

	//LCD_Set_Window(0, 0, lcd.width, lcd.height);
	
	
	LCD_Clear(0, 0, 320, 80, WHITE);
	
	LCD_Clear(0, 80, 320, 80, WHITE);
	
	LCD_Clear(0, 160, 320, 80, WHITE);

	
}



void LCD_Set_Cursor(uint16_t x, uint16_t y)
{
  LCD_Write_Cmd(lcd.setxcmd); 
	LCD_Write_Dat(x >> 8); 
	LCD_Write_Dat(x & 0xFF); 
//	LCD_Write_Dat(lcd.width & 0xFF); 
//	LCD_Write_Dat(x & 0xFF);	 
	LCD_Write_Cmd(lcd.setycmd); 
	LCD_Write_Dat(y >> 8); 
	LCD_Write_Dat(y & 0xFF);
//	LCD_Write_Dat(lcd.height >> 8); 
//	LCD_Write_Dat(lcd.height & 0xFF); 
} 	

void LCD_GRAM_Prepare(void)
{
	LCD_Write_Cmd(0x2C);
	
}


void LCD_Clear(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	uint32_t points = width * height ;
//	uint16_t c;
//	HAL_SPI_StateTypeDef s;
	
//	c = (color << 8) + (color >> 8);
	
	LCD_Set_Window(x, y, width, height);
	
	LCD_GRAM_Prepare();
	
//	for(index = 0; index < points; index ++)
//	{
//		LCD_Write_Color(color);
//		
//	}
	
	
	LCD_CS_CLEAR;
	LCD_DC_SET;
	
//	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)(&c), 320 * 2);
	
	SPI_Set_Data_Size(16);
	

	
	while ( HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
	{
  } 
	
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)(&color), points);
	

  while ((HAL_SPI_GetState(&hspi1)) != HAL_SPI_STATE_READY)
  {
  } 
	
	
	
	SPI_Set_Data_Size(8);
	LCD_CS_SET;	
	
	//SPI_Set_Data_Size(8);
	
}

void LCD_Draw_Image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *pData)
{
//	HAL_SPI_StateTypeDef s;
	
	LCD_Set_Window(x, y, w, h);
	LCD_GRAM_Prepare();
	
	LCD_CS_CLEAR;
	LCD_DC_SET;
	
	SPI_Set_Data_Size(16);
	SPI_Set_Data_Inc(1);
	while ( HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
	{
	} 
	
	HAL_SPI_Transmit_DMA(&hspi1, pData, w * h);
	

  while ((HAL_SPI_GetState(&hspi1)) != HAL_SPI_STATE_READY)
  {
  } 
	
	SPI_Set_Data_Inc(0);
	
	SPI_Set_Data_Size(8);
	
	
	LCD_CS_SET;	
	
	
}


void LCD_Set_Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	
	LCD_Write_Cmd(lcd.setxcmd); 
	LCD_Write_Dat(x >> 8);
	LCD_Write_Dat(x & 0xFF); 
	LCD_Write_Dat((x + width - 1) >> 8); 
	LCD_Write_Dat((x + width - 1) & 0xFF);	 
	LCD_Write_Cmd(lcd.setycmd); 
	LCD_Write_Dat(y >> 8); 
	LCD_Write_Dat(y & 0xFF);
	LCD_Write_Dat((y + height - 1) >> 8); 
	LCD_Write_Dat((y + height - 1) & 0xFF); 
	lcd.currentx = x;
	lcd.currentx = y;
		
	
	
}



void LCD_Draw_Char_En(uint16_t x, uint16_t y, char ch, uint16_t color)
{
	volatile uint16_t row, mask, index;
	//volatile uint8_t *matrix;
	//extern char ascii_tab[];
	
	index = 16 * (ch - ' ');
	
	if(x + 8 > lcd.width)
	{
		x = 10; 
		y += 20;
	}
	
	LCD_Set_Window(x, y, 8, 16);
	
	LCD_GRAM_Prepare();
	
	for(row = 0; row < 16; row ++)
	{
		for(mask = 0x80; mask != 0; mask >>= 1)
		{
			
			if( ascii_tab[index] & mask)
				LCD_Write_Color(color);
			else
				LCD_Write_Color(BACKGROUND);
			
		}
		
		index ++;
		
	}
	
	
}

void LCD_Draw_String_En(uint16_t x, uint16_t y, char *str, uint16_t color)
{
	uint16_t index; 
	
	for(index = 0; index < strlen(str); index ++)
	{
		LCD_Draw_Char_En(x + index * 8, y, str[index], color);
		
	}
	
	
	
}


void LCD_Draw_Char_Ch(uint16_t x, uint16_t y, char *code, uint16_t color)
{
	volatile uint16_t row, mask, index, count;
	//volatile uint8_t *matrix;
	//extern char ascii_tab[];
	
	uint16_t len = sizeof(GB_16) / sizeof(typFNT_GB16);
	
	for(index = 0; index < len; )
	{
		if(*code == GB_16[index].Index[0] && *(code + 1) == GB_16[index].Index[1])
			break;
		
		index ++;
		
	}
	
	//index ++;
	
	
	//index = 32 * (code);
	if(x + 16 > lcd.width)
	{
		x = 10; 
		y += 20;
	}
	
	LCD_Set_Window(x, y, 16, 16);
	
	LCD_GRAM_Prepare();
	count = 0;
	
	for(row = 0; row < 16; row ++)
	{
		for(mask = 0x80; mask != 0; mask >>= 1)
		{
			
			if( GB_16[index].Msk[count] & mask)
				LCD_Write_Color(color);
			else
				LCD_Write_Color(BACKGROUND);
			
		}
		
		count ++;
		
		for(mask = 0x80; mask != 0; mask >>= 1)
		{
			
			if( GB_16[index].Msk[count] & mask)
				LCD_Write_Color(color);
			else
				LCD_Write_Color(BACKGROUND);
			
		}
		
		count ++;
	}
		
	
}
void LCD_Draw_String_Ch(uint16_t x, uint16_t y, char *str, uint16_t color)
{
	
	uint16_t index; 
	
	for(index = 0; index < strlen(str); index += 2)
	{
		LCD_Draw_Char_Ch(x + index * 8, y, &str[index], color);
		
	}
	
		
}

void LCD_Draw_String(uint16_t x, uint16_t y, char *str, uint16_t color)
{
	
	uint16_t index; 
	
	for(index = 0; index < strlen(str);)
	{
		if(!(str[index] & 0x80))
		{
			LCD_Draw_Char_En(x + index * 8, y, str[index], color);
			index ++;
		}
		else
		{
			LCD_Draw_Char_Ch(x + index * 8, y, &str[index], color);
			index += 2;
			
		}
	
		
		
	}
	
		
}



void LCD_DMA_Test(void)
{
	
	
	uint16_t color = BLUE;
	
	LCD_Set_Window(20, 30, lcd.width, lcd.height);
	
	LCD_GRAM_Prepare();
	
	LCD_CS_CLEAR;
	LCD_DC_SET;
	
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)(&color), 200);
	
	while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
	{
		
	}
	
	
	
	LCD_CS_SET;
	
}

void SPI_Set_Data_Size(uint8_t bits_width)
{
//	__HAL_SPI_DISABLE(&hspi1);
//	
//	if(bits_width == 8)
//	{
//		SPI1->CR2 &= 0xF0FF;
//		SPI1->CR2 |= 0x0700;
//	}
//	else
//	{
//		SPI1->CR2 &= 0xF0FF;
//		SPI1->CR2 |= 0x0F00;
//		
//	}
//	
//	__HAL_SPI_ENABLE(&hspi1);
	
	if(bits_width == 8)
	{
		hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
		//hspi1.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
	}
	else
	{
		hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
		//hspi1.Init.CRCLength = SPI_CRC_LENGTH_16BIT;
	}
	
	HAL_SPI_Init(&hspi1);
	
}

void SPI_Set_Data_Inc(uint16_t inc)
{
	
	__HAL_DMA_DISABLE(&hdma_spi1_tx);
	
	//hdma_spi1_tx.Instance = DMA1_Channel3;
	
	if(inc != 0)
	{
		//hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
		DMA1_Channel3->CCR |= DMA_CCR_MINC;
		
	}
	else
	{
		//hdma_spi1_tx.Init.MemInc = DMA_MINC_DISABLE;
		DMA1_Channel3->CCR &= ~DMA_CCR_MINC;
		//DMA1_Channel3->CCR |= 0x80;
	}
	
	//HAL_DMA_Init(&hdma_spi1_tx);
	
	
	
	//__HAL_LINKDMA(&hspi1,hdmatx,hdma_spi1_tx);
	
}




