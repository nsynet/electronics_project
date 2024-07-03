/**
  * @file    stm32f429i_lcd.c 
  * @author  WB R&D Team - openmcu666
  * @version V0.1
  * @date    2016.08.16
  * @brief   RGB LCD Driver(7��)
  */
#include "stm32f429i_lcd.h"
#include "fonts.c"
#include "QR_Decoder.h"
uint8_t CHAR_H = 16;                                                   //�ַ��߶�                     
uint8_t CHAR_W = 8;                                                    //�ַ����  


sFONT *LCD_Currentfonts;

static uint16_t CurrentTextColor   = BLUE;
static uint16_t CurrentBackColor   = 0xFFFF;

static uint32_t CurrentTextColor_ARGB8888   = 0x00000000;

static uint32_t CurrentFrameBuffer = LCD_FRAME_BUFFER;
static uint32_t CurrentLayer = LCD_BACKGROUND_LAYER;
int pos=(LCD_PIXEL_HEIGHT-Frame_Width)/2+5*Frame_Line_Size;
#ifdef USE_Delay
static void delay(__IO uint32_t nCount);
#endif 

void PutPixel(int16_t x, int16_t y);
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);
void LCD_AF_GPIOConfig(void);

/**
  * @brief  DeInitializes the LCD.
  * @param  None
  * @retval None
  */
void LCD_DeInit(void)
{ 
   GPIO_InitTypeDef GPIO_InitStructure;
  /*
   ****************************************************************************
   *R3 --> PB0  | G2 --> PH13 | B3 --> PG11 | DE    --> PF10
   *R4 --> PA11 | G3 --> PG10 | B4 --> PG12 | BL_EN --> PI11
   *R5 --> PA12 | G4 --> PH15 | B5 -->  PA3 | VSYNC --> PI10
   *R6 --> PB1  | G5 -->  PI0 | B6 -->  PI6 | HSYNC -->  PI9
   *R7 --> PG6  | G6 -->  PI1 | B7 -->  PI7 | CLK   -->  PG7
   *            | G7 -->  PI2 |
   ****************************************************************************
   */
	/* ʹ��GPIOʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG 
                         | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOA , ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  /* PB1����ΪLCD_R6 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, 0x09);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, 0x09);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_13
                                |  GPIO_Pin_15;
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 
                                | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */ 
void LCD_Init(void)
{
 LTDC_InitTypeDef       LTDC_InitStruct;

 /* Enable the LTDC Clock */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

 /* Enable the DMA2D Clock */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

 /* Configure the LCD Control pins */
 LCD_AF_GPIOConfig();

 /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
 SDRAM_Init();

 /* LTDC Configuration *********************************************************/
 /* Polarity configuration */
 /* Initialize the horizontal synchronization polarity as active low */
 LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
 /* Initialize the vertical synchronization polarity as active low */
 LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
 /* Initialize the data enable polarity as active low */
 LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
 /* Initialize the pixel clock polarity as input pixel clock */
 LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

 /* Configure R,G,B component values for LCD background color */
 LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
 LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
 LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;

  RCC_PLLSAIConfig(416, 7, 6);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);

 /* Enable PLLSAI Clock */
 RCC_PLLSAICmd(ENABLE);
 /* Wait for PLLSAI activation */
 while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
 {
 } 
      // /* Timing configuration */
	/* Configure horizontal synchronization width */     
	LTDC_InitStruct.LTDC_HorizontalSync = 20;
	/* Configure vertical synchronization height */
	LTDC_InitStruct.LTDC_VerticalSync = 7;
	/* Configure accumulated horizontal back porch */
	LTDC_InitStruct.LTDC_AccumulatedHBP = 42; 
	/* Configure accumulated vertical back porch */
	LTDC_InitStruct.LTDC_AccumulatedVBP = 22;  
	/* Configure accumulated active width */
	LTDC_InitStruct.LTDC_AccumulatedActiveW = 862;
	/* Configure accumulated active height */
	LTDC_InitStruct.LTDC_AccumulatedActiveH = 525;
	/* Configure total width */
	LTDC_InitStruct.LTDC_TotalWidth = 1056; 
	/* Configure total height */
	LTDC_InitStruct.LTDC_TotalHeigh = 540;   
 
 LTDC_Init(&LTDC_InitStruct);
}

/**
  * @brief  Initializes the LCD Layers.
  * @param  None
  * @retval None
  */
void LCD_LayerInit(void)
{
 LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

 /* Windowing configuration */
 /* In this case all the active display area is used to display a picture then :
 Horizontal start = horizontal synchronization + Horizontal back porch = 30
 Horizontal stop = Horizontal start + window width -1 = 30 + 240 -1
 Vertical start   = vertical synchronization + vertical back porch     = 4
 Vertical stop   = Vertical start + window height -1  = 4 + 320 -1      */
	
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 43;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 43 - 1); 
  LTDC_Layer_InitStruct.LTDC_VerticalStart = 22;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 22 - 1);

 /* Pixel Format configuration*/
 LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
 /* Alpha constant (255 totally opaque) */
 LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;
 /* Default Color configuration (configure A,R,G,B component values) */
 LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
 LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
 LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;
 LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
 /* Configure blending factors */
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

 /* the length of one line of pixels in bytes + 3 then :
 Line Lenth = Active high width x number of bytes per pixel + 3
 Active high width         = LCD_PIXEL_WIDTH
 number of bytes per pixel = 2    (pixel_format : RGB565)
 */
 LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3);
 /* the pitch is the increment from the start of one line of pixels to the
 start of the next line in bytes, then :
 Pitch = Active high width x number of bytes per pixel */
 LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);

 /* Configure the number of lines */
 LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;

 /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
 LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;

 /* Initialize LTDC layer 1 */
 LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  /* Configure Layer2 */
 /* Pixel Format configuration*/
 LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB8888;
 
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 4) + 3);
 /* the pitch is the increment from the start of one line of pixels to the
 start of the next line in bytes, then :
 Pitch = Active high width x number of bytes per pixel */
 LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 4);
 
  /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */
 LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + BUFFER_OFFSET;

  /* Configure blending factors */
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

  /* Initialize LTDC layer 2 */
 LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

  /* LTDC configuration reload */
 LTDC_ReloadConfig(LTDC_IMReload);

  /* Enable foreground & background Layers */
 LTDC_LayerCmd(LTDC_Layer1, ENABLE);
 LTDC_LayerCmd(LTDC_Layer2, ENABLE);

  /* LTDC configuration reload */
 LTDC_ReloadConfig(LTDC_IMReload);

  /* Set default font */
 LCD_SetFont(&LCD_DEFAULT_FONT);

  /* dithering activation */
 LTDC_DitherCmd(ENABLE);
}

/**
  * @brief  Sets the LCD Layer.
  * @param  Layerx: specifies the Layer foreground or background.
  * @retval None
  */
void LCD_SetLayer(uint32_t Layerx)
{
  if (Layerx == LCD_BACKGROUND_LAYER)
  {
    CurrentFrameBuffer = LCD_FRAME_BUFFER; 
    CurrentLayer = LCD_BACKGROUND_LAYER;
  }
  else
  {
    CurrentFrameBuffer = LCD_FRAME_BUFFER + BUFFER_OFFSET;
    CurrentLayer = LCD_FOREGROUND_LAYER;
  }
}  

/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  TextColor: specifies the Text Color.
  * @param  BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor)
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  TextColor: pointer to the variable that will contain the Text 
            Color.
  * @param  BackColor: pointer to the variable that will contain the Background 
            Color.
  * @retval None
  */
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
  *TextColor = CurrentTextColor;
  *BackColor = CurrentBackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color)
{
  CurrentBackColor = Color;
}

/**
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  Configure the transparency.
  * @param  transparency: specifies the transparency, 
  *         This parameter must range from 0x00 to 0xFF.
  * @retval None
  */
void LCD_SetTransparency(uint8_t transparency)
{
  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {
    LTDC_LayerAlpha(LTDC_Layer1, transparency);
  }
  else
  {     
    LTDC_LayerAlpha(LTDC_Layer2, transparency);
  }
  LTDC_ReloadConfig(LTDC_IMReload);
}

/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg LCD_LINE_x: where x can be: 0..13 if LCD_Currentfonts is Font16x24
  *                                      0..26 if LCD_Currentfonts is Font12x12 or Font8x12
  *                                      0..39 if LCD_Currentfonts is Font8x8
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  uint16_t refcolumn = 0;
  /* Send the string character by character on lCD */
  while ((refcolumn < LCD_PIXEL_WIDTH) && (((refcolumn + LCD_Currentfonts->Width)& 0xFFFF) >= LCD_Currentfonts->Width))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, ' ');
    /* Decrement the column position by 16 */
    refcolumn += LCD_Currentfonts->Width;
  }
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{
  uint32_t index = 0;
  
  /* erase memory */
  for (index = 0x00; index <BUFFER_OFFSET; index++)
  {
    *(__IO uint16_t*)(CurrentFrameBuffer + (2*index)) = Color;

  } 
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval Display Address
  */
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{  
  return CurrentFrameBuffer + 2*(Xpos + (LCD_PIXEL_WIDTH*Ypos));
}

/**
  * @brief  Config and Sets the color Keying.
  * @param  RGBValue: Specifies the Color reference. 
  * @retval None
  */
void LCD_SetColorKeying(uint32_t RGBValue)
{  
  LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;
  
  /* configure the color Keying */
  LTDC_colorkeying_InitStruct.LTDC_ColorKeyBlue = 0x0000FF & RGBValue;
  LTDC_colorkeying_InitStruct.LTDC_ColorKeyGreen = (0x00FF00 & RGBValue) >> 8;
  LTDC_colorkeying_InitStruct.LTDC_ColorKeyRed = (0xFF0000 & RGBValue) >> 16;  

  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {   
    /* Enable the color Keying for Layer1 */
    LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, ENABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
  else
  {
    /* Enable the color Keying for Layer2 */
    LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, ENABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
}

/**
  * @brief  Disable the color Keying.
  * @param  RGBValue: Specifies the Color reference. 
  * @retval None
  */
void LCD_ReSetColorKeying(void)
{
  LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;
  
  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {   
    /* Disable the color Keying for Layer1 */
    LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, DISABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
  else
  {
    /* Disable the color Keying for Layer2 */
    LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, DISABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
} 

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, counter = 0, xpos =0;
  uint32_t  Xaddress = 0;
  
  xpos = Xpos*LCD_PIXEL_WIDTH*2;
  Xaddress += Ypos;
  
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    
    for(counter = 0; counter < LCD_Currentfonts->Width; counter++)
    {
          
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> counter)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
        (((c[index] & (0x1 << counter)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
      {
          /* Write data value to all SDRAM memory */
         *(__IO uint16_t*) (CurrentFrameBuffer + (2*Xaddress) + xpos) = CurrentBackColor;
      }
      else
      {
          /* Write data value to all SDRAM memory */
         *(__IO uint16_t*) (CurrentFrameBuffer + (2*Xaddress) + xpos) = CurrentTextColor;         
      }
      Xaddress++;
    }
      Xaddress += (LCD_PIXEL_WIDTH - LCD_Currentfonts->Width);
  }
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..29
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;

  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{  
  uint16_t refcolumn = 0;
  /* Send the string character by character on lCD */
  while ((refcolumn < LCD_PIXEL_WIDTH) && ((*ptr != 0) & (((refcolumn + LCD_Currentfonts->Width) & 0xFFFF) >= LCD_Currentfonts->Width)))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by width */
    refcolumn += LCD_Currentfonts->Width;
    /* Point on the next character */
    ptr++;
  }
}

/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X bottom left position from 0 to 240.
  * @param  Ypos: specifies the Y bottom left position from 0 to 320.
  * @param  Height: display window height, can be a value from 0 to 320.
  * @param  Width: display window width, can be a value from 0 to 240.
  * @retval None
  */
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{

  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  { 
    /* reconfigure the layer1 position */
    LTDC_LayerPosition(LTDC_Layer1, Xpos, Ypos);
    LTDC_ReloadConfig(LTDC_IMReload);
    
    /* reconfigure the layer1 size */
    LTDC_LayerSize(LTDC_Layer1, Width, Height);
    LTDC_ReloadConfig(LTDC_IMReload);
 }
 else
 {   
    /* reconfigure the layer2 position */
    LTDC_LayerPosition(LTDC_Layer2, Xpos, Ypos);
    LTDC_ReloadConfig(LTDC_IMReload); 
   
   /* reconfigure the layer2 size */
    LTDC_LayerSize(LTDC_Layer2, Width, Height);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
}

/**
  * @brief  Disables LCD Window mode.
  * @param  None
  * @retval None
  */
void LCD_WindowModeDisable(void)
{
  LCD_SetDisplayWindow(0, 0, LCD_PIXEL_HEIGHT, LCD_PIXEL_WIDTH); 
}

/**
  * @brief  Displays a line.
  * @param Xpos: specifies the X position, can be a value from 0 to 240.
  * @param Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param Length: line length.
  * @param Direction: line direction.
  *   This parameter can be one of the following values: LCD_DIR_HORIZONTAL or LCD_DIR_VERTICAL.
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  
  uint32_t  Xaddress = 0;
  uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
  
  Xaddress = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);
 
  Red_Value = (0xF800 & CurrentTextColor) >> 11;
  Blue_Value = 0x001F & CurrentTextColor;
  Green_Value = (0x07E0 & CurrentTextColor) >> 5;

  /* Configure DMA2D */    
  DMA2D_DeInit();  
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                  
  
  if(Direction == LCD_DIR_HORIZONTAL)
  {                                                      
    DMA2D_InitStruct.DMA2D_OutputOffset = 0;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = 1;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = Length; 
  }
  else
  {                                                            
    DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - 1;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = Length;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = 1;  
  }
  
  DMA2D_Init(&DMA2D_InitStruct);  
  /* Start Transfer */ 
  DMA2D_StartTransfer();  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  }
  
}
/**
  * @brief  Displays a color line.
  * @param Xpos: specifies the X position, can be a value from 0 to 800.
  * @param Ypos: specifies the Y position, can be a value from 0 to 480.
  * @param Length: line length.
  * @param Direction: line direction.
  *   This parameter can be one of the following values: LCD_DIR_HORIZONTAL or LCD_DIR_VERTICAL.
  * @param Color
  * @retval None
  */
void LCD_DrawLineColor(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction,uint16_t Color)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  
  uint32_t  Xaddress = 0;
  uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
  
  Xaddress = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);
 
  Red_Value = (0xF800 & Color) >> 11;
  Blue_Value = 0x001F & Color;
  Green_Value = (0x07E0 & Color) >> 5;

  /* Configure DMA2D */    
  DMA2D_DeInit();  
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                  
  
  if(Direction == LCD_DIR_HORIZONTAL)
  {                                                      
    DMA2D_InitStruct.DMA2D_OutputOffset = 0;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = 1;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = Length; 
  }
  else
  {                                                            
    DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - 1;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = Length;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = 1;  
  }
  
  DMA2D_Init(&DMA2D_InitStruct);  
  /* Start Transfer */ 
  DMA2D_StartTransfer();  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  }
  
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 800.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 480.
  * @param  Height: display rectangle height, can be a value from 0 to 480.
  * @param  Width: display rectangle width, can be a value from 0 to 800.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
  /* draw horizontal lines */
  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos, (Ypos+ Height), Width, LCD_DIR_HORIZONTAL);
  
  /* draw vertical lines */
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine((Xpos + Width), Ypos, Height, LCD_DIR_VERTICAL);
}

/**
  * @brief  Draw a circle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 800.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 480.
  * @param  Radius: radius of the circle.
  * @retval None
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    do {
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor; 
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
      
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
}

/**
  * @brief  Draw a full ellipse.
  * @param  Xpos: specifies the X position, can be a value from 0 to 800.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 480
.
  * @param  Radius: minor radius of ellipse.
  * @param  Radius2: major radius of ellipse.  
  * @retval None
  */
void LCD_DrawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
  int x = -Radius, y = 0, err = 2-2*Radius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = Radius;
  rad2 = Radius2;
  
  if (Radius > Radius2)
  { 
    do 
    {
      K = (float)(rad1/rad2);
      LCD_DrawLine((Xpos+x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);
      LCD_DrawLine((Xpos-x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);
      
      e2 = err;
      if (e2 <= y) 
      {
        err += ++y*2+1;
        if (-x == y && e2 <= x) e2 = 0;
      }
      if (e2 > x) err += ++x*2+1;
      
    }
    while (x <= 0);
  }
  else
  {
    y = -Radius2; 
    x = 0;
    do 
    { 
      K = (float)(rad2/rad1);       
      LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);
      LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);
      
      e2 = err;
      if (e2 <= x) 
      {
        err += ++x*2+1;
        if (-y == x && e2 <= y) e2 = 0;
      }
      if (e2 > y) err += ++y*2+1;
    }
    while (y <= 0);
  }
}

/**
  * @brief  Displays an Ellipse.
  * @param  Xpos: specifies the X position, can be a value from 0 to 800.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 480.
  * @param  Radius: specifies Radius.
  * @param  Radius2: specifies Radius2.
  * @retval None
  */
void LCD_DrawEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
  int x = -Radius, y = 0, err = 2-2*Radius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
   
  rad1 = Radius;
  rad2 = Radius2;
  
  if (Radius > Radius2)
  { 
    do {
      K = (float)(rad1/rad2);
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = CurrentTextColor; 
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = CurrentTextColor;
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = CurrentTextColor;
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = CurrentTextColor;
            
      e2 = err;
      if (e2 <= y) {
        err += ++y*2+1;
        if (-x == y && e2 <= x) e2 = 0;
      }
      if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
  }
  else
  {
    y = -Radius2; 
    x = 0;
    do { 
      K = (float)(rad2/rad1);
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor; 
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
      *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
      
      e2 = err;
      if (e2 <= x) {
        err += ++x*2+1;
        if (-y == x && e2 <= y) e2 = 0;
      }
      if (e2 > y) err += ++y*2+1;     
    }
    while (y <= 0);
  }
}

/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void LCD_DrawMonoPict(const uint32_t *Pict)
{
  uint32_t index = 0, counter = 0;
  
   
  for(index = 0; index < 2400; index++)
  {
    for(counter = 0; counter < 32; counter++)
    {
      if((Pict[index] & (1 << counter)) == 0x00)
      {
        *(__IO uint16_t*)(CurrentFrameBuffer) = CurrentBackColor;
      }
      else
      {
        *(__IO uint16_t*)(CurrentFrameBuffer) = CurrentTextColor;
      }
    }
  }
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void LCD_WriteBMP(uint32_t BmpAddress)
{
  uint32_t index = 0, size = 0, width = 0, height = 0, bit_pixel = 0;
  uint32_t Address;
  uint32_t currentline = 0, linenumber = 0;
 
  Address = CurrentFrameBuffer;

  /* Read bitmap size */
  size = *(__IO uint16_t *) (BmpAddress + 2);
  size |= (*(__IO uint16_t *) (BmpAddress + 4)) << 16;
  
  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (BmpAddress + 10);
  index |= (*(__IO uint16_t *) (BmpAddress + 12)) << 16;

  /* Read bitmap width */
  width = *(uint16_t *) (BmpAddress + 18);
  width |= (*(uint16_t *) (BmpAddress + 20)) << 16;

  /* Read bitmap height */
  height = *(uint16_t *) (BmpAddress + 22);
  height |= (*(uint16_t *) (BmpAddress + 24)) << 16;

  /* Read bit/pixel */
  bit_pixel = *(uint16_t *) (BmpAddress + 28);  
 
  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {
    /* reconfigure layer size in accordance with the picture */
    LTDC_LayerSize(LTDC_Layer1, width, height);
    LTDC_ReloadConfig(LTDC_VBReload);

    /* Reconfigure the Layer pixel format in accordance with the picture */    
    if ((bit_pixel/8) == 4)
    {
      LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_ARGB8888);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else if ((bit_pixel/8) == 2)
    {
      LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB565);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else 
    {
      LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB888);
      LTDC_ReloadConfig(LTDC_VBReload);
    } 
  }
  else
  {
    /* reconfigure layer size in accordance with the picture */
    LTDC_LayerSize(LTDC_Layer2, width, height);
    LTDC_ReloadConfig(LTDC_VBReload); 
    
    /* Reconfigure the Layer pixel format in accordance with the picture */
    if ((bit_pixel/8) == 4)
    {
      LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_ARGB8888);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else if ((bit_pixel/8) == 2)
    {
      LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB565);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else
    {
      LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB888);
      LTDC_ReloadConfig(LTDC_VBReload);  
    }
  }
 
  /* compute the real size of the picture (without the header)) */  
  size = (size - index); 

  /* bypass the bitmap header */
  BmpAddress += index;

  /* start copie image from the bottom */
  Address += width*(height-1)*(bit_pixel/8);
  
  for(index = 0; index < size; index++)
  {
    *(__IO uint8_t*) (Address) = *(__IO uint8_t *)BmpAddress;
    
    /*jump on next byte */   
    BmpAddress++;
    Address++;
    currentline++;
    
    if((currentline/(bit_pixel/8)) == width)
    {
      if(linenumber < height)
      {
        linenumber++;
        Address -=(2*width*(bit_pixel/8));
        currentline = 0;
      }
    }
  }
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 800.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 480.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  
  uint32_t  Xaddress = 0; 
  uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
 
  Red_Value = (0xF800 & CurrentTextColor) >> 11;
  Blue_Value = 0x001F & CurrentTextColor;
  Green_Value = (0x07E0 & CurrentTextColor) >> 5;
  
  Xaddress = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);
  
  /* configure DMA2D */
  DMA2D_DeInit();
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                
  DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - Width);                
  DMA2D_InitStruct.DMA2D_NumberOfLine = Height;            
  DMA2D_InitStruct.DMA2D_PixelPerLine = Width;
  DMA2D_Init(&DMA2D_InitStruct); 
  
  /* Start Transfer */ 
  DMA2D_StartTransfer();
  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  } 

  LCD_SetTextColor(CurrentTextColor);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 800.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 480.
  * @param  Radius
  * @retval None
  */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    if(CurY > 0) 
    {
      LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
    }
    
    if(CurX > 0) 
    {
      LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
  
  LCD_DrawCircle(Xpos, Ypos, Radius);  
}

/**
  * @brief  ���32λ��ɫ
  * @param  Color:������ɫ
  * @retval None
  */
void LCD_Clear_ARGB8888(uint32_t Color)
{	
  DMA2D_InitTypeDef      DMA2D_InitStruct;
 
  uint16_t Alpha_Value=0,Red_Value = 0, Green_Value = 0, Blue_Value = 0;

  Alpha_Value = (0xFF000000&Color)>>24;
  Red_Value = (0x00FF0000 & Color) >> 16;
  Blue_Value = 0x000000FF & Color;
  Green_Value = (0x0000FF00 & Color) >> 8;

  DMA2D_DeInit();
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_ARGB8888;
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
  DMA2D_InitStruct.DMA2D_OutputAlpha = Alpha_Value;	
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = CurrentFrameBuffer;
  DMA2D_InitStruct.DMA2D_OutputOffset = 0;
  DMA2D_InitStruct.DMA2D_NumberOfLine = LCD_PIXEL_HEIGHT;
  DMA2D_InitStruct.DMA2D_PixelPerLine = LCD_PIXEL_WIDTH;
  DMA2D_Init(&DMA2D_InitStruct);

  DMA2D_StartTransfer();

  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  }
}

/**
  * @brief  ����������
  * @param  Width,Height:���ο�Ⱥ͸߶�
  * @retval None
  */
void LCD_DrawFullRect_ARGB8888(uint16_t Width, uint16_t Height)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;

  uint32_t  Xaddress = 0;
  uint16_t Alpha_Value=0,Red_Value = 0, Green_Value = 0, Blue_Value = 0;

  Alpha_Value = (0xFF000000&0x00000000)>>24;
  Red_Value = (0x00FF0000 & 0x00000000) >> 16;
  Blue_Value = 0x000000FF & 0x00000000;
  Green_Value = (0x0000FF00 & 0x00000000) >> 8;
  
  Xaddress = CurrentFrameBuffer + 4*(LCD_PIXEL_WIDTH*(LCD_PIXEL_HEIGHT-Height)/2 +
                                    (LCD_PIXEL_WIDTH-Width)/2);

  DMA2D_DeInit();
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_ARGB8888;
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
  DMA2D_InitStruct.DMA2D_OutputAlpha = Alpha_Value;
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;
  DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - Width);
  DMA2D_InitStruct.DMA2D_NumberOfLine = Height;
  DMA2D_InitStruct.DMA2D_PixelPerLine = Width;
  DMA2D_Init(&DMA2D_InitStruct);

  DMA2D_StartTransfer();
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  }
  LCD_SetTextColor(CurrentTextColor);
}

/**
  * @brief  ����ǰ��ɫ
  * @param  Color:ǰ����ɫ
  * @retval None
  */
void LCD_SetTextColor_ARGB8888(uint32_t Color)
{
  CurrentTextColor_ARGB8888 =Color;
}

/**
  * @brief  ��������
  * @param  Xpos,Ypos:������ʼλ��;Length,Line_width:�������ȺͿ��;Direction:���Ʒ���
  * @retval None
  */
void LCD_DrawLine_ARGB8888(uint16_t Xpos,uint16_t Ypos,uint16_t Length,uint8_t Line_width,uint8_t Direction)
{
   DMA2D_InitTypeDef    DMA2D_InitStruct;

   uint32_t Xaddress = 0;
   uint16_t Alpha_Value=0,Red_Value = 0, Green_Value = 0, Blue_Value = 0;
  
   Alpha_Value = (0xFF000000&CurrentTextColor_ARGB8888)>>24;
   Red_Value = (0x00FF0000 & CurrentTextColor_ARGB8888) >> 16;
   Blue_Value = 0x000000FF & CurrentTextColor_ARGB8888;
   Green_Value = (0x0000FF00 & CurrentTextColor_ARGB8888) >> 8;

   Xaddress = CurrentFrameBuffer + 4*(LCD_PIXEL_WIDTH*Ypos + Xpos);

   DMA2D_DeInit();
   DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
   DMA2D_InitStruct.DMA2D_CMode = DMA2D_ARGB8888;
   DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
   DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
   DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
   DMA2D_InitStruct.DMA2D_OutputAlpha = Alpha_Value;
   DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;
	
   if(Direction == LCD_DIR_HORIZONTAL)                            //ˮƽ
   {
     DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH-Length;
     DMA2D_InitStruct.DMA2D_NumberOfLine = Line_width;
     DMA2D_InitStruct.DMA2D_PixelPerLine = Length;
   }
   else                                                             //��ֱ
   {
     DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - Line_width;
     DMA2D_InitStruct.DMA2D_NumberOfLine = Length;
     DMA2D_InitStruct.DMA2D_PixelPerLine = Line_width;
   }

   DMA2D_Init(&DMA2D_InitStruct);
   DMA2D_StartTransfer();
   while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
   {
   }

}

/**
  * @brief  ��������
  * @param  Xpos,Ypos:������ʼλ��;Length,Line_width:�������ȺͿ��;Direction:���Ʒ���
  * @retval None
  */
void LCD_DrawRect_ARGB8888(uint16_t Xpos, uint16_t Ypos, uint16_t Width,uint16_t Height,uint8_t Line_width)
{
  LCD_DrawLine_ARGB8888(Xpos, Ypos, Width,Line_width ,LCD_DIR_HORIZONTAL);
  LCD_DrawLine_ARGB8888(Xpos, (Ypos+ Height), Width+Line_width,Line_width ,LCD_DIR_HORIZONTAL);

  LCD_DrawLine_ARGB8888(Xpos, Ypos, Height, Line_width ,LCD_DIR_VERTICAL);
  LCD_DrawLine_ARGB8888((Xpos + Width), Ypos, Height, Line_width ,LCD_DIR_VERTICAL);
}

/**
  * @brief  ����ɨ���
  * @param  Width,Lengthɨ����������ȺͿ��;Size:ɨ����������;Color:������ɫ
  * @retval None
  */
void LCD_DrawBox_ARGB8888(uint16_t Width,uint16_t Length,uint16_t Size,uint32_t Color)
{
  LCD_SetTextColor_ARGB8888(Color);
  LCD_DrawRect_ARGB8888((LCD_PIXEL_WIDTH-Width)/2,(LCD_PIXEL_HEIGHT-Width)/2,\
	                      Width,Width-Size,Size); 
  LCD_SetTextColor_ARGB8888(0x00000000);
  LCD_DrawLine_ARGB8888((LCD_PIXEL_WIDTH-Width)/2+Length,
                        (LCD_PIXEL_HEIGHT-Width)/2,Width-2*Length,Size, LCD_DIR_HORIZONTAL);
    
  LCD_DrawLine_ARGB8888((LCD_PIXEL_WIDTH-Width)/2+Length,
                        (LCD_PIXEL_HEIGHT+Width)/2-Size,Width-2*Length,Size, LCD_DIR_HORIZONTAL);
    
  LCD_DrawLine_ARGB8888((LCD_PIXEL_WIDTH-Width)/2,
                        (LCD_PIXEL_HEIGHT-Width)/2+Length,Width-2*Length,Size, LCD_DIR_VERTICAL);
    
  LCD_DrawLine_ARGB8888((LCD_PIXEL_WIDTH+Width)/2,
                        (LCD_PIXEL_HEIGHT-Width)/2+Length, Width-2*Length,Size, LCD_DIR_VERTICAL);
}

/**
  * @brief  ����ɨ����
  * @param  None
  * @retval None
  */
void LCD_DrawLineScan_ARGB8888(void)
{
  LCD_SetLayer(LCD_FOREGROUND_LAYER);      //����Ϊǰ����
  LCD_SetTextColor_ARGB8888(0x00000000);   //����ǰ����ɫΪ͸��
    
  LCD_DrawLine_ARGB8888((LCD_PIXEL_WIDTH-Frame_Width+8*Frame_Line_Size)/2,pos, 
                         Frame_Width-8*Frame_Line_Size,Frame_Line_Size, LCD_DIR_HORIZONTAL);
                                          //����͸����ɫ������,��������ϴλ��Ƶ�����
  pos=pos+Frame_Line_Size;                //�ƶ�����
    
  if(pos>=((LCD_PIXEL_HEIGHT+Frame_Width)/2-5*Frame_Line_Size))//����
  {
    pos = (LCD_PIXEL_HEIGHT-Frame_Width)/2+5*Frame_Line_Size;
  }
   
  LCD_SetTextColor_ARGB8888(0xD05CBDE7);    //��������Ϊǳ��ɫ
    
  LCD_DrawLine_ARGB8888((LCD_PIXEL_WIDTH-Frame_Width+8*Frame_Line_Size)/2,pos, 
                        Frame_Width-8*Frame_Line_Size,Frame_Line_Size, LCD_DIR_HORIZONTAL);
                                           //��������
} 

/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    PutPixel(x, y);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays an triangle.
  * @param  Points: pointer to the points array.
  * @retval None
  */
void LCD_Triangle(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount != 3)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawUniLine(X, Y, Points->X, Points->Y);
  }
  LCD_DrawUniLine(First->X, First->Y, Points->X, Points->Y);
}

/**
  * @brief  Fill an triangle (between 3 points).
  * @param  x1..3: x position of triangle point 1..3.
  * @param  y1..3: y position of triangle point 1..3.
  * @retval None
  */
void LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{ 
  
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    LCD_DrawUniLine(x, y, x3, y3);
    
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }  
  
  
}
/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;

  if(PointCount < 2)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawUniLine(X, Y, Points->X, Points->Y);
  }
}

/**
  * @brief  Displays an relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
  int16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount < 2)
  {
    return;
  }  
  X = Points->X;
  Y = Points->Y;
  while(--PointCount)
  {
    Points++;
    LCD_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
    X = X + Points->X;
    Y = Y + Points->Y;
  }
  if(Closed)
  {
    LCD_DrawUniLine(First->X, First->Y, X, Y);
  }  
}

/**
  * @brief  Displays a closed poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLine(Points, PointCount);
  LCD_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
  * @brief  Displays a relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}

/**
  * @brief  Displays a  full poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount)
{
 
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;  

  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

  for(counter = 1; counter < PointCount; counter++)
  {
    pixelX = POLY_X(counter);
    if(pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if(pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }
    
    pixelY = POLY_Y(counter);
    if(pixelY < IMAGE_TOP)
    { 
      IMAGE_TOP = pixelY;
    }
    if(pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }  
  
  if(PointCount < 2)
  {
    return;
  }
  
  X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
  Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;
 
  X_first = Points->X;
  Y_first = Points->Y;
  
  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;    
  
    LCD_FillTriangle(X, X2, X_center, Y, Y2, Y_center);
    LCD_FillTriangle(X, X_center, X2, Y, Y_center, Y2);
    LCD_FillTriangle(X_center, X2, X, Y_center, Y2, Y);   
  }
  
  LCD_FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
  LCD_FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
  LCD_FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first); 
}


/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void)
{
  /* Enable LCD Backlight */ 
  GPIO_SetBits(GPIOI, GPIO_Pin_11);
  /* Display On */
  LTDC_Cmd(ENABLE); /* display ON */
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void)
{
  /* Enable LCD Backlight */ 
  GPIO_ResetBits(GPIOI, GPIO_Pin_11);
  /* Display Off */
  LTDC_Cmd(DISABLE); 
}

/**
  * @brief  GPIO config for LTDC.
  * @param  None
  * @retval None
  */
 void LCD_AF_GPIOConfig(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
  /*
   ****************************************************************************
   *R3 --> PB0  | G2 --> PH13 | B3 --> PG11 | DE    --> PF10
   *R4 --> PA11 | G3 --> PG10 | B4 --> PG12 | BL_EN --> PI11
   *R5 --> PA12 | G4 --> PH15 | B5 -->  PA3 | VSYNC --> PI10
   *R6 --> PB1  | G5 -->  PI0 | B6 -->  PI6 | HSYNC -->  PI9
   *R7 --> PG6  | G6 -->  PI1 | B7 -->  PI7 | CLK   -->  PG7
   *            | G7 -->  PI2 |
   ****************************************************************************
   */
	/* ʹ��GPIOʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG 
                         | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOA , ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  /* PB1����ΪLCD_R6 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, 0x09);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, 0x09);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_13
                                |  GPIO_Pin_15;
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 
                                | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5; //LR UD
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);                    //LR
  GPIO_ResetBits(GPIOB,GPIO_Pin_3);                  //UD    �������

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //LCD_RESET
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);                  //__����
	
	
	 /* BL_EN pin configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
	LCD_DisplayOn();
	
	GPIO_SetBits(GPIOG,GPIO_Pin_9);                    //LCD_RESET   __����  low-->height
	
  
 
}

/**
  * @brief  Displays a pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.  
  * @retval None
  */
 void PutPixel(int16_t x, int16_t y)
{ 
  if(x < 0 || x > 479 || y < 0 || y > 271)
  {
    return;  
  }
  LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}

/**
  * @brief  Displays a color pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.  
  * @param  Color: 
  * @retval None
  */
 void PutPixelColor(int16_t x, int16_t y,uint16_t Color)
{ 

  LCD_DrawLineColor(x, y, 1, LCD_DIR_HORIZONTAL ,Color);   //LCD_DIR_VERTICAL
}


/**
  * @brief  m^n����
  * @param  m: 
  * @param  n:  
  * @retval m^n�η�.
  */
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/**
  * @brief  дһ���ַ�
  * @param  x,y :�ַ���ʼ����
  * @param  num: ��ʾ���ַ� 
  * @param  PenColor,BackColor:�ַ���ɫ,������ɫ
  * @param  font:0:8x16; 1:16x24 �û�������Լ���Ҫ���ַ���С
  * @retval None
  */
void WriteOneChar(uint16_t x, 
                  uint16_t y, 
                  uint8_t num, 
                  uint16_t PenColor, 
                  uint16_t BackColor,
                  uint8_t font)
{   
    uint8_t  pos,t;
    uint16_t temp;

  	num=num-' ';                                                       //�õ�ƫ�ƺ��ֵ               
    if( font==FONT_8x16){                                            //���ʹ�õ���8*16������       
        CHAR_H = 16;
        CHAR_W = 8;
        for(pos=0;pos<CHAR_H;pos++) {
          	temp = ASCII8x16_Table[num*CHAR_H+pos];
            for(t=0;t<CHAR_W;t++) {
                if(temp&0x80){			                                 //�Ӹ�λ��ʼ                   		
    				PutPixelColor(x+t, y+pos, PenColor);                     //��������ɫ һ����            
    			} else {			
    				PutPixelColor(x+t, y+pos, BackColor);	                   //��������ɫ һ����            
                }     
	            temp<<=1;
            }        
        }      
    } else {                                                         //���ʹ�õ���16*24������     
        CHAR_H = 24;
        CHAR_W = 16;
        for(pos=0;pos<CHAR_H;pos++) {
            temp = ASCII16x24_Table[num*CHAR_H+pos];
            for(t=0;t<CHAR_W;t++) {
                if(temp&0x01){			                                 //�ӵ�λ��ʼ                   		
    				PutPixelColor(x+t, y+pos, PenColor);                     //��������ɫ һ����            
    			} else {			
    				PutPixelColor(x+t, y+pos, BackColor);	                   //��������ɫ һ����           
                }     
	            temp>>=1;
							
            }        
        }
    }
}

/**
  * @brief  ��ʾ�ַ���
  * @param  x,y :�ַ���ʼ����
  * @param  s:ָ���ַ����׵�ַ
  * @param  PenColor,BackColor:�ַ���ɫ,������ɫ
  * @param  font:0:8x16; 1:16x24 �û�������Լ���Ҫ���ַ���С
  * @retval None
  */
void LCD_DisplayStr(uint16_t x, 
                    uint16_t y, 
                    uint8_t * s, 
                    uint16_t PenColor, 
                    uint16_t BackColor,
                    uint8_t font)
{
    while (*s!='\0') {
        WriteOneChar(x,y,*s++,PenColor,BackColor,font);
        x+=CHAR_W;
    }
}

/**
  * @brief  ��ʾ����
  * @param  x,y:���ֿ�ʼ����
  * @param  num: ��ʾ������
  * @param  len: �ַ�����
  * @param  size: 0:8x16; 1:16x24 �û�������Լ���Ҫ���ַ���С
  * @param  PenColor,BackColor:�ַ���ɫ,������ɫ
  * @retval None
  */
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,uint16_t PenColor, 
                  uint16_t BackColor)
{         	
	u8 t,temp;
	u8 enshow=0;		
 
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				WriteOneChar(x+(12)*t,y,' ',PenColor, 
                   BackColor,size);
				continue;
			}else enshow=1; 		 	 
		}
	 	WriteOneChar(x+(12)*t,y,'0'+temp,PenColor, 
                   BackColor,size); 
		
	}
} 

#ifndef USE_Delay
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = nCount; index != 0; index--)
  {
  }
}
#endif /* USE_Delay*/
