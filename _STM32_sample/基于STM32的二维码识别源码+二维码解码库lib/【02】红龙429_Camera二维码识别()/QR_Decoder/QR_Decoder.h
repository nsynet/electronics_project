#ifndef __QR_DECODER_H
#define	__QR_DECODER_H

#include <stdio.h>
#include "stm32f4xx.h"

#define  QR_FRAME_BUFFER  ((uint32_t)0xD0500000) 

#define  Frame_Width           ((uint16_t)320)    //扫描框的大小

#define  Frame_Line_Length     ((uint16_t)30)     //扫描框线条长度
#define  Frame_Line_Size       ((uint16_t)3)      //扫描框线条宽度

#define  QR_SYMBOL_NUM          3                  //识别二维码的最大个数
#define  QR_SYMBOL_SIZE         512                //每组二维码的的最大容量

//解码数据格式为：
// (第一组:解码类型长度(8bit)+解码类型名称+解码数据长度(16bit,高位在前低位在后)+解码数据)
// (第二组:解码类型长度(8bit)+解码类型名称+解码数据长度(16bit,高位在前低位在后)+解码数据)
//  。。。
//以此类推
extern char decoded_buf[QR_SYMBOL_NUM][QR_SYMBOL_SIZE];

char QR_decoder(void);                                                  //返回识别个数
void get_image(uint32_t src_addr,uint16_t img_width,uint16_t img_height);//获取一帧图像
#endif 
