///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:09 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60 _IAR\TCPIP\ENC28J60.C      /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60 _IAR\TCPIP\ENC28J60.C"    /
//                    -lcN "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\"    /
//                    -lb "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\" -o  /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\Obj\"          /
//                    --no_cse --no_unroll --no_inline --no_code_motion       /
//                    --no_tbaa --no_clustering --no_scheduling --debug       /
//                    --endian=little --cpu=Cortex-M3 -e --fpu=None           /
//                    --dlib_config "D:\Program Files\IAR Systems\Embedded    /
//                    Workbench 5.0 Evaluation\ARM\INC\DLib_Config_Full.h"    /
//                    -I "F:\b百利胜\enc28j60\enc28j60                        /
//                    _IAR\EWARM_V2.0.2\library\inc\" -I                      /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\EWARM_V2.0.2\" -I    /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\" -I                 /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\TCPIP\" -I           /
//                    "D:\Program Files\IAR Systems\Embedded Workbench 5.0    /
//                    Evaluation\ARM\INC\" -Ol                                /
//    List file    =  F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\ENC28J60.s /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ENC28J60

        EXTERN GPIO_ResetBits
        EXTERN GPIO_SetBits
        EXTERN SPI1_ReadWrite
        EXTERN printf

        PUBLIC enc28j60Init
        PUBLIC enc28j60PacketReceive
        PUBLIC enc28j60PacketSend
        PUBLIC enc28j60PhyWrite
        PUBLIC enc28j60Read
        PUBLIC enc28j60ReadBuffer
        PUBLIC enc28j60ReadOp
        PUBLIC enc28j60SetBank
        PUBLIC enc28j60Write
        PUBLIC enc28j60WriteBuffer
        PUBLIC enc28j60WriteOp
        PUBLIC enc28j60clkout
        PUBLIC enc28j60getrev


        SECTION `.bss`:DATA:NOROOT(0)
Enc28j60Bank:
        DS8 1

        SECTION `.bss`:DATA:NOROOT(1)
NextPacketPtr:
        DS8 2

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60ReadOp:
        PUSH     {R4-R6,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R6,#+0
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_ResetBits
        ANDS     R0,R5,#0x1F
        ORRS     R0,R0,R4
        MOVS     R6,R0
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BL       SPI1_ReadWrite
        MOVS     R0,#+255
        BL       SPI1_ReadWrite
        MOVS     R6,R0
        TST      R5,#0x80
        BEQ.N    ??enc28j60ReadOp_0
        MOVS     R0,#+255
        BL       SPI1_ReadWrite
        MOVS     R6,R0
??enc28j60ReadOp_0:
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_SetBits
        MOVS     R0,R6
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R4-R6,PC}       ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60WriteOp:
        PUSH     {R3-R7,LR}
        MOVS     R5,R0
        MOVS     R6,R1
        MOVS     R4,R2
        MOVS     R7,#+0
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_ResetBits
        ANDS     R0,R6,#0x1F
        ORRS     R0,R0,R5
        MOVS     R7,R0
        MOVS     R0,R7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BL       SPI1_ReadWrite
        MOVS     R7,R4
        MOVS     R0,R7
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BL       SPI1_ReadWrite
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_SetBits
        POP      {R0,R4-R7,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60ReadBuffer:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_ResetBits
        MOVS     R0,#+58
        BL       SPI1_ReadWrite
        B.N      ??enc28j60ReadBuffer_0
??enc28j60ReadBuffer_1:
        SUBS     R4,R4,#+1
        MOVS     R0,#+0
        BL       SPI1_ReadWrite
        STRB     R0,[R5, #+0]
        ADDS     R5,R5,#+1
??enc28j60ReadBuffer_0:
        CMP      R4,#+0
        BNE.N    ??enc28j60ReadBuffer_1
        MOVS     R0,#+0
        STRB     R0,[R5, #+0]
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_SetBits
        POP      {R0,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60WriteBuffer:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_ResetBits
        MOVS     R0,#+122
        BL       SPI1_ReadWrite
        B.N      ??enc28j60WriteBuffer_0
??enc28j60WriteBuffer_1:
        SUBS     R4,R4,#+1
        LDRB     R0,[R5, #+0]
        BL       SPI1_ReadWrite
        ADDS     R5,R5,#+1
??enc28j60WriteBuffer_0:
        CMP      R4,#+0
        BNE.N    ??enc28j60WriteBuffer_1
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_SetBits
        POP      {R0,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60SetBank:
        PUSH     {R4,LR}
        MOVS     R4,R0
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        ANDS     R0,R4,#0x60
        LDR.N    R1,??enc28j60SetBank_0  ;; Enc28j60Bank
        LDRB     R1,[R1, #+0]
        CMP      R0,R1
        BEQ.N    ??enc28j60SetBank_1
        MOVS     R2,#+3
        MOVS     R1,#+31
        MOVS     R0,#+160
        BL       enc28j60WriteOp
        ANDS     R0,R4,#0x60
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R2,R0,#+5
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,#+31
        MOVS     R0,#+128
        BL       enc28j60WriteOp
        ANDS     R0,R4,#0x60
        LDR.N    R1,??enc28j60SetBank_0  ;; Enc28j60Bank
        STRB     R0,[R1, #+0]
??enc28j60SetBank_1:
        POP      {R4,PC}          ;; return
        DATA
??enc28j60SetBank_0:
        DC32     Enc28j60Bank

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60Read:
        PUSH     {R4,LR}
        MOVS     R4,R0
        MOVS     R0,R4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BL       enc28j60SetBank
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+0
        BL       enc28j60ReadOp
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60Write:
        PUSH     {R3-R5,LR}
        MOVS     R5,R0
        MOVS     R4,R1
        MOVS     R0,R5
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        BL       enc28j60SetBank
        MOVS     R2,R4
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+64
        BL       enc28j60WriteOp
        POP      {R0,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60PhyWrite:
        PUSH     {R4,LR}
        MOVS     R4,R1
        MOVS     R1,R0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+212
        BL       enc28j60Write
        MOVS     R1,R4
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+214
        BL       enc28j60Write
        LSRS     R1,R4,#+8
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+215
        BL       enc28j60Write
??enc28j60PhyWrite_0:
        MOVS     R0,#+234
        BL       enc28j60Read
        TST      R0,#0x1
        BNE.N    ??enc28j60PhyWrite_0
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60clkout:
        PUSH     {R7,LR}
        ANDS     R1,R0,#0x7
        MOVS     R0,#+117
        BL       enc28j60Write
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60Init:
        PUSH     {R4,LR}
        MOVS     R4,R0
        MOVS     R1,#+16
        LDR.N    R0,??DataTable8  ;; 0x40010800
        BL       GPIO_SetBits
        MOVS     R2,#+255
        MOVS     R1,#+0
        MOVS     R0,#+255
        BL       enc28j60WriteOp
        LDR.N    R0,??DataTable16  ;; NextPacketPtr
        MOVS     R1,#+0
        STRH     R1,[R0, #+0]
        MOVS     R1,#+0
        MOVS     R0,#+8
        BL       enc28j60Write
        MOVS     R1,#+0
        MOVS     R0,#+9
        BL       enc28j60Write
        MOVS     R1,#+0
        MOVS     R0,#+12
        BL       enc28j60Write
        MOVS     R1,#+0
        MOVS     R0,#+13
        BL       enc28j60Write
        MOVS     R1,#+254
        MOVS     R0,#+10
        BL       enc28j60Write
        MOVS     R1,#+25
        MOVS     R0,#+11
        BL       enc28j60Write
        MOVS     R1,#+255
        MOVS     R0,#+4
        BL       enc28j60Write
        MOVS     R1,#+25
        MOVS     R0,#+5
        BL       enc28j60Write
        MOVS     R1,#+255
        MOVS     R0,#+6
        BL       enc28j60Write
        MOVS     R1,#+31
        MOVS     R0,#+7
        BL       enc28j60Write
        MOVS     R1,#+176
        MOVS     R0,#+56
        BL       enc28j60Write
        MOVS     R1,#+63
        MOVS     R0,#+40
        BL       enc28j60Write
        MOVS     R1,#+48
        MOVS     R0,#+41
        BL       enc28j60Write
        MOVS     R1,#+249
        MOVS     R0,#+48
        BL       enc28j60Write
        MOVS     R1,#+247
        MOVS     R0,#+49
        BL       enc28j60Write
        MOVS     R1,#+13
        MOVS     R0,#+192
        BL       enc28j60Write
        MOVS     R1,#+0
        MOVS     R0,#+193
        BL       enc28j60Write
        MOVS     R2,#+51
        MOVS     R1,#+194
        MOVS     R0,#+128
        BL       enc28j60WriteOp
        MOVS     R1,#+18
        MOVS     R0,#+198
        BL       enc28j60Write
        MOVS     R1,#+12
        MOVS     R0,#+199
        BL       enc28j60Write
        MOVS     R1,#+18
        MOVS     R0,#+196
        BL       enc28j60Write
        MOVS     R1,#+220
        MOVS     R0,#+202
        BL       enc28j60Write
        MOVS     R1,#+5
        MOVS     R0,#+203
        BL       enc28j60Write
        LDRB     R1,[R4, #+0]
        MOVS     R0,#+228
        BL       enc28j60Write
        LDRB     R1,[R4, #+1]
        MOVS     R0,#+229
        BL       enc28j60Write
        LDRB     R1,[R4, #+2]
        MOVS     R0,#+226
        BL       enc28j60Write
        LDRB     R1,[R4, #+3]
        MOVS     R0,#+227
        BL       enc28j60Write
        LDRB     R1,[R4, #+4]
        MOVS     R0,#+224
        BL       enc28j60Write
        LDRB     R1,[R4, #+5]
        MOVS     R0,#+225
        BL       enc28j60Write
        MOVS     R0,#+228
        BL       enc28j60Read
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        MOVS     R1,R0
        LDR.N    R0,??enc28j60Init_0  ;; `?<Constant "MAADR5 = 0x%x\\r\\n">`
        BL       printf
        MOVS     R0,#+229
        BL       enc28j60Read
        MOVS     R1,R0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.N    R0,??enc28j60Init_0+0x4  ;; `?<Constant "MAADR4 = 0x%x\\r\\n">`
        BL       printf
        MOVS     R0,#+226
        BL       enc28j60Read
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        MOVS     R1,R0
        LDR.N    R0,??enc28j60Init_0+0x8  ;; `?<Constant "MAADR3 = 0x%x\\r\\n">`
        BL       printf
        MOVS     R0,#+227
        BL       enc28j60Read
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        MOVS     R1,R0
        LDR.N    R0,??enc28j60Init_0+0xC  ;; `?<Constant "MAADR2 = 0x%x\\r\\n">`
        BL       printf
        MOVS     R0,#+224
        BL       enc28j60Read
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        MOVS     R1,R0
        LDR.N    R0,??enc28j60Init_0+0x10  ;; `?<Constant "MAADR1 = 0x%x\\r\\n">`
        BL       printf
        MOVS     R0,#+225
        BL       enc28j60Read
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        MOVS     R1,R0
        LDR.N    R0,??enc28j60Init_0+0x14  ;; `?<Constant "MAADR0 = 0x%x\\r\\n">`
        BL       printf
        MOV      R1,#+256
        MOVS     R0,#+0
        BL       enc28j60PhyWrite
        MOV      R1,#+256
        MOVS     R0,#+16
        BL       enc28j60PhyWrite
        MOVS     R0,#+31
        BL       enc28j60SetBank
        MOVS     R2,#+192
        MOVS     R1,#+27
        MOVS     R0,#+128
        BL       enc28j60WriteOp
        MOVS     R2,#+4
        MOVS     R1,#+31
        MOVS     R0,#+128
        BL       enc28j60WriteOp
        POP      {R4,PC}          ;; return
        Nop      
        DATA
??enc28j60Init_0:
        DC32     `?<Constant "MAADR5 = 0x%x\\r\\n">`
        DC32     `?<Constant "MAADR4 = 0x%x\\r\\n">`
        DC32     `?<Constant "MAADR3 = 0x%x\\r\\n">`
        DC32     `?<Constant "MAADR2 = 0x%x\\r\\n">`
        DC32     `?<Constant "MAADR1 = 0x%x\\r\\n">`
        DC32     `?<Constant "MAADR0 = 0x%x\\r\\n">`

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable8:
        DC32     0x40010800

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60getrev:
        PUSH     {R7,LR}
        MOVS     R0,#+114
        BL       enc28j60Read
        POP      {R1,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60PacketSend:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R1,#+255
        MOVS     R0,#+2
        BL       enc28j60Write
        MOVS     R1,#+25
        MOVS     R0,#+3
        BL       enc28j60Write
        MOVS     R0,R4
        SUBS     R1,R0,#+1
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+6
        BL       enc28j60Write
        MOVW     R0,#+6655
        ADDS     R0,R0,R4
        LSRS     R1,R0,#+8
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+7
        BL       enc28j60Write
        MOVS     R2,#+0
        MOVS     R1,#+0
        MOVS     R0,#+122
        BL       enc28j60WriteOp
        MOVS     R1,R5
        MOVS     R0,R4
        BL       enc28j60WriteBuffer
        MOVS     R2,#+8
        MOVS     R1,#+31
        MOVS     R0,#+128
        BL       enc28j60WriteOp
        MOVS     R0,#+28
        BL       enc28j60Read
        TST      R0,#0x2
        BEQ.N    ??enc28j60PacketSend_0
        MOVS     R2,#+8
        MOVS     R1,#+31
        MOVS     R0,#+160
        BL       enc28j60WriteOp
??enc28j60PacketSend_0:
        POP      {R0,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
enc28j60PacketReceive:
        PUSH     {R3-R7,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,#+57
        BL       enc28j60Read
        CMP      R0,#+0
        BNE.N    ??enc28j60PacketReceive_0
        MOVS     R0,#+0
        B.N      ??enc28j60PacketReceive_1
??enc28j60PacketReceive_0:
        LDR.N    R0,??DataTable16  ;; NextPacketPtr
        LDRH     R1,[R0, #+0]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+0
        BL       enc28j60Write
        LDR.N    R0,??DataTable16  ;; NextPacketPtr
        LDRH     R0,[R0, #+0]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        LSRS     R1,R0,#+8
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+1
        BL       enc28j60Write
        MOVS     R1,#+0
        MOVS     R0,#+58
        BL       enc28j60ReadOp
        LDR.N    R1,??DataTable16  ;; NextPacketPtr
        STRH     R0,[R1, #+0]
        MOVS     R1,#+0
        MOVS     R0,#+58
        BL       enc28j60ReadOp
        LDR.N    R1,??DataTable16  ;; NextPacketPtr
        LDRH     R1,[R1, #+0]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        ORRS     R0,R1,R0, LSL #+8
        LDR.N    R1,??DataTable16  ;; NextPacketPtr
        STRH     R0,[R1, #+0]
        MOVS     R1,#+0
        MOVS     R0,#+58
        BL       enc28j60ReadOp
        MOVS     R6,R0
        MOVS     R1,#+0
        MOVS     R0,#+58
        BL       enc28j60ReadOp
        ORRS     R6,R6,R0, LSL #+8
        SUBS     R6,R6,#+4
        MOVS     R1,#+0
        MOVS     R0,#+58
        BL       enc28j60ReadOp
        MOVS     R7,R0
        MOVS     R1,#+0
        MOVS     R0,#+58
        BL       enc28j60ReadOp
        ORRS     R7,R7,R0, LSL #+8
        SUBS     R0,R4,#+1
        CMP      R0,R6
        BCS.N    ??enc28j60PacketReceive_2
        SUBS     R6,R4,#+1
??enc28j60PacketReceive_2:
        TST      R7,#0x80
        BNE.N    ??enc28j60PacketReceive_3
        MOVS     R6,#+0
        B.N      ??enc28j60PacketReceive_4
??enc28j60PacketReceive_3:
        MOVS     R1,R5
        MOVS     R0,R6
        BL       enc28j60ReadBuffer
??enc28j60PacketReceive_4:
        LDR.N    R0,??DataTable16  ;; NextPacketPtr
        LDRH     R1,[R0, #+0]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+12
        BL       enc28j60Write
        LDR.N    R0,??DataTable16  ;; NextPacketPtr
        LDRH     R0,[R0, #+0]
        UXTH     R0,R0            ;; ZeroExt  R0,R0,#+16,#+16
        LSRS     R1,R0,#+8
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        MOVS     R0,#+13
        BL       enc28j60Write
        MOVS     R2,#+64
        MOVS     R1,#+30
        MOVS     R0,#+128
        BL       enc28j60WriteOp
        MOVS     R0,R6
??enc28j60PacketReceive_1:
        POP      {R1,R4-R7,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable16:
        DC32     NextPacketPtr

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "MAADR5 = 0x%x\\r\\n">`:
        DATA
        DC8 "MAADR5 = 0x%x\015\012"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "MAADR4 = 0x%x\\r\\n">`:
        DATA
        DC8 "MAADR4 = 0x%x\015\012"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "MAADR3 = 0x%x\\r\\n">`:
        DATA
        DC8 "MAADR3 = 0x%x\015\012"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "MAADR2 = 0x%x\\r\\n">`:
        DATA
        DC8 "MAADR2 = 0x%x\015\012"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "MAADR1 = 0x%x\\r\\n">`:
        DATA
        DC8 "MAADR1 = 0x%x\015\012"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "MAADR0 = 0x%x\\r\\n">`:
        DATA
        DC8 "MAADR0 = 0x%x\015\012"

        END
// 
//     3 bytes in section .bss
//    96 bytes in section .rodata
// 1 146 bytes in section .text
// 
// 1 146 bytes of CODE  memory
//    96 bytes of CONST memory
//     3 bytes of DATA  memory
//
//Errors: none
//Warnings: none
