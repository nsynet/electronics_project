#ifndef __QR_DECODER_H
#define	__QR_DECODER_H

#include <stdio.h>
#include "stm32f4xx.h"

#define  QR_FRAME_BUFFER  ((uint32_t)0xD0500000) 

#define  Frame_Width           ((uint16_t)320)    //ɨ���Ĵ�С

#define  Frame_Line_Length     ((uint16_t)30)     //ɨ�����������
#define  Frame_Line_Size       ((uint16_t)3)      //ɨ����������

#define  QR_SYMBOL_NUM          3                  //ʶ���ά���������
#define  QR_SYMBOL_SIZE         512                //ÿ���ά��ĵ��������

//�������ݸ�ʽΪ��
// (��һ��:�������ͳ���(8bit)+������������+�������ݳ���(16bit,��λ��ǰ��λ�ں�)+��������)
// (�ڶ���:�������ͳ���(8bit)+������������+�������ݳ���(16bit,��λ��ǰ��λ�ں�)+��������)
//  ������
//�Դ�����
extern char decoded_buf[QR_SYMBOL_NUM][QR_SYMBOL_SIZE];

char QR_decoder(void);                                                  //����ʶ�����
void get_image(uint32_t src_addr,uint16_t img_width,uint16_t img_height);//��ȡһ֡ͼ��
#endif 
