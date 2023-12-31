/*-------------------------------------------
项目名：　　　2006年毕业设计项目
程序名： 　　 32*8点阵屏电子钟--字节转换（嵌入汇编程序）
编写人： 　　 杜洋　
初写时间：　　2005-12-15~2005-12-　　
CPU说明：　　 MCS-51（AT89S52）　12MHZ
接口说明：　　DS1302、DS18B20
信息说明：　　程序为_DL有反回值在R7中，函数名为DL
修改日志：　　
　　NO.1-20051215　完成LED驱动、显示切换、时钟温度读取
	NO.2-20051216　完成整点报时、填加注释信息
　	NO.3-20051220　完成了调时、按键音等所以的功能（没有闹钟）
-------------------------------------------*/

DLP  SEGMENT  CODE    ;程序段

PUBLIC  _DL			  ;入口地址，跳转到DL标号处执行汇编程序

RSEG  DLP			  ;程序段



_DL:;125MS延时汇编程序
    ;-------------------------------------------字节全转换程序
ABBA:  ;将字节中的76543210变为01234567
	MOV		A,R7
     MOV    B,A
	 MOV    C,B.0
	 MOV    ACC.7,C
	 MOV    C,B.1
	 MOV    ACC.6,C
	 MOV    C,B.2
	 MOV    ACC.5,C
	 MOV    C,B.3
	 MOV    ACC.4,C
	 MOV    C,B.4
	 MOV    ACC.3,C
	 MOV    C,B.5
	 MOV    ACC.2,C
	 MOV    C,B.6
	 MOV    ACC.1,C
	 MOV    C,B.7
	 MOV    ACC.0,C
	 MOV	R7,A
    RET				  ;汇编结束，返回C程序


	end				  ;程序文件编译结束





