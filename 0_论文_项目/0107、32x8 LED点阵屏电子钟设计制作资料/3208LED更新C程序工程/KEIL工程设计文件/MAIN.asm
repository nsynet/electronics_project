MAINP  SEGMENT  CODE    ;程序段

PUBLIC  MAIN			  ;入口地址，跳转到DL标号处执行汇编程序

RSEG  MAINP			  ;程序段


	   ;这是关于DS18B20的读写程序,数据脚IN,晶振12MHZ
;温度传感器18B20汇编程序,采用器件默认的12位转化,最大转化时间750微秒
;可以将检测到的温度直接显示到AT89C51开发实验板的两个数码管上
;显示温度00到99度,很准确哦~~无需校正!

;单片机内存分配申明!
TEMPER_L EQU 29H;用于保存读出温度的低8位
TEMPER_H EQU 28H;用于保存读出温度的高8位
in equ P3.3



MAIN: 
LCALL GET_TEMPER;调用读温度子程序

;进行温度显示,这里我们考虑用网站提供的两位数码管来显示温度
;显示范围00到99度,显示精度为1度
;因为12位转化时每一位的精度为0.0625度,我们不要求显示小数所以可以抛弃29H的低4位
;将28H中的低4位移入29H中的高4位,这样获得一个新字节,这个字节就是实际测量获得的温度
;这个转化温度的方法可是我想出来的哦~~非常简洁无需乘于0.0625系数

MOV A,29H
	 MOV B,28H
MOV C,B.0;将28H中的最低位移入C
RRC A
MOV C,B.1
RRC A
MOV C,B.2
RRC A
MOV C,B.3
RRC A
MOV 29H,A 

RET


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
;SETB FLAG1 ; 置标志位,表示DS1820存在
LJMP TSR5
TSR4:
;CLR FLAG1 ; 清标志位,表示DS1820不存在
LJMP TSR7

TSR5:
MOV R0,#117
TSR6:
DJNZ R0,TSR6 ; 时序要求延时一段时间
TSR7:
SETB IN
RET


; 读出转换后的温度值
GET_TEMPER:
SETB IN
LCALL INIT_1820;先复位DS18B20
;JB FLAG1,TSS2
;CLR P1.2
;RET ; 判断DS1820是否存在?若DS18B20不存在则返回
TSS2:
;CLR P1.3;DS18B20已经被检测到!!!!!!!!!!!!!!!!!!
MOV A,#0CCH ; 跳过ROM匹配
LCALL WRITE_1820
MOV A,#44H ; 发出温度转换命令
LCALL WRITE_1820

;这里通过调用显示子程序实现延时一段时间,等待AD转换结束,12位的话750微秒
;LCALL D1MS;DISPLAY

LCALL INIT_1820;准备读温度前先复位

MOV A,#0CCH ; 跳过ROM匹配
LCALL WRITE_1820

MOV A,#0BEH ; 发出读温度命令
LCALL WRITE_1820

LCALL READ_18200; 将读出的温度数据保存到35H/36H 

RET

;写DS18B20的子程序(有具体的时序要求)
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

; 读DS18B20的程序,从DS18B20中读出两个字节的温度数据 
READ_18200:
MOV R4,#2 ; 将温度高位和低位从DS18B20中读出
MOV R1,#29H ; 低位存入29H(TEMPER_L),高位存入28H(TEMPER_H)
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


end
