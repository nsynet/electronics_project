m16+enc28j60组成的网络接口。
m16使用内部RC振荡8M。
enc28j60模块使用www.icdev.com.cn的。
软件编写使用avrstudio4.12+sp4+winavr（avrstudio自带的winavr嵌入方式）。
硬件连接：
m16	| enc28j60
PB4	| CS
PB5	| SI
PB6	| SO
PB7	| SCK
PB0	| nRST
PD2	| INT
PD3	| WOL

WOL引脚用于网络唤醒功能，在本程序中并未使用；INT引脚也未使用，程序采用查询寄存器的方式接收数据包。
由于采用的是spi接口方式，其读写速度有限，加上m16的处理速度，经测试实际网速约为双向30kBps(240kbps)。
由于ram有限，仅仅采用了332Bytes的数据缓冲做为收发buffer，超过此长度的以太网包将抛弃。
此工程文件仅仅做为enc28j60的一个移植范例，高级tcpip应用代码因商业缘故不会提供，请勿找我询问。

文件编写，参考了MicroChip公司的cn026314.pcf数据手册以及http://www.olimex.com的应用代码，部分采用了作者本人的avr m128高级应用板以太网应用代码，特此声名！

源文件供大家参考，可任意使用，但请转载时注明出处：http://www.embed.org.cn。
