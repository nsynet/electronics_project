/* @NewSimSun_13.c - @NewSimSun_13 font for UCGUI font driver */ 
/********************************************************************************************************
*                                            UCGUI
*                                       http://www.ucgui.com
*
*                             (c) Copyright 2005-2006, UCGUI专业论坛, 学习交流UCGUI
*
*                               	ucgui@16.com 2005.07.31
*                                           作者: ucgui
*********************************************************************************************************
*/

#include "..\core\GUI.H" 
#ifndef GUI_FLASH 
	#define GUI_FLASH 
#endif 
extern GUI_FLASH const GUI_FONT GUI_FontHZ_@NewSimSun_13;

GUI_FLASH  const unsigned char acFontHZ@NewSimSun_13_002e[13] = {	/* . 0x2e00*/
________,
________,
________,
________,
________,
________,
________,
________,
________,
_XX_____,
_XX_____,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZ@NewSimSun_13_0031[13] = {	/* 1 0x3100*/
________,
________,
___X____,
__XX____,
___X____,
___X____,
___X____,
___X____,
___X____,
___X____,
__XXX___,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZ@NewSimSun_13_0032[13] = {	/* 2 0x3200*/
________,
________,
__XXX___,
_X___X__,
_X___X__,
_____X__,
____X___,
___X____,
__X_____,
_X______,
_XXXXX__,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZ@NewSimSun_13_0035[13] = {	/* 5 0x3500*/
________,
________,
_XXXXX__,
_X______,
_X______,
_XXXX___,
_X___X__,
_____X__,
_X___X__,
_X___X__,
__XXX___,
________,
________,
};

GUI_FLASH  const unsigned char acFontHZ@NewSimSun_13_a1e6[26] = {	/* ℃ 0xa1e6*/
________,________,
________,_X______,
___XXX__,__X_____,
____X___,___X____,
___X____,___X____,
___X____,___X____,
___X____,___X____,
____X___,__X_____,
_____XXX,XX______,
___X____,________,
__X_X___,________,
___X____,________,
________,________,
};

GUI_FLASH const GUI_CHARINFO GUI_FontHZ@NewSimSun_13_CharInfo[7710] = {
{ 7, 7, 1, (void GUI_FLASH *)&acFontHZ@NewSimSun_13_002e},/* . 0 */
{ 7, 7, 1, (void GUI_FLASH *)&acFontHZ@NewSimSun_13_0031},/* 1 1 */
{ 7, 7, 1, (void GUI_FLASH *)&acFontHZ@NewSimSun_13_0032},/* 2 2 */
{ 7, 7, 1, (void GUI_FLASH *)&acFontHZ@NewSimSun_13_0035},/* 5 3 */
{ 13, 13, 2, (void GUI_FLASH *)&acFontHZ@NewSimSun_13_a1e6},/* ℃ 96 */
};

/*℃--℃*/

GUI_FLASH const GUI_FONT_PROP GUI_FontHZ@NewSimSun_13_Propa1e6= {
      0xa1e6,
      0xa1e6,
      &GUI_FontHZ@NewSimSun_13_CharInfo[4],
      (void *)0 
};
/*5--5*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ@NewSimSun_13_Prop0035= {
      0x0035,
      0x0035,
      &GUI_FontHZ@NewSimSun_13_CharInfo[3],
      (void *)&GUI_FontHZ@NewSimSun_13_Propa1e6 
};
/*1--2*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ@NewSimSun_13_Prop0031= {
      0x0031,
      0x0032,
      &GUI_FontHZ@NewSimSun_13_CharInfo[1],
      (void *)&GUI_FontHZ@NewSimSun_13_Prop0035 
};
/*.--.*/
GUI_FLASH  const GUI_FONT_PROP GUI_FontHZ@NewSimSun_13_Prop002e= {
      0x002e,
      0x002e,
      &GUI_FontHZ@NewSimSun_13_CharInfo[0],
      (void *)&GUI_FontHZ@NewSimSun_13_Prop0031 
};
GUI_FLASH const GUI_FONT GUI_FontHZ_@NewSimSun_13 = {
      GUI_FONTTYPE_PROP_SJIS, 
      13, 
      13, 
      1,  
      1,  
      (void GUI_FLASH *)&GUI_FontHZ@NewSimSun_13_Prop002e
};
