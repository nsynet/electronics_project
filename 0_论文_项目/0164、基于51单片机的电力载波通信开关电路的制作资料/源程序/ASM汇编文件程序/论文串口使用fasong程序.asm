/*

 �˳����Ѿ����������Ե��Թ��������ѧϰ������ʲô���ʣ���QQ��277039462
 ����ͬѧ��ϵ��


*/

 ORG 00H		 
 JMP START 		 //��ʼ
 ORG 10H		  //��ʼ��ַ
START:
	MOV SP,#60H	   //���ö�ե��ַ
	MOV SCON,#50H	//����ͨ�ŷ�ʽ2�������շ�
	MOV  TMOD,#20H	//ѡ��ʱ��/������1�еķ�ʽ2
	MOV TH1,#0FDH	//������9600bit/s
	MOV TL1,#0FDH
	SETB TR1	 //����������
	SETB P3.3	 //�����������1
	SETB P3.5	 //LED��ʾ����
SCAN:
	JB RI,UART	  //�ж��Ƿ����
	JNB P3.3,KEYIN	//�жϰ����Ƿ��£����򰴼�����
	JMP SCAN		//���ǰ���ɨ��
KEYIN:
 	MOV  A,#5AH		//��������װ��A
	MOV SBUF,A		//��������
WAIT:				// �ȴ�����
	JBC TI,SCAN		//�����������TI��־λ
	JMP WAIT
UART:			   //�������ݴ���
 	MOV A,SBUF 
	CJNE A,#5AH,ERROR  //�򵥵ĵ�ַ������Ϊ5AH,�û�����㶨��
	CLR RI				//������ձ�־λ
	CPL P3.5			//������
ERROR:
	JMP SCAN			//���ص�ɨ��
END

	

