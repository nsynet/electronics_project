;CodeVisionAVR C Compiler V1.24.5 Standard
;(C) Copyright 1998-2005 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com
;e-mail:office@hpinfotech.com

;Chip type              : ATmega16L
;Program type           : Application
;Clock frequency        : 8.000000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External SRAM size     : 0
;Data Stack size        : 256 byte(s)
;Heap size              : 0 byte(s)
;Promote char to int    : No
;char is unsigned       : Yes
;8 bit enums            : Yes
;Word align FLASH struct: No
;Enhanced core instructions    : On
;Automatic register allocation : On

	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __se_bit=0x40
	.EQU __sm_mask=0xB0
	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0xA0
	.EQU __sm_ext_standby=0xB0

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __CLRD1S
	LDI  R30,0
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+@1)
	LDI  R31,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+@2)
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+@3)
	LDI  R@1,HIGH(@2+@3)
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+@1
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	LDS  R22,@0+@1+2
	LDS  R23,@0+@1+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@2,@0+@1
	.ENDM

	.MACRO __GETWRMN
	LDS  R@2,@0+@1
	LDS  R@3,@0+@1+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+@1
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	LDS  R24,@0+@1+2
	LDS  R25,@0+@1+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+@1,R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	STS  @0+@1+2,R22
	STS  @0+@1+3,R23
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+@1,R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+@1,R@2
	STS  @0+@1+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	ICALL
	.ENDM


	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	CALL __GETW1PF
	ICALL
	.ENDM


	.MACRO __CALL2EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMRDW
	ICALL
	.ENDM


	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __CLRD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R@1
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOV  R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOV  R30,R0
	.ENDM

	.CSEG
	.ORG 0

	.INCLUDE "Program.vec"
	.INCLUDE "Program.inc"

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,13
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(0x400)
	LDI  R25,HIGH(0x400)
	LDI  R26,0x60
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;STACK POINTER INITIALIZATION
	LDI  R30,LOW(0x45F)
	OUT  SPL,R30
	LDI  R30,HIGH(0x45F)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(0x160)
	LDI  R29,HIGH(0x160)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x160
;       1 /*****************************************************
;       2 Project : 汽车空调控制系统
;       3 Version : 1
;       4 Date    : 2005-12-13
;       5 Author  : Benny 
;       6 Blog    : http://bennylo.mcublog.com                         
;       7 Company : 509                             
;       8 Comments: 
;       9 
;      10 
;      11 Chip type           : ATmega16L
;      12 Program type        : Application
;      13 Clock frequency     : 8.000000 MHz
;      14 Memory model        : Small
;      15 External SRAM size  : 0
;      16 Data Stack size     : 256
;      17 *****************************************************/
;      18 
;      19 #include <mega16.h> 
;      20 #include <stdlib.h> 
;      21 #include "Pt1000Tab.h"

	.CSEG
;      22 #include "inc.h"
;      23 
;      24 #asm
;      25    .equ __lcd_port=0x18 ;PORTB
   .equ __lcd_port=0x18 ;PORTB
;      26 #endasm
;      27 #include <lcd.h>
;      28 
;      29 bit boolean;   
;      30 
;      31 uchar ventilator_state; 
;      32 uchar fan;  
;      33 uchar blow; 
;      34 uchar run_mode;
;      35 uchar temp;
;      36 uchar setting_value;
;      37 interrupt [TIM2_COMP] void timer2_comp_isr(void)
;      38 {
_timer2_comp_isr:
	ST   -Y,R30
	IN   R30,SREG
	ST   -Y,R30
;      39 //产生PWM,控制步进电机
;      40 if (fan==1)
	LDI  R30,LOW(1)
	CP   R30,R5
	BRNE _0x3
;      41 {
;      42    OCR2=64;
	LDI  R30,LOW(64)
	OUT  0x23,R30
;      43 }
;      44 else if(fan==2)
	RJMP _0x4
_0x3:
	LDI  R30,LOW(2)
	CP   R30,R5
	BRNE _0x5
;      45 {
;      46    OCR2=128;
	LDI  R30,LOW(128)
	OUT  0x23,R30
;      47 }
;      48 }
_0x5:
_0x4:
	LD   R30,Y+
	OUT  SREG,R30
	LD   R30,Y+
	RETI
;      49 
;      50 #define FIRST_ADC_INPUT 0   //第一通道
;      51 #define LAST_ADC_INPUT 1    //最后一通道 ，最大值为7,共8个通道
;      52 unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];

	.DSEG
_adc_data:
	.BYTE 0x4
;      53 #define ADC_VREF_TYPE 0x40
;      54 // ADC中断服务程序
;      55 // 自动扫描模拟量输入端口，
;      56 interrupt [ADC_INT] void adc_isr(void)
;      57 {

	.CSEG
_adc_isr:
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;      58 register static unsigned char input_index=0;

	.DSEG

	.CSEG
;      59 // 读取转换结果
;      60 adc_data[input_index]=ADCW;
	MOV  R30,R10
	CALL SUBOPT_0x0
	ADD  R26,R30
	ADC  R27,R31
	IN   R30,0x4
	IN   R31,0x4+1
	ST   X+,R30
	ST   X,R31
;      61 // 选择转换通道
;      62 if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
	INC  R10
	LDI  R30,LOW(1)
	CP   R30,R10
	BRSH _0x6
;      63    input_index=0;
	CLR  R10
;      64 ADMUX=(FIRST_ADC_INPUT|ADC_VREF_TYPE)+input_index;
_0x6:
	MOV  R30,R10
	SUBI R30,-LOW(64)
	OUT  0x7,R30
;      65 //启动AD转换
;      66 ADCSRA|=0x40;
	SBI  0x6,6
;      67 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	RETI
;      68 
;      69 
;      70 void main(void)
;      71 {
_main:
;      72 float current_temp;//保存当前温度
;      73 // Port A 初始化
;      74 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
;      75 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
;      76 PORTA=0x00;
	SBIW R28,4
;	current_temp -> Y+0
	LDI  R30,LOW(0)
	OUT  0x1B,R30
;      77 DDRA=0x00;
	OUT  0x1A,R30
;      78 
;      79 // Port B 初始化
;      80 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
;      81 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
;      82 PORTB=0x00;
	OUT  0x18,R30
;      83 DDRB=0x00;
	OUT  0x17,R30
;      84 
;      85 // Port C 初始化
;      86 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
;      87 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
;      88 PORTC=0x38;
	LDI  R30,LOW(56)
	OUT  0x15,R30
;      89 DDRC=0x38;
	OUT  0x14,R30
;      90 
;      91 // Port D 初始化
;      92 // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
;      93 // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
;      94 PORTD=0x1f;
	LDI  R30,LOW(31)
	OUT  0x12,R30
;      95 DDRD=0xF8;
	LDI  R30,LOW(248)
	OUT  0x11,R30
;      96 
;      97 
;      98 //用T2产生PWM,控制风向步进电机
;      99 // Timer/Counter 2 initialization
;     100 // Clock source: System Clock
;     101 // Clock value: Timer 2 Stopped
;     102 // Mode: Normal top=FFh
;     103 // OC2 output: Disconnected
;     104 ASSR=0x00;
	LDI  R30,LOW(0)
	OUT  0x22,R30
;     105 TCCR2=0x64;
	LDI  R30,LOW(100)
	OUT  0x25,R30
;     106 TCNT2=0x00;
	LDI  R30,LOW(0)
	OUT  0x24,R30
;     107 OCR2=0x00;
	OUT  0x23,R30
;     108 
;     109 
;     110 // Timer(s)/Counter(s) Interrupt(s) initialization
;     111 TIMSK=0x80;
	LDI  R30,LOW(128)
	OUT  0x39,R30
;     112 
;     113 // Analog Comparator initialization
;     114 // Analog Comparator: Off
;     115 // Analog Comparator Input Capture by Timer/Counter 1: Off
;     116 ACSR=0x80;
	OUT  0x8,R30
;     117 SFIOR=0x00;
	LDI  R30,LOW(0)
	OUT  0x30,R30
;     118 
;     119 // ADC initialization
;     120 // ADC Clock frequency: 125.000 kHz
;     121 // ADC Voltage Reference: AVCC pin
;     122 // ADC Auto Trigger Source: Free Running
;     123 ADMUX=FIRST_ADC_INPUT|ADC_VREF_TYPE;
	LDI  R30,LOW(64)
	OUT  0x7,R30
;     124 ADCSRA=0xEE;
	LDI  R30,LOW(238)
	OUT  0x6,R30
;     125 SFIOR&=0x1F;
	IN   R30,0x30
	ANDI R30,LOW(0x1F)
	OUT  0x30,R30
;     126 
;     127 // LCD module初始化
;     128 lcd_init(16);
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _lcd_init
;     129 
;     130 // 开启全局中断
;     131 #asm("sei")
	sei
;     132 dis_character(); 
	RCALL _dis_character
;     133 
;     134 setting_value=25; 
	LDI  R30,LOW(25)
	MOV  R9,R30
;     135 run_mode=0; 
	CLR  R7
;     136 ventilator_state=0;
	CLR  R4
;     137 lcd_gotoxy(12,3); 
	CALL SUBOPT_0x1
;     138 lcd_putsf("OFF");  
	__POINTW1FN _0,0
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     139 while (1)
_0x7:
;     140       {    
;     141         scan_key();  
	RCALL _scan_key
;     142         blow_mode();
	RCALL _blow_mode
;     143         display();
	RCALL _display
;     144         if(run_mode==2)
	LDI  R30,LOW(2)
	CP   R30,R7
	BRNE _0xA
;     145         {   
;     146             current_temp=Pt1000Tab[adc_data[0]];
	LDI  R26,LOW(_Pt1000Tab*2)
	LDI  R27,HIGH(_Pt1000Tab*2)
	LDS  R30,_adc_data
	LDS  R31,_adc_data+1
	CALL SUBOPT_0x2
	__PUTD1S 0
;     147             if (current_temp<setting_value)
	MOV  R30,R9
	__GETD2S 0
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	CALL __CMPF12
	BRSH _0xB
;     148             {       
;     149               stop_compressor(); 
	RCALL _stop_compressor
;     150               //lcd_gotoxy(10,1); 
;     151               //lcd_putsf("Blast");          
;     152             }
;     153             else
	RJMP _0xC
_0xB:
;     154             {
;     155               start_compressor();   
	RCALL _start_compressor
;     156               //lcd_gotoxy(10,1); 
;     157               //lcd_putsf("Cool");     
;     158             }        
_0xC:
;     159              
;     160        }
;     161 
;     162       }                         
_0xA:
	RJMP _0x7
;     163 }
	ADIW R28,4
_0xD:
	RJMP _0xD
;     164 void start_compressor(void){
_start_compressor:
;     165   //Start
;     166   PORTD|=0x18;
	CALL SUBOPT_0x3
;     167  
;     168   PORTD&=~0x10; 
	CBI  0x12,4
;     169  
;     170   PORTD|=0x18; 
	CALL SUBOPT_0x3
;     171 }
	RET
;     172 void stop_compressor(void){
_stop_compressor:
;     173   //Stop
;     174   PORTD|=0x18; 
	CALL SUBOPT_0x3
;     175   PORTD&=~0x08; 
	CBI  0x12,3
;     176   PORTD|=0x18; 
	CALL SUBOPT_0x3
;     177 
;     178 }
	RET
;     179 void start_ventilator(void) {
_start_ventilator:
;     180   PORTD|=0x40;//换气风机运行
	SBI  0x12,6
;     181 }
	RET
;     182 void stop_ventilator(void){
_stop_ventilator:
;     183   PORTD&=~0x40;//换气风机停止
	CBI  0x12,6
;     184 } 
	RET
;     185 void start_evaporator_fan(void){
_start_evaporator_fan:
;     186   PORTD|=0x20;//蒸发器风机运行
	SBI  0x12,5
;     187 }
	RET
;     188 void stop_evaporator_fan(void){
_stop_evaporator_fan:
;     189   PORTD&=~0x20;//蒸发器风机停止
	CBI  0x12,5
;     190 } 
	RET
;     191 /*-----------------------------------------------键盘扫描-------------------------------------------
;     192 
;     193 
;     194 -----------------------------------------------键盘扫描-------------------------------------------*/
;     195 void scan_key(void){
_scan_key:
;     196 /* 
;     197    K11    K12     K13
;     198    K21    K22     K23 
;     199    K31    K32     K33 
;     200  */
;     201  
;     202  //K13 K23 K33  
;     203 PORTC&=~0x20; 
	CBI  0x15,5
;     204 if((PIND&0x07)!=0x07) 
	CALL SUBOPT_0x4
	BRNE PC+3
	JMP _0xE
;     205 {   
;     206    delay();
	RCALL _delay
;     207    if((PIND&0x07)!=0x07) 
	CALL SUBOPT_0x4
	BRNE PC+3
	JMP _0xF
;     208    {    
;     209         if((PIND&0x07)==0x06) 
	CALL SUBOPT_0x5
	BRNE _0x10
;     210         {  //Key 3-3  
;     211           //uchar key_num[]="K33"; 
;     212           while((PIND&0x07)==0x06);
_0x11:
	CALL SUBOPT_0x5
	BREQ _0x11
;     213           switch(blow)
	MOV  R30,R6
;     214           {
;     215             case 0:
	CPI  R30,0
	BRNE _0x17
;     216             {
;     217               blow=1;
	LDI  R30,LOW(1)
	CALL SUBOPT_0x6
;     218               lcd_gotoxy(10,2); 
;     219               lcd_putsf("Mode0");
	__POINTW1FN _0,4
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     220               break;        
	RJMP _0x16
;     221             }
;     222             case 1:
_0x17:
	CPI  R30,LOW(0x1)
	BRNE _0x18
;     223             {
;     224               blow=2; 
	LDI  R30,LOW(2)
	CALL SUBOPT_0x6
;     225               lcd_gotoxy(10,2); 
;     226               lcd_putsf("Mode1");
	__POINTW1FN _0,10
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     227               break;        
	RJMP _0x16
;     228             } 
;     229             case 2:
_0x18:
	CPI  R30,LOW(0x2)
	BRNE _0x19
;     230             {
;     231               blow=3;
	LDI  R30,LOW(3)
	CALL SUBOPT_0x6
;     232               lcd_gotoxy(10,2); 
;     233               lcd_putsf("Mode2");              
	__POINTW1FN _0,16
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     234               break;        
	RJMP _0x16
;     235             } 
;     236             case 3:
_0x19:
	CPI  R30,LOW(0x3)
	BRNE _0x16
;     237             {
;     238               blow=0;
	CLR  R6
;     239               lcd_gotoxy(10,2); 
	CALL SUBOPT_0x7
;     240               lcd_putsf("Mode3");
	__POINTW1FN _0,22
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     241               break;        
;     242             }                                        
;     243           }
_0x16:
;     244           return;
	RET
;     245         }
;     246         if((PIND&0x07)==0x05) 
_0x10:
	CALL SUBOPT_0x8
	BRNE _0x1B
;     247         {  //Key 2-3   
;     248            //uchar key_num[]="K23";           
;     249            while((PIND&0x07)==0x05); 
_0x1C:
	CALL SUBOPT_0x8
	BREQ _0x1C
;     250                        
;     251            if(ventilator_state==1)
	LDI  R30,LOW(1)
	CP   R30,R4
	BRNE _0x1F
;     252            {
;     253               ventilator_state=0;
	CLR  R4
;     254               stop_ventilator();
	CALL _stop_ventilator
;     255               lcd_gotoxy(12,3); 
	CALL SUBOPT_0x1
;     256               lcd_putsf("OFF");             
	__POINTW1FN _0,0
	RJMP _0x6D
;     257            }
;     258            else
_0x1F:
;     259            {
;     260               ventilator_state=1;
	LDI  R30,LOW(1)
	MOV  R4,R30
;     261               start_ventilator(); 
	CALL _start_ventilator
;     262               lcd_gotoxy(12,3); 
	CALL SUBOPT_0x1
;     263               lcd_putsf("Run");                
	__POINTW1FN _0,28
_0x6D:
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     264            }           
;     265            return;
	RET
;     266         }
;     267         if((PIND&0x07)==0x03) 
_0x1B:
	CALL SUBOPT_0x9
	BRNE _0x21
;     268         {  //Key 1-3           
;     269           //uchar key_num[]="K13"; 
;     270           while((PIND&0x07)==0x03); 
_0x22:
	CALL SUBOPT_0x9
	BREQ _0x22
;     271           switch(run_mode)
	MOV  R30,R7
;     272           {
;     273             case 2:
	CPI  R30,LOW(0x2)
	BRNE _0x28
;     274             { //关闭模式 
;     275               stop_evaporator_fan();
	CALL _stop_evaporator_fan
;     276               stop_compressor();  
	CALL SUBOPT_0xA
;     277               lcd_gotoxy(10,1); 
;     278               lcd_putsf("OFF  "); 
	__POINTW1FN _0,32
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     279               run_mode=0;
	CLR  R7
;     280               break; 
	RJMP _0x27
;     281             }
;     282             case 0:
_0x28:
	CPI  R30,0
	BRNE _0x29
;     283             {
;     284               //送风模式  
;     285               start_evaporator_fan();
	CALL _start_evaporator_fan
;     286               stop_compressor(); 
	CALL SUBOPT_0xA
;     287               lcd_gotoxy(10,1); 
;     288               lcd_putsf("Blast");
	__POINTW1FN _0,38
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     289               run_mode=1; 
	LDI  R30,LOW(1)
	MOV  R7,R30
;     290               break;       
	RJMP _0x27
;     291             }        
;     292             case 1:
_0x29:
	CPI  R30,LOW(0x1)
	BRNE _0x27
;     293             {
;     294               //制冷模式
;     295               start_evaporator_fan();
	CALL _start_evaporator_fan
;     296               start_compressor(); 
	CALL _start_compressor
;     297               lcd_gotoxy(10,1); 
	CALL SUBOPT_0xB
;     298               lcd_putsf("Cool ");              
	__POINTW1FN _0,44
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     299               run_mode=2;
	LDI  R30,LOW(2)
	MOV  R7,R30
;     300               break;           
;     301             }
;     302           } 
_0x27:
;     303  
;     304           return;
	RET
;     305         }   
;     306    }      
_0x21:
;     307 
;     308 }
_0xF:
;     309 
;     310 PORTC|=0x20;
_0xE:
	SBI  0x15,5
;     311 
;     312  //K12 K22 K32
;     313 PORTC&=~0x10; 
	CBI  0x15,4
;     314 if((PIND&0x07)!=0x07)
	CALL SUBOPT_0x4
	BREQ _0x2B
;     315 {
;     316         if((PIND&0x07)==0x06) 
	CALL SUBOPT_0x5
	BRNE _0x2C
;     317         {  //Key 3-2
;     318            //uchar key_num[]="K32";
;     319            while((PIND&0x07)==0x06);
_0x2D:
	CALL SUBOPT_0x5
	BREQ _0x2D
;     320            return;
	RET
;     321         }
;     322         if((PIND&0x07)==0x05) 
_0x2C:
	CALL SUBOPT_0x8
	BRNE _0x30
;     323         {  //Key 2-2
;     324            //uchar key_num[]="K22";
;     325            while((PIND&0x07)==0x05);
_0x31:
	CALL SUBOPT_0x8
	BREQ _0x31
;     326            if (setting_value<=18)
	LDI  R30,LOW(18)
	CP   R30,R9
	BRLO _0x34
;     327            {
;     328               setting_value=18;
	MOV  R9,R30
;     329            } 
;     330            else
	RJMP _0x35
_0x34:
;     331            {
;     332               setting_value--;
	DEC  R9
;     333            }
_0x35:
;     334  
;     335            return;
	RET
;     336         }
;     337         if((PIND&0x07)==0x03) 
_0x30:
	CALL SUBOPT_0x9
	BRNE _0x36
;     338         {  //Key 1-2
;     339            //uchar key_num[]="K12";
;     340            while((PIND&0x07)==0x03);
_0x37:
	CALL SUBOPT_0x9
	BREQ _0x37
;     341            if (setting_value>=28)
	LDI  R30,LOW(28)
	CP   R9,R30
	BRLO _0x3A
;     342            {
;     343               setting_value=28;
	MOV  R9,R30
;     344            } 
;     345            else
	RJMP _0x3B
_0x3A:
;     346            {
;     347               setting_value++;
	INC  R9
;     348            }
_0x3B:
;     349            return;
	RET
;     350         }
;     351 }
_0x36:
;     352 PORTC|=0x10;
_0x2B:
	SBI  0x15,4
;     353  //K11 K21 K31
;     354 PORTC&=~0x08; 
	CBI  0x15,3
;     355 if((PIND&0x07)!=0x07) 
	CALL SUBOPT_0x4
	BREQ _0x3C
;     356 {   
;     357    delay();
	RCALL _delay
;     358    if((PIND&0x07)!=0x07) 
	CALL SUBOPT_0x4
	BREQ _0x3D
;     359    {    
;     360         if((PIND&0x07)==0x06) 
	CALL SUBOPT_0x5
	BRNE _0x3E
;     361         {  //Key 3-1  
;     362           //uchar key_num[]="K31"; 
;     363           while((PIND&0x07)==0x06);
_0x3F:
	CALL SUBOPT_0x5
	BREQ _0x3F
;     364           return;
	RET
;     365         }
;     366         if((PIND&0x07)==0x05) 
_0x3E:
	CALL SUBOPT_0x8
	BRNE _0x42
;     367         {  //Key 2-1   
;     368            //uchar key_num[]="K21";           
;     369            while((PIND&0x07)==0x05); 
_0x43:
	CALL SUBOPT_0x8
	BREQ _0x43
;     370            return;
	RET
;     371         }
;     372         if((PIND&0x07)==0x03) 
_0x42:
	CALL SUBOPT_0x9
	BRNE _0x46
;     373         {  //Key 1-1           
;     374            //uchar key_num[]="K11"; 
;     375            while((PIND&0x07)==0x03);  
_0x47:
	CALL SUBOPT_0x9
	BREQ _0x47
;     376           return;
	RET
;     377         }   
;     378    }      
_0x46:
;     379 
;     380 }
_0x3D:
;     381 PORTC|=0x08;
_0x3C:
	SBI  0x15,3
;     382 
;     383 }
	RET
;     384 
;     385 void blow_mode(void){
_blow_mode:
;     386 switch(blow)
	MOV  R30,R6
;     387 {
;     388   case 0:
	CPI  R30,0
	BRNE _0x4D
;     389   { 
;     390     fan=0;
	CLR  R5
;     391     break;
	RJMP _0x4C
;     392   }     
;     393   case 1:
_0x4D:
	CPI  R30,LOW(0x1)
	BRNE _0x4E
;     394   {
;     395     fan=1;
	LDI  R30,LOW(1)
	MOV  R5,R30
;     396     break;
	RJMP _0x4C
;     397   } 
;     398   case 2:
_0x4E:
	CPI  R30,LOW(0x2)
	BRNE _0x4F
;     399   { 
;     400     fan=2;
	LDI  R30,LOW(2)
	MOV  R5,R30
;     401     break;
	RJMP _0x4C
;     402   }  
;     403   case 3:
_0x4F:
	CPI  R30,LOW(0x3)
	BRNE _0x4C
;     404   {
;     405     temp++;
	INC  R8
;     406     if(temp==5)
	LDI  R30,LOW(5)
	CP   R30,R8
	BRNE _0x51
;     407     {
;     408       boolean=~boolean;          
	LDI  R30,LOW(1)
	EOR  R2,R30
;     409     }
;     410     if (boolean==1)
_0x51:
	SBRS R2,0
	RJMP _0x52
;     411     {
;     412       fan=1;
	LDI  R30,LOW(1)
	RJMP _0x6E
;     413     }
;     414     else
_0x52:
;     415     {
;     416       fan=2;
	LDI  R30,LOW(2)
_0x6E:
	MOV  R5,R30
;     417     }  
;     418     break;
;     419   }      
;     420 }
_0x4C:
;     421 } 
	RET
;     422 void delay(void){
_delay:
;     423 uint i,j;
;     424 for (i=0;i<10;i++)
	CALL __SAVELOCR4
;	i -> R16,R17
;	j -> R18,R19
	__GETWRN 16,17,0
_0x55:
	__CPWRN 16,17,10
	BRSH _0x56
;     425   { 
;     426   for (j=0;j<10;j++)
	__GETWRN 18,19,0
_0x58:
	__CPWRN 18,19,10
	BRSH _0x59
;     427      {}
	__ADDWRN 18,19,1
	RJMP _0x58
_0x59:
;     428   }
	__ADDWRN 16,17,1
	RJMP _0x55
_0x56:
;     429 }
	CALL __LOADLOCR4
	ADIW R28,4
	RET
;     430 void dis(uchar channel,uchar x,uchar y){
_dis:
;     431    uchar arr[5];  
;     432    ftoa(Pt1000Tab[adc_data[channel]],1,arr);
	SBIW R28,5
;	channel -> Y+7
;	x -> Y+6
;	y -> Y+5
;	arr -> Y+0
	LDI  R30,LOW(_Pt1000Tab*2)
	LDI  R31,HIGH(_Pt1000Tab*2)
	PUSH R31
	PUSH R30
	LDD  R30,Y+7
	CALL SUBOPT_0x0
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETW1P
	POP  R26
	POP  R27
	CALL SUBOPT_0x2
	CALL __PUTPARD1
	LDI  R30,LOW(1)
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,5
	ST   -Y,R31
	ST   -Y,R30
	CALL _ftoa
;     433    //itoa(adc_data[channel],arr);
;     434    lcd_gotoxy(x,y); 
	LDD  R30,Y+6
	ST   -Y,R30
	LDD  R30,Y+6
	CALL SUBOPT_0xC
;     435    lcd_puts(arr); 
;     436    //lcd_putchar(0);   
;     437 } 
	ADIW R28,8
	RET
;     438 void display(void){  
_display:
;     439    uchar setting[3];
;     440    dis(0,2,0);
	SBIW R28,3
;	setting -> Y+0
	CALL SUBOPT_0xD
	LDI  R30,LOW(0)
	ST   -Y,R30
	CALL _dis
;     441    itoa(setting_value,setting);
	MOV  R30,R9
	LDI  R31,0
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,2
	ST   -Y,R31
	ST   -Y,R30
	RCALL _itoa
;     442    lcd_gotoxy(12,0); 
	LDI  R30,LOW(12)
	ST   -Y,R30
	LDI  R30,LOW(0)
	CALL SUBOPT_0xC
;     443    lcd_puts(setting); 
;     444 }
	ADIW R28,3
	RET
;     445 void dis_character(void){   
_dis_character:
;     446    lcd_gotoxy(0,0); 
	LDI  R30,LOW(0)
	CALL SUBOPT_0xE
;     447    lcd_putsf("T:"); 
	__POINTW1FN _0,50
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     448    
;     449    lcd_gotoxy(8,0); 
	LDI  R30,LOW(8)
	CALL SUBOPT_0xE
;     450    lcd_putsf("Set:");    
	__POINTW1FN _0,53
	CALL SUBOPT_0xF
;     451    
;     452    lcd_gotoxy(0,1); 
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _lcd_gotoxy
;     453    lcd_putsf("Run  Mode:");  
	__POINTW1FN _0,58
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     454    lcd_gotoxy(10,1); 
	CALL SUBOPT_0xB
;     455    lcd_putsf("OFF  "); 
	__POINTW1FN _0,32
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     456 
;     457    lcd_gotoxy(0,2); 
	CALL SUBOPT_0xD
	CALL _lcd_gotoxy
;     458    lcd_putsf("Blow Mode:"); 
	__POINTW1FN _0,69
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     459     
;     460    lcd_gotoxy(10,2); 
	CALL SUBOPT_0x7
;     461    lcd_putsf("Mode0");  
	__POINTW1FN _0,4
	CALL SUBOPT_0xF
;     462    
;     463    lcd_gotoxy(0,3); 
	LDI  R30,LOW(3)
	ST   -Y,R30
	CALL _lcd_gotoxy
;     464    lcd_putsf("Ventilator:");     
	__POINTW1FN _0,80
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
;     465 }
	RET
;     466 
;     467 
;     468 
;     469 
;     470 
;     471 
;     472 
;     473 
;     474 

_itoa:
    ld   r26,y+
    ld   r27,y+
    ld   r30,y+
    ld   r31,y+
    adiw r30,0
    brpl __itoa0
    com  r30
    com  r31
    adiw r30,1
    ldi  r22,'-'
    st   x+,r22
__itoa0:
    clt
    ldi  r24,low(10000)
    ldi  r25,high(10000)
    rcall __itoa1
    ldi  r24,low(1000)
    ldi  r25,high(1000)
    rcall __itoa1
    ldi  r24,100
    clr  r25
    rcall __itoa1
    ldi  r24,10
    rcall __itoa1
    mov  r22,r30
    rcall __itoa5
    clr  r22
    st   x,r22
    ret
__itoa1:
    clr	 r22
__itoa2:
    cp   r30,r24
    cpc  r31,r25
    brlo __itoa3
    inc  r22
    sub  r30,r24
    sbc  r31,r25
    brne __itoa2
__itoa3:
    tst  r22
    brne __itoa4
    brts __itoa5
    ret
__itoa4:
    set
__itoa5:
    subi r22,-0x30
    st   x+,r22
    ret
_ftoa:
	SBIW R28,4
	ST   -Y,R17
	ST   -Y,R16
	__GETD2S 9
	CALL __CPD20
	BRGE _0x5A
	__GETD1S 9
	CALL __ANEGF1
	__PUTD1S 9
	CALL SUBOPT_0x10
	LDI  R30,LOW(45)
	ST   X,R30
_0x5A:
	LDD  R26,Y+8
	LDI  R30,LOW(5)
	CP   R30,R26
	BRSH _0x5B
	STD  Y+8,R30
_0x5B:
	LDI  R26,LOW(__fround_G2*2)
	LDI  R27,HIGH(__fround_G2*2)
	LDD  R30,Y+8
	LDI  R31,0
	CALL SUBOPT_0x2
	__GETD2S 9
	CALL __ADDF12
	__PUTD1S 9
	LDI  R16,LOW(0)
	__GETD1N 0x3F800000
	__PUTD1S 2
_0x5C:
	__GETD1S 2
	__GETD2S 9
	CALL __CMPF12
	BRLO _0x5E
	__GETD2S 2
	__GETD1N 0x41200000
	CALL __MULF12
	__PUTD1S 2
	SUBI R16,-LOW(1)
	RJMP _0x5C
_0x5E:
	CPI  R16,0
	BRNE _0x5F
	CALL SUBOPT_0x10
	LDI  R30,LOW(48)
	ST   X,R30
	RJMP _0x60
_0x5F:
_0x61:
	MOV  R30,R16
	SUBI R16,1
	CPI  R30,0
	BREQ _0x63
	__GETD2S 2
	__GETD1N 0x41200000
	CALL __DIVF21
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL __PUTPARD1
	CALL _floor
	__PUTD1S 2
	__GETD2S 9
	CALL __DIVF21
	CALL __CFD1
	MOV  R17,R30
	CALL SUBOPT_0x10
	CALL SUBOPT_0x11
	__GETD2S 2
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	CALL __MULF12
	__GETD2S 9
	CALL SUBOPT_0x12
	RJMP _0x61
_0x63:
_0x60:
	LDD  R30,Y+8
	CPI  R30,0
	BRNE _0x64
	CALL SUBOPT_0x13
	RJMP _0x6C
_0x64:
	CALL SUBOPT_0x10
	LDI  R30,LOW(46)
	ST   X,R30
_0x65:
	LDD  R30,Y+8
	SUBI R30,LOW(1)
	STD  Y+8,R30
	SUBI R30,-LOW(1)
	BREQ _0x67
	__GETD2S 9
	__GETD1N 0x41200000
	CALL __MULF12
	__PUTD1S 9
	CALL __CFD1
	MOV  R17,R30
	CALL SUBOPT_0x10
	CALL SUBOPT_0x11
	__GETD2S 9
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	CALL SUBOPT_0x12
	RJMP _0x65
_0x67:
	CALL SUBOPT_0x13
_0x6C:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,13
	RET
    .equ __lcd_direction=__lcd_port-1
    .equ __lcd_pin=__lcd_port-2
    .equ __lcd_rs=0
    .equ __lcd_rd=1
    .equ __lcd_enable=2
    .equ __lcd_busy_flag=7

	.DSEG
__base_y_G3:
	.BYTE 0x4

	.CSEG
__lcd_delay_G3:
    ldi   r31,15
__lcd_delay0:
    dec   r31
    brne  __lcd_delay0
	RET
__lcd_ready:
    in    r26,__lcd_direction
    andi  r26,0xf                 ;set as input
    out   __lcd_direction,r26
    sbi   __lcd_port,__lcd_rd     ;RD=1
    cbi   __lcd_port,__lcd_rs     ;RS=0
__lcd_busy:
	CALL __lcd_delay_G3
    sbi   __lcd_port,__lcd_enable ;EN=1
	CALL __lcd_delay_G3
    in    r26,__lcd_pin
    cbi   __lcd_port,__lcd_enable ;EN=0
	CALL __lcd_delay_G3
    sbi   __lcd_port,__lcd_enable ;EN=1
	CALL __lcd_delay_G3
    cbi   __lcd_port,__lcd_enable ;EN=0
    sbrc  r26,__lcd_busy_flag
    rjmp  __lcd_busy
	RET
__lcd_write_nibble_G3:
    andi  r26,0xf0
    or    r26,r27
    out   __lcd_port,r26          ;write
    sbi   __lcd_port,__lcd_enable ;EN=1
	CALL __lcd_delay_G3
    cbi   __lcd_port,__lcd_enable ;EN=0
	CALL __lcd_delay_G3
	RET
__lcd_write_data:
    cbi  __lcd_port,__lcd_rd 	  ;RD=0
    in    r26,__lcd_direction
    ori   r26,0xf0 | (1<<__lcd_rs) | (1<<__lcd_rd) | (1<<__lcd_enable) ;set as output    
    out   __lcd_direction,r26
    in    r27,__lcd_port
    andi  r27,0xf
    ld    r26,y
	CALL __lcd_write_nibble_G3
    ld    r26,y
    swap  r26
	CALL __lcd_write_nibble_G3
    sbi   __lcd_port,__lcd_rd     ;RD=1
	ADIW R28,1
	RET
__lcd_read_nibble_G3:
    sbi   __lcd_port,__lcd_enable ;EN=1
	CALL __lcd_delay_G3
    in    r30,__lcd_pin           ;read
    cbi   __lcd_port,__lcd_enable ;EN=0
	CALL __lcd_delay_G3
    andi  r30,0xf0
	RET
_lcd_read_byte0_G3:
	CALL __lcd_delay_G3
	CALL __lcd_read_nibble_G3
    mov   r26,r30
	CALL __lcd_read_nibble_G3
    cbi   __lcd_port,__lcd_rd     ;RD=0
    swap  r30
    or    r30,r26
	RET
_lcd_gotoxy:
	CALL __lcd_ready
	LD   R30,Y
	LDI  R31,0
	SUBI R30,LOW(-__base_y_G3)
	SBCI R31,HIGH(-__base_y_G3)
	LD   R30,Z
	LDD  R26,Y+1
	ADD  R30,R26
	ST   -Y,R30
	CALL __lcd_write_data
	LDD  R11,Y+1
	LDD  R12,Y+0
	ADIW R28,2
	RET
_lcd_clear:
	CALL __lcd_ready
	LDI  R30,LOW(2)
	CALL SUBOPT_0x14
	LDI  R30,LOW(12)
	CALL SUBOPT_0x14
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL __lcd_write_data
	LDI  R30,LOW(0)
	MOV  R12,R30
	MOV  R11,R30
	RET
_lcd_putchar:
    push r30
    push r31
    ld   r26,y
    set
    cpi  r26,10
    breq __lcd_putchar1
    clt
	INC  R11
	CP   R13,R11
	BRSH _0x69
	__lcd_putchar1:
	INC  R12
	LDI  R30,LOW(0)
	ST   -Y,R30
	ST   -Y,R12
	CALL _lcd_gotoxy
	brts __lcd_putchar0
_0x69:
    rcall __lcd_ready
    sbi  __lcd_port,__lcd_rs ;RS=1
    ld   r26,y
    st   -y,r26
    rcall __lcd_write_data
__lcd_putchar0:
    pop  r31
    pop  r30
	ADIW R28,1
	RET
_lcd_puts:
    ldd  r31,y+1
    ld   r30,y
__lcd_puts0:
    ld   r26,z+
    tst  r26
    breq __lcd_puts1
    st   -y,r26    
    rcall _lcd_putchar
    rjmp __lcd_puts0
__lcd_puts1:
	ADIW R28,2
	RET
_lcd_putsf:
    ld   r30,y
    ldd  r31,y+1
__lcd_putsf0:
	lpm
    tst  r0
    breq __lcd_putsf1
    adiw r30,1
    st   -y,r0
    rcall _lcd_putchar
    rjmp __lcd_putsf0
__lcd_putsf1:
	ADIW R28,2
	RET
__long_delay_G3:
    clr   r26
    clr   r27
__long_delay0:
    sbiw  r26,1         ;2 cycles
    brne  __long_delay0 ;2 cycles
	RET
__lcd_init_write_G3:
    cbi  __lcd_port,__lcd_rd 	  ;RD=0
    in    r26,__lcd_direction
    ori   r26,0xf7                ;set as output
    out   __lcd_direction,r26
    in    r27,__lcd_port
    andi  r27,0xf
    ld    r26,y
	CALL __lcd_write_nibble_G3
    sbi   __lcd_port,__lcd_rd     ;RD=1
	ADIW R28,1
	RET
_lcd_init:
    cbi   __lcd_port,__lcd_enable ;EN=0
    cbi   __lcd_port,__lcd_rs     ;RS=0
	LDD  R13,Y+0
	LD   R30,Y
	SUBI R30,-LOW(128)
	__PUTB1MN __base_y_G3,2
	LD   R30,Y
	SUBI R30,-LOW(192)
	__PUTB1MN __base_y_G3,3
	CALL SUBOPT_0x15
	CALL SUBOPT_0x15
	CALL SUBOPT_0x15
	CALL __long_delay_G3
	LDI  R30,LOW(32)
	ST   -Y,R30
	CALL __lcd_init_write_G3
	CALL __long_delay_G3
	LDI  R30,LOW(40)
	CALL SUBOPT_0x16
	LDI  R30,LOW(4)
	CALL SUBOPT_0x16
	LDI  R30,LOW(133)
	CALL SUBOPT_0x16
    in    r26,__lcd_direction
    andi  r26,0xf                 ;set as input
    out   __lcd_direction,r26
    sbi   __lcd_port,__lcd_rd     ;RD=1
	CALL _lcd_read_byte0_G3
	CPI  R30,LOW(0x5)
	BREQ _0x6A
	LDI  R30,LOW(0)
	RJMP _0x6B
_0x6A:
	CALL __lcd_ready
	LDI  R30,LOW(6)
	ST   -Y,R30
	CALL __lcd_write_data
	CALL _lcd_clear
	LDI  R30,LOW(1)
_0x6B:
	ADIW R28,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0x0:
	LDI  R26,LOW(_adc_data)
	LDI  R27,HIGH(_adc_data)
	LDI  R31,0
	LSL  R30
	ROL  R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES
SUBOPT_0x1:
	LDI  R30,LOW(12)
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	JMP  _lcd_gotoxy

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES
SUBOPT_0x2:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES
SUBOPT_0x3:
	IN   R30,0x12
	ORI  R30,LOW(0x18)
	OUT  0x12,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES
SUBOPT_0x4:
	IN   R30,0x10
	ANDI R30,LOW(0x7)
	CPI  R30,LOW(0x7)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES
SUBOPT_0x5:
	IN   R30,0x10
	ANDI R30,LOW(0x7)
	CPI  R30,LOW(0x6)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES
SUBOPT_0x6:
	MOV  R6,R30
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _lcd_gotoxy

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0x7:
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _lcd_gotoxy

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES
SUBOPT_0x8:
	IN   R30,0x10
	ANDI R30,LOW(0x7)
	CPI  R30,LOW(0x5)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES
SUBOPT_0x9:
	IN   R30,0x10
	ANDI R30,LOW(0x7)
	CPI  R30,LOW(0x3)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0xA:
	CALL _stop_compressor
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _lcd_gotoxy

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0xB:
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _lcd_gotoxy

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0xC:
	ST   -Y,R30
	CALL _lcd_gotoxy
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	JMP  _lcd_puts

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0xD:
	LDI  R30,LOW(0)
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0xE:
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _lcd_gotoxy

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0xF:
	ST   -Y,R31
	ST   -Y,R30
	CALL _lcd_putsf
	LDI  R30,LOW(0)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES
SUBOPT_0x10:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADIW R26,1
	STD  Y+6,R26
	STD  Y+6+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0x11:
	MOV  R30,R17
	SUBI R30,-LOW(48)
	ST   X,R30
	MOV  R30,R17
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0x12:
	CALL __SWAPD12
	CALL __SUBF12
	__PUTD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0x13:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES
SUBOPT_0x14:
	ST   -Y,R30
	CALL __lcd_write_data
	JMP  __lcd_ready

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES
SUBOPT_0x15:
	CALL __long_delay_G3
	LDI  R30,LOW(48)
	ST   -Y,R30
	JMP  __lcd_init_write_G3

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES
SUBOPT_0x16:
	ST   -Y,R30
	CALL __lcd_write_data
	JMP  __long_delay_G3

__ftrunc:
	ldd  r23,y+3
	ldd  r22,y+2
	ldd  r31,y+1
	ld   r30,y
	bst  r23,7
	lsl  r23
	sbrc r22,7
	sbr  r23,1
	mov  r25,r23
	subi r25,0x7e
	breq __ftrunc0
	brcs __ftrunc0
	cpi  r25,24
	brsh __ftrunc1
	clr  r26
	clr  r27
	clr  r24
__ftrunc2:
	sec
	ror  r24
	ror  r27
	ror  r26
	dec  r25
	brne __ftrunc2
	and  r30,r26
	and  r31,r27
	and  r22,r24
	rjmp __ftrunc1
__ftrunc0:
	clt
	clr  r23
	clr  r30
	clr  r31
	clr  r22
__ftrunc1:
	cbr  r22,0x80
	lsr  r23
	brcc __ftrunc3
	sbr  r22,0x80
__ftrunc3:
	bld  r23,7
	ld   r26,y+
	ld   r27,y+
	ld   r24,y+
	ld   r25,y+
	cp   r30,r26
	cpc  r31,r27
	cpc  r22,r24
	cpc  r23,r25
	bst  r25,7
	ret

_floor:
	rcall __ftrunc
	brne __floor1
__floor0:
	ret
__floor1:
	brtc __floor0
	ldi  r25,0xbf

__addfc:
	clr  r26
	clr  r27
	ldi  r24,0x80
	rjmp __addf12

__ANEGD1:
	COM  R30
	COM  R31
	COM  R22
	COM  R23
	SUBI R30,-1
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__CBD1:
	MOV  R31,R30
	ADD  R31,R31
	SBC  R31,R31
	MOV  R22,R31
	MOV  R23,R31
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETD1PF:
	LPM  R0,Z+
	LPM  R1,Z+
	LPM  R22,Z+
	LPM  R23,Z
	MOVW R30,R0
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__SWAPD12:
	MOV  R1,R24
	MOV  R24,R22
	MOV  R22,R1
	MOV  R1,R25
	MOV  R25,R23
	MOV  R23,R1

__SWAPW12:
	MOV  R1,R27
	MOV  R27,R31
	MOV  R31,R1

__SWAPB12:
	MOV  R1,R26
	MOV  R26,R30
	MOV  R30,R1
	RET

__ANEGF1:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __ANEGF10
	SUBI R23,0x80
__ANEGF10:
	RET

__REPACK:
	LDI  R21,0x80
	EOR  R21,R23
	BRNE __REPACK0
	PUSH R21
	RJMP __ZERORES
__REPACK0:
	CPI  R21,0xFF
	BREQ __REPACK1
	LSL  R22
	LSL  R0
	ROR  R21
	ROR  R22
	MOV  R23,R21
	RET
__REPACK1:
	PUSH R21
	TST  R0
	BRMI __REPACK2
	RJMP __MAXRES
__REPACK2:
	RJMP __MINRES

__UNPACK:
	LDI  R21,0x80
	MOV  R1,R25
	AND  R1,R21
	LSL  R24
	ROL  R25
	EOR  R25,R21
	LSL  R21
	ROR  R24

__UNPACK1:
	LDI  R21,0x80
	MOV  R0,R23
	AND  R0,R21
	LSL  R22
	ROL  R23
	EOR  R23,R21
	LSL  R21
	ROR  R22
	RET

__CFD1:
	PUSH R21
	RCALL __UNPACK1
	CPI  R23,0x80
	BRLO __CFD10
	CPI  R23,0xFF
	BRCC __CFD10
	RJMP __ZERORES
__CFD10:
	LDI  R21,22
	SUB  R21,R23
	BRPL __CFD11
	NEG  R21
	CPI  R21,8
	BRLO __CFD17
	SER  R30
	SER  R31
	SER  R22
	LDI  R23,0x7F
	RJMP __CFD15
__CFD17:
	CLR  R23
	TST  R21
	BREQ __CFD15
__CFD18:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	DEC  R21
	BRNE __CFD18
	RJMP __CFD15
__CFD11:
	CLR  R23
__CFD12:
	CPI  R21,8
	BRLO __CFD13
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R23
	SUBI R21,8
	RJMP __CFD12
__CFD13:
	TST  R21
	BREQ __CFD15
__CFD14:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R21
	BRNE __CFD14
__CFD15:
	TST  R0
	BRPL __CFD16
	RCALL __ANEGD1
__CFD16:
	POP  R21
	RET

__CDF1U:
	SET
	RJMP __CDF1U0
__CDF1:
	CLT
__CDF1U0:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __CDF10
	CLR  R0
	BRTS __CDF11
	TST  R23
	BRPL __CDF11
	COM  R0
	RCALL __ANEGD1
__CDF11:
	MOV  R1,R23
	LDI  R23,30
	TST  R1
__CDF12:
	BRMI __CDF13
	DEC  R23
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R1
	RJMP __CDF12
__CDF13:
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R1
	PUSH R21
	RCALL __REPACK
	POP  R21
__CDF10:
	RET

__SWAPACC:
	MOV  R21,R30
	MOV  R30,R26
	MOV  R26,R21
	MOV  R21,R31
	MOV  R31,R27
	MOV  R27,R21
	MOV  R21,R22
	MOV  R22,R24
	MOV  R24,R21
	MOV  R21,R23
	MOV  R23,R25
	MOV  R25,R21
	MOV  R21,R0
	MOV  R0,R1
	MOV  R1,R21
	RET

__UADD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	RET

__NEGMAN1:
	COM  R30
	COM  R31
	COM  R22
	SUBI R30,-1
	SBCI R31,-1
	SBCI R22,-1
	RET

__SUBF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129
	LDI  R21,0x80
	EOR  R1,R21

	RJMP __ADDF120

__ADDF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129

__ADDF120:
	CPI  R23,0x80
	BREQ __ADDF128
__ADDF121:
	MOV  R21,R23
	SUB  R21,R25
	BRVS __ADDF129
	BRPL __ADDF122
	RCALL __SWAPACC
	RJMP __ADDF121
__ADDF122:
	CPI  R21,24
	BRLO __ADDF123
	CLR  R26
	CLR  R27
	CLR  R24
__ADDF123:
	CPI  R21,8
	BRLO __ADDF124
	MOV  R26,R27
	MOV  R27,R24
	CLR  R24
	SUBI R21,8
	RJMP __ADDF123
__ADDF124:
	TST  R21
	BREQ __ADDF126
__ADDF125:
	LSR  R24
	ROR  R27
	ROR  R26
	DEC  R21
	BRNE __ADDF125
__ADDF126:
	MOV  R21,R0
	EOR  R21,R1
	BRMI __ADDF127
	RCALL __UADD12
	BRCC __ADDF129
	ROR  R22
	ROR  R31
	ROR  R30
	INC  R23
	BRVC __ADDF129
	RJMP __MAXRES
__ADDF128:
	RCALL __SWAPACC
__ADDF129:
	RCALL __REPACK
	POP  R21
	RET
__ADDF127:
	SUB  R30,R26
	SBC  R31,R27
	SBC  R22,R24
	BREQ __ZERORES
	BRCC __ADDF1210
	COM  R0
	RCALL __NEGMAN1
__ADDF1210:
	TST  R22
	BRMI __ADDF129
	LSL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVC __ADDF1210

__MINRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	SER  R23
	POP  R21
	RET

__ZERORES:
	CLR  R30
	CLR  R31
	CLR  R22
	CLR  R23
	POP  R21
	RET

__MAXRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	LDI  R23,0x7F
	POP  R21
	RET

__MULF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BREQ __ZERORES
	CPI  R25,0x80
	BREQ __ZERORES
	EOR  R0,R1
	SEC
	ADC  R23,R25
	BRVC __MULF124
	BRLT __ZERORES
__MULF125:
	TST  R0
	BRMI __MINRES
	RJMP __MAXRES
__MULF124:
	PUSH R19
	PUSH R20
	CLR  R1
	CLR  R19
	CLR  R20
	CLR  R21
	LDI  R25,24
__MULF120:
	LSL  R19
	ROL  R20
	ROL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	BRCC __MULF121
	ADD  R19,R26
	ADC  R20,R27
	ADC  R21,R24
	ADC  R30,R1
	ADC  R31,R1
	ADC  R22,R1
__MULF121:
	DEC  R25
	BRNE __MULF120
	POP  R20
	POP  R19
	TST  R22
	BRMI __MULF122
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	RJMP __MULF123
__MULF122:
	INC  R23
	BRVS __MULF125
__MULF123:
	RCALL __REPACK
	POP  R21
	RET

__DIVF21:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BRNE __DIVF210
	TST  R1
__DIVF211:
	BRPL __MAXRES
	RJMP __MINRES
__DIVF210:
	CPI  R25,0x80
	BRNE __DIVF218
__DIVF217:
	RJMP __ZERORES
__DIVF218:
	EOR  R0,R1
	SEC
	SBC  R25,R23
	BRVC __DIVF216
	BRLT __DIVF217
	TST  R0
	RJMP __DIVF211
__DIVF216:
	MOV  R23,R25
	LSR  R22
	ROR  R31
	ROR  R30
	LSR  R24
	ROR  R27
	ROR  R26
	PUSH R20
	CLR  R1
	CLR  R20
	CLR  R21
	LDI  R25,24
__DIVF212:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	BRLO __DIVF213
	SUB  R26,R30
	SBC  R27,R31
	SBC  R24,R22
	SEC
	RJMP __DIVF214
__DIVF213:
	CLC
__DIVF214:
	ROL  R1
	ROL  R20
	ROL  R21
	ROL  R26
	ROL  R27
	ROL  R24
	DEC  R25
	BRNE __DIVF212
	MOV  R30,R1
	MOV  R31,R20
	MOV  R22,R21
	LSR  R26
	ADC  R30,R25
	ADC  R31,R25
	ADC  R22,R25
	POP  R20
	TST  R22
	BRMI __DIVF215
	LSL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVS __DIVF217
__DIVF215:
	RCALL __REPACK
	POP  R21
	RET

__CMPF12:
	TST  R25
	BRMI __CMPF120
	TST  R23
	BRMI __CMPF121
	CP   R25,R23
	BRLO __CMPF122
	BRNE __CMPF121
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	BRLO __CMPF122
	BREQ __CMPF123
__CMPF121:
	CLZ
	CLC
	RET
__CMPF122:
	CLZ
	SEC
	RET
__CMPF123:
	SEZ
	CLC
	RET
__CMPF120:
	TST  R23
	BRPL __CMPF122
	CP   R25,R23
	BRLO __CMPF121
	BRNE __CMPF122
	CP   R30,R26
	CPC  R31,R27
	CPC  R22,R24
	BRLO __CMPF122
	BREQ __CMPF123
	RJMP __CMPF121

__CPD20:
	SBIW R26,0
	SBCI R24,0
	SBCI R25,0
	RET

__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

;END OF CODE MARKER
__END_OF_CODE:
