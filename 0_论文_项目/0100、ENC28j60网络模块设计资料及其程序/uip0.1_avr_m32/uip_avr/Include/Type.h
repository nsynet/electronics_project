/*
;*****************************************************************************************************
;*    				Copyright (c) 2006 by JiaoJinXing. 
;*					All rights reserved.
;*
;*---- �ļ���Ϣ --------------------------------------------------------------------------------------
;*��   ��   �� : Type.h
;*��   ��   �� : ������
;*��        �� : �������Ͷ���ͷ�ļ�
;*
;*---- ��ʷ�汾��Ϣ ----------------------------------------------------------------------------------
;* �ա�		�� : 2008�� 12 �� 20 ��
;* ��	��	�� : ������
;* �衡		�� : �����汾 V1.0.0
;*
;*---- ��ϵ��ʽ --------------------------------------------------------------------------------------
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
;*			���������ص���������
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
