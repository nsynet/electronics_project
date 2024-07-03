#define SPI_MASTER      0x003f	           // 8位模式, CKE=0, CKP=0 F=Fcy 3c-1:64,3d-1:16,3e-1:4,3f-1:1
#define SPI_ENABLE	    0x8000	           // 清除标志位，使能SPI

extern unsigned char SPI1_ReadWrite(unsigned char i);//写一个字节
extern void SPI1_Init(void);                         //中断配置


