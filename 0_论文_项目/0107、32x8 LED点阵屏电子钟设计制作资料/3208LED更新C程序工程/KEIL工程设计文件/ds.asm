/*-------------------------------------------
项目名：　　　2006年毕业设计项目
程序名： 　　 32*8点阵屏电子钟--温度读取（嵌入汇编程序）
编写人： 　　 杜洋　
初写时间：　　2005-12-15~2005-12-20　　
CPU说明：　　 MCS-51（AT89S52）　12MHZ
接口说明：　　DS1302、DS18B20
信息说明：　　程序为_DS有反回值在R7中，函数名为DS
修改日志：　　
　　NO.1-20051215　完成LED驱动、显示切换、时钟温度读取
	NO.2-20051216　完成整点报时、填加注释信息
　	NO.3-20051220　完成了调时、按键音等所以的功能（没有闹钟）
-------------------------------------------*/

DSSP  SEGMENT  CODE    ;程序段

PUBLIC  DSS			  ;入口地址，跳转到DL标号处执行汇编程序

RSEG  DSSP			  ;程序段



IN EQU P1.6
FLAG1     EQU   19H   ;DS18B20器件存在标志
TTL       EQU   21H   ;温度读出值（高位在TTL-1中，低位在TTL中）
;-------------------------------------------DS18B20读数据程序（0~99度）
; 这是DS18B20复位初始化子程序
INIT_1820:
SETB IN
NOP
CLR IN
;主机发出延时537微秒的复位低脉冲
MOV R1,#3
TSR1:MOV R0,#107
DJNZ R0,$
DJNZ R1,TSR1
SETB IN;然后拉高数据线
NOP
NOP
NOP
MOV R0,#25H
TSR2:
JNB IN,TSR3;等待DS18B20回应
DJNZ R0,TSR2
LJMP TSR4 ; 延时
TSR3:
SETB FLAG1 ; 置标志位,表示DS1820存在
LJMP TSR5
TSR4:
CLR FLAG1 ; 清标志位,表示DS1820不存在
LJMP TSR7
TSR5:		   
MOV R0,#117
TSR6:
DJNZ R0,TSR6 ; 时序要求延时一段时间 */
TSR7:
SETB IN
RET
;------------------------------------------读出转换后的温度值
DSS:
DS1820PRO:
;这里通过调用显示子程序实现延时一段时间,等待AD转换结束,12位的话750微秒
SETB IN
LCALL INIT_1820;先复位DS18B20
JB FLAG1,TSS2
jmp BCD33 ; 判断DS1820是否存在?若DS18B20不存在则返回
TSS2:
MOV A,#0CCH ; 跳过ROM匹配
LCALL WRITE_1820
MOV A,#44H ; 发出温度转换命令
LCALL WRITE_1820
CALL  DL1MS 
SETB IN
LCALL INIT_1820;准备读温度前先复位
MOV A,#0CCH ; 跳过ROM匹配
LCALL WRITE_1820
MOV A,#0BEH ; 发出读温度命令
LCALL WRITE_1820
LCALL READ_18200; 将读出的温度数据保存到35H/36H 
MOV A,TTL
MOV B,TTL-1
MOV C,B.0;将28H中的最低位移入C
RRC A
MOV C,B.1
RRC A
MOV C,B.2
RRC A
MOV C,B.3
RRC A
MOV TTL,A 
BCD33: 
JB FLAG1,BCD44
mov R7,#0FFH
JMP TORET
BCD44:
MOV R7,TTL
TORET:
RET
;--------------------------------写DS18B20的子程序(有具体的时序要求)
WRITE_1820:
MOV R2,#8;一共8位数据
CLR C
WR1:
CLR IN
MOV R3,#6
DJNZ R3,$
RRC A
MOV IN,C
MOV R3,#23
DJNZ R3,$
SETB IN
NOP
DJNZ R2,WR1
SETB IN
RET
;-----------------------------读DS18B20的程序,从DS18B20中读出两个字节的温度数据 
READ_18200:
MOV R4,#2 ; 将温度高位和低位从DS18B20中读出
MOV R1,#TTL ; 低位存入29H(TEMPER_L),高位存入28H(TEMPER_H)
RE00:
MOV R2,#8;数据一共有8位
RE01:
CLR C
SETB IN
NOP
NOP
CLR IN
NOP
NOP
NOP
SETB IN
MOV R3,#9
RE10: 
DJNZ R3,RE10
MOV C,IN
MOV R3,#23
RE20: 
DJNZ R3,RE20
RRC A
DJNZ R2,RE01
MOV @R1,A
DEC R1
DJNZ R4,RE00
RET

DL1MS:
    MOV R6,#255
	 DJNZ R6,$
    RET 

END
