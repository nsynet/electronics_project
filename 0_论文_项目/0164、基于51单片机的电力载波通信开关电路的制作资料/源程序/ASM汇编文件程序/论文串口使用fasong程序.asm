/*

 此程序已经经本人亲自调试过，如果在学习当中有什么疑问，可QQ：277039462
 找马同学联系。


*/

 ORG 00H		 
 JMP START 		 //开始
 ORG 10H		  //开始地址
START:
	MOV SP,#60H	   //设置堆榨地址
	MOV SCON,#50H	//串口通信方式2，允许收发
	MOV  TMOD,#20H	//选择顶时器/计数器1中的方式2
	MOV TH1,#0FDH	//波特率9600bit/s
	MOV TL1,#0FDH
	SETB TR1	 //开启计数器
	SETB P3.3	 //开关输入口置1
	SETB P3.5	 //LED显示口灭
SCAN:
	JB RI,UART	  //判断是否接收
	JNB P3.3,KEYIN	//判断按键是否按下，是则按键处理
	JMP SCAN		//不是按键扫描
KEYIN:
 	MOV  A,#5AH		//发送数据装入A
	MOV SBUF,A		//发送数据
WAIT:				// 等待发送
	JBC TI,SCAN		//发送完了清除TI标志位
	JMP WAIT
UART:			   //接收数据处理
 	MOV A,SBUF 
	CJNE A,#5AH,ERROR  //简单的地址码设置为5AH,用户可随便定义
	CLR RI				//清除接收标志位
	CPL P3.5			//点亮灯
ERROR:
	JMP SCAN			//返回到扫描
END

	

