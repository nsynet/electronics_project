MAINP  SEGMENT  CODE    ;�����

PUBLIC  MAIN			  ;��ڵ�ַ����ת��DL��Ŵ�ִ�л�����

RSEG  MAINP			  ;�����


	   ;���ǹ���DS18B20�Ķ�д����,���ݽ�IN,����12MHZ
;�¶ȴ�����18B20������,��������Ĭ�ϵ�12λת��,���ת��ʱ��750΢��
;���Խ���⵽���¶�ֱ����ʾ��AT89C51����ʵ���������������
;��ʾ�¶�00��99��,��׼ȷŶ~~����У��!

;��Ƭ���ڴ��������!
TEMPER_L EQU 29H;���ڱ�������¶ȵĵ�8λ
TEMPER_H EQU 28H;���ڱ�������¶ȵĸ�8λ
in equ P3.3



MAIN: 
LCALL GET_TEMPER;���ö��¶��ӳ���

;�����¶���ʾ,�������ǿ�������վ�ṩ����λ���������ʾ�¶�
;��ʾ��Χ00��99��,��ʾ����Ϊ1��
;��Ϊ12λת��ʱÿһλ�ľ���Ϊ0.0625��,���ǲ�Ҫ����ʾС�����Կ�������29H�ĵ�4λ
;��28H�еĵ�4λ����29H�еĸ�4λ,�������һ�����ֽ�,����ֽھ���ʵ�ʲ�����õ��¶�
;���ת���¶ȵķ����������������Ŷ~~�ǳ�����������0.0625ϵ��

MOV A,29H
	 MOV B,28H
MOV C,B.0;��28H�е����λ����C
RRC A
MOV C,B.1
RRC A
MOV C,B.2
RRC A
MOV C,B.3
RRC A
MOV 29H,A 

RET


; ����DS18B20��λ��ʼ���ӳ���
INIT_1820:
SETB IN
NOP
CLR IN
;����������ʱ537΢��ĸ�λ������
MOV R1,#3
TSR1:MOV R0,#107
DJNZ R0,$
DJNZ R1,TSR1
SETB IN;Ȼ������������
NOP
NOP
NOP
MOV R0,#25H
TSR2:
JNB IN,TSR3;�ȴ�DS18B20��Ӧ
DJNZ R0,TSR2
LJMP TSR4 ; ��ʱ
TSR3:
;SETB FLAG1 ; �ñ�־λ,��ʾDS1820����
LJMP TSR5
TSR4:
;CLR FLAG1 ; ���־λ,��ʾDS1820������
LJMP TSR7

TSR5:
MOV R0,#117
TSR6:
DJNZ R0,TSR6 ; ʱ��Ҫ����ʱһ��ʱ��
TSR7:
SETB IN
RET


; ����ת������¶�ֵ
GET_TEMPER:
SETB IN
LCALL INIT_1820;�ȸ�λDS18B20
;JB FLAG1,TSS2
;CLR P1.2
;RET ; �ж�DS1820�Ƿ����?��DS18B20�������򷵻�
TSS2:
;CLR P1.3;DS18B20�Ѿ�����⵽!!!!!!!!!!!!!!!!!!
MOV A,#0CCH ; ����ROMƥ��
LCALL WRITE_1820
MOV A,#44H ; �����¶�ת������
LCALL WRITE_1820

;����ͨ��������ʾ�ӳ���ʵ����ʱһ��ʱ��,�ȴ�ADת������,12λ�Ļ�750΢��
;LCALL D1MS;DISPLAY

LCALL INIT_1820;׼�����¶�ǰ�ȸ�λ

MOV A,#0CCH ; ����ROMƥ��
LCALL WRITE_1820

MOV A,#0BEH ; �������¶�����
LCALL WRITE_1820

LCALL READ_18200; ���������¶����ݱ��浽35H/36H 

RET

;дDS18B20���ӳ���(�о����ʱ��Ҫ��)
WRITE_1820:
MOV R2,#8;һ��8λ����
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

; ��DS18B20�ĳ���,��DS18B20�ж��������ֽڵ��¶����� 
READ_18200:
MOV R4,#2 ; ���¶ȸ�λ�͵�λ��DS18B20�ж���
MOV R1,#29H ; ��λ����29H(TEMPER_L),��λ����28H(TEMPER_H)
RE00:
MOV R2,#8;����һ����8λ
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
