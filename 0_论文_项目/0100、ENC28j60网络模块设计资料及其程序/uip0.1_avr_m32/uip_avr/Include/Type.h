/*
;*****************************************************************************************************
;*    				Copyright (c) 2006 by JiaoJinXing. 
;*					All rights reserved.
;*
;*---- 文件信息 --------------------------------------------------------------------------------------
;*文   件   名 : Type.h
;*创   建   人 : 焦进星
;*描        述 : 数据类型定义头文件
;*
;*---- 历史版本信息 ----------------------------------------------------------------------------------
;* 日　		期 : 2008年 12 月 20 日
;* 创	建	人 : 焦进星
;* 描　		述 : 建立版本 V1.0.0
;*
;*---- 联系方式 --------------------------------------------------------------------------------------
;* Email    : wyoujtg@163.com
;* Web      : http://wyoujtg.blog.163.com
;*     
;*****************************************************************************************************
;*/

#ifndef TYPE_H
#define TYPE_H

#ifdef __cplusplus
extern "C" {
#endif


/*
;***************************************************
;*			与编译器相关的数据类型
;***************************************************
;*/  
typedef unsigned char     boolean;
typedef unsigned char     uint8;                 /* Unsigned  8 bit quantity                            */
typedef signed   char     int8;                  /* Signed    8 bit quantity                            */
typedef unsigned int      uint16;                /* Unsigned 16 bit quantity                            */
typedef signed   int      int16;                 /* Signed   16 bit quantity                            */
typedef unsigned long     uint32;                /* Unsigned 32 bit quantity                            */
typedef signed   long     int32;                 /* Signed   32 bit quantity                            */
typedef float             fp32;                  /* Single precision floating point                     */


#ifndef NULL
#define NULL	(0)
#endif


#ifndef FALSE
#define FALSE	(0)
#endif


#ifndef TRUE
#define TRUE	(1)
#endif


#ifdef __cplusplus
}
#endif

#endif

/*
;*****************************************************************************************************
;*                            			End Of File
;*****************************************************************************************************
;*/
