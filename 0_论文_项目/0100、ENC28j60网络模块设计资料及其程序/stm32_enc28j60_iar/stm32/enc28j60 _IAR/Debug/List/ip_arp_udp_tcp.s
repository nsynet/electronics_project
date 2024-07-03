///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      21/Apr/2009  21:08:10 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60                            /
//                    _IAR\TCPIP\ip_arp_udp_tcp.c                             /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60                           /
//                    _IAR\TCPIP\ip_arp_udp_tcp.c" -lcN                       /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\" -lb     /
//                    "F:\b百利胜\enc28j60\enc28j60 _IAR\Debug\List\" -o      /
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
//    List file    =  F:\b百利胜\enc28j60\enc28j60                            /
//                    _IAR\Debug\List\ip_arp_udp_tcp.s                        /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME ip_arp_udp_tcp

        EXTERN enc28j60PacketSend

        PUBLIC checksum
        PUBLIC eth_type_is_arp_and_my_ip
        PUBLIC eth_type_is_ip_and_my_ip
        PUBLIC fill_ip_hdr_checksum
        PUBLIC fill_tcp_data
        PUBLIC fill_tcp_data_p
        PUBLIC get_tcp_data_pointer
        PUBLIC init_ip_arp_udp_tcp
        PUBLIC init_len_info
        PUBLIC make_arp_answer_from_request
        PUBLIC make_echo_reply_from_request
        PUBLIC make_eth
        PUBLIC make_ip
        PUBLIC make_tcp_ack_from_any
        PUBLIC make_tcp_ack_with_data
        PUBLIC make_tcp_synack_from_syn
        PUBLIC make_tcphead
        PUBLIC make_udp_reply_from_request


        SECTION `.data`:DATA:NOROOT(0)
wwwport:
        DATA
        DC8 80

        SECTION `.bss`:DATA:NOROOT(2)
macaddr:
        DS8 8

        SECTION `.bss`:DATA:NOROOT(2)
ipaddr:
        DS8 4

        SECTION `.bss`:DATA:NOROOT(2)
info_hdr_len:
        DS8 4

        SECTION `.bss`:DATA:NOROOT(2)
info_data_len:
        DS8 4

        SECTION `.data`:DATA:NOROOT(0)
seqnum:
        DATA
        DC8 10

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
checksum:
        MOVS     R3,#+0
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+1
        BNE.N    ??checksum_0
        ADDS     R3,R3,#+17
        SUBS     R12,R1,#+8
        ADDS     R3,R12,R3
??checksum_0:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+2
        BNE.N    ??checksum_1
        ADDS     R3,R3,#+6
        SUBS     R2,R1,#+8
        ADDS     R3,R2,R3
        B.N      ??checksum_1
??checksum_2:
        LDRB     R2,[R0, #+0]
        LDRB     R12,[R0, #+1]
        ORRS     R2,R12,R2, LSL #+8
        ADDS     R3,R2,R3
        ADDS     R0,R0,#+2
        SUBS     R1,R1,#+2
??checksum_1:
        CMP      R1,#+2
        BCS.N    ??checksum_2
        CMP      R1,#+0
        BEQ.N    ??checksum_3
        LDRB     R0,[R0, #+0]
        ADDS     R3,R3,R0, LSL #+8
        B.N      ??checksum_3
??checksum_4:
        UXTH     R0,R3            ;; ZeroExt  R0,R3,#+16,#+16
        ADDS     R3,R0,R3, LSR #+16
??checksum_3:
        LSRS     R0,R3,#+16
        CMP      R0,#+0
        BNE.N    ??checksum_4
        MOVW     R0,#+65535
        EORS     R0,R0,R3
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
init_ip_arp_udp_tcp:
        MOVS     R3,#+0
        LDR.W    R12,??DataTable7  ;; wwwport
        STRB     R2,[R12, #+0]
        B.N      ??init_ip_arp_udp_tcp_0
??init_ip_arp_udp_tcp_1:
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        LDRB     R2,[R3, R1]
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        LDR.W    R12,??DataTable9  ;; ipaddr
        STRB     R2,[R3, R12]
        ADDS     R3,R3,#+1
??init_ip_arp_udp_tcp_0:
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        CMP      R3,#+4
        BCC.N    ??init_ip_arp_udp_tcp_1
        MOVS     R3,#+0
        B.N      ??init_ip_arp_udp_tcp_2
??init_ip_arp_udp_tcp_3:
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        LDRB     R1,[R3, R0]
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        LDR.N    R2,??DataTable8  ;; macaddr
        STRB     R1,[R3, R2]
        ADDS     R3,R3,#+1
??init_ip_arp_udp_tcp_2:
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        CMP      R3,#+6
        BCC.N    ??init_ip_arp_udp_tcp_3
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
eth_type_is_arp_and_my_ip:
        MOVS     R2,#+0
        CMP      R1,#+41
        BCS.N    ??eth_type_is_arp_and_my_ip_0
        MOVS     R0,#+0
        B.N      ??eth_type_is_arp_and_my_ip_1
??eth_type_is_arp_and_my_ip_0:
        LDRB     R1,[R0, #+12]
        CMP      R1,#+8
        BNE.N    ??eth_type_is_arp_and_my_ip_2
        LDRB     R1,[R0, #+13]
        CMP      R1,#+6
        BEQ.N    ??eth_type_is_arp_and_my_ip_3
??eth_type_is_arp_and_my_ip_2:
        MOVS     R0,#+0
        B.N      ??eth_type_is_arp_and_my_ip_1
??eth_type_is_arp_and_my_ip_4:
        ADDS     R2,R2,#+1
??eth_type_is_arp_and_my_ip_3:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+4
        BCS.N    ??eth_type_is_arp_and_my_ip_5
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        ADDS     R1,R2,R0
        LDRB     R1,[R1, #+38]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LDR.N    R3,??DataTable9  ;; ipaddr
        LDRB     R3,[R2, R3]
        CMP      R1,R3
        BEQ.N    ??eth_type_is_arp_and_my_ip_4
        MOVS     R0,#+0
        B.N      ??eth_type_is_arp_and_my_ip_1
??eth_type_is_arp_and_my_ip_5:
        MOVS     R0,#+1
??eth_type_is_arp_and_my_ip_1:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
eth_type_is_ip_and_my_ip:
        MOVS     R2,#+0
        CMP      R1,#+42
        BCS.N    ??eth_type_is_ip_and_my_ip_0
        MOVS     R0,#+0
        B.N      ??eth_type_is_ip_and_my_ip_1
??eth_type_is_ip_and_my_ip_0:
        LDRB     R1,[R0, #+12]
        CMP      R1,#+8
        BNE.N    ??eth_type_is_ip_and_my_ip_2
        LDRB     R1,[R0, #+13]
        CMP      R1,#+0
        BEQ.N    ??eth_type_is_ip_and_my_ip_3
??eth_type_is_ip_and_my_ip_2:
        MOVS     R0,#+0
        B.N      ??eth_type_is_ip_and_my_ip_1
??eth_type_is_ip_and_my_ip_3:
        LDRB     R1,[R0, #+14]
        CMP      R1,#+69
        BEQ.N    ??eth_type_is_ip_and_my_ip_4
        MOVS     R0,#+0
        B.N      ??eth_type_is_ip_and_my_ip_1
??eth_type_is_ip_and_my_ip_5:
        ADDS     R2,R2,#+1
??eth_type_is_ip_and_my_ip_4:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+4
        BCS.N    ??eth_type_is_ip_and_my_ip_6
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        ADDS     R1,R2,R0
        LDRB     R1,[R1, #+30]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LDR.N    R3,??DataTable9  ;; ipaddr
        LDRB     R3,[R2, R3]
        CMP      R1,R3
        BEQ.N    ??eth_type_is_ip_and_my_ip_5
        MOVS     R0,#+0
        B.N      ??eth_type_is_ip_and_my_ip_1
??eth_type_is_ip_and_my_ip_6:
        MOVS     R0,#+1
??eth_type_is_ip_and_my_ip_1:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_eth:
        MOVS     R1,#+0
        B.N      ??make_eth_0
??make_eth_1:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R2,R1,R0
        LDRB     R2,[R2, #+6]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        STRB     R2,[R1, R0]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R2,R1,R0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.N    R3,??DataTable8  ;; macaddr
        LDRB     R3,[R1, R3]
        STRB     R3,[R2, #+6]
        ADDS     R1,R1,#+1
??make_eth_0:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+6
        BCC.N    ??make_eth_1
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
fill_ip_hdr_checksum:
        PUSH     {R4,LR}
        MOVS     R4,R0
        MOVS     R0,#+0
        STRB     R0,[R4, #+24]
        MOVS     R0,#+0
        STRB     R0,[R4, #+25]
        MOVS     R0,#+64
        STRB     R0,[R4, #+20]
        MOVS     R0,#+0
        STRB     R0,[R4, #+21]
        MOVS     R0,#+64
        STRB     R0,[R4, #+22]
        MOVS     R2,#+0
        MOVS     R1,#+20
        ADDS     R0,R4,#+14
        BL       checksum
        LSRS     R1,R0,#+8
        STRB     R1,[R4, #+24]
        STRB     R0,[R4, #+25]
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_ip:
        PUSH     {R7,LR}
        MOVS     R1,#+0
        B.N      ??make_ip_0
??make_ip_1:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R2,R1,R0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R3,R1,R0
        LDRB     R3,[R3, #+26]
        STRB     R3,[R2, #+30]
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        ADDS     R2,R1,R0
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.N    R3,??DataTable9  ;; ipaddr
        LDRB     R3,[R1, R3]
        STRB     R3,[R2, #+26]
        ADDS     R1,R1,#+1
??make_ip_0:
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+4
        BCC.N    ??make_ip_1
        BL       fill_ip_hdr_checksum
        POP      {R0,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_tcphead:
        PUSH     {R4,LR}
        MOVS     R12,#+0
        B.N      ??make_tcphead_0
??make_tcphead_1:
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     LR,R12,R0
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     R4,R12,R0
        LDRB     R4,[R4, #+34]
        STRB     R4,[LR, #+36]
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     LR,R12,R0
        MOVS     R4,#+0
        STRB     R4,[LR, #+34]
        ADDS     R12,R12,#+1
??make_tcphead_0:
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        CMP      R12,#+2
        BCC.N    ??make_tcphead_1
        LDR.W    R12,??DataTable7  ;; wwwport
        LDRB     R12,[R12, #+0]
        STRB     R12,[R0, #+35]
        MOVS     R12,#+4
        B.N      ??make_tcphead_2
??make_tcphead_3:
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     LR,R12,R0
        MOVS     R4,#+0
        STRB     R4,[LR, #+37]
??make_tcphead_4:
        LSRS     R1,R1,#+8
        SUBS     R12,R12,#+1
??make_tcphead_2:
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        CMP      R12,#+1
        BCC.N    ??make_tcphead_5
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     LR,R12,R0
        LDRB     LR,[LR, #+37]
        ADDS     R1,LR,R1
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     LR,R12,R0
        LDRB     LR,[LR, #+41]
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     R4,R12,R0
        STRB     R1,[R4, #+41]
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        CMP      R3,#+0
        BEQ.N    ??make_tcphead_3
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        ADDS     R4,R12,R0
        STRB     LR,[R4, #+37]
        B.N      ??make_tcphead_4
??make_tcphead_5:
        UXTB     R3,R3            ;; ZeroExt  R3,R3,#+24,#+24
        CMP      R3,#+0
        BNE.N    ??make_tcphead_6
        MOVS     R1,#+0
        STRB     R1,[R0, #+38]
        MOVS     R1,#+0
        STRB     R1,[R0, #+39]
        LDR.N    R1,??make_tcphead_7  ;; seqnum
        LDRB     R1,[R1, #+0]
        STRB     R1,[R0, #+40]
        MOVS     R1,#+0
        STRB     R1,[R0, #+41]
        LDR.N    R1,??make_tcphead_7  ;; seqnum
        LDRB     R1,[R1, #+0]
        ADDS     R1,R1,#+2
        LDR.N    R3,??make_tcphead_7  ;; seqnum
        STRB     R1,[R3, #+0]
??make_tcphead_6:
        MOVS     R1,#+0
        STRB     R1,[R0, #+50]
        MOVS     R1,#+0
        STRB     R1,[R0, #+51]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+0
        BEQ.N    ??make_tcphead_8
        MOVS     R1,#+2
        STRB     R1,[R0, #+54]
        MOVS     R1,#+4
        STRB     R1,[R0, #+55]
        MOVS     R1,#+5
        STRB     R1,[R0, #+56]
        MOVS     R1,#+128
        STRB     R1,[R0, #+57]
        MOVS     R1,#+96
        STRB     R1,[R0, #+46]
        B.N      ??make_tcphead_9
??make_tcphead_8:
        MOVS     R1,#+80
        STRB     R1,[R0, #+46]
??make_tcphead_9:
        POP      {R4,PC}          ;; return
        DATA
??make_tcphead_7:
        DC32     seqnum

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable7:
        DC32     wwwport

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_arp_answer_from_request:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,#+0
        MOVS     R0,R4
        BL       make_eth
        MOVS     R0,#+0
        STRB     R0,[R4, #+20]
        MOVS     R0,#+2
        STRB     R0,[R4, #+21]
        B.N      ??make_arp_answer_from_request_0
??make_arp_answer_from_request_1:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R5,R4
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R5,R4
        LDRB     R1,[R1, #+22]
        STRB     R1,[R0, #+32]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R5,R4
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LDR.N    R1,??DataTable8  ;; macaddr
        LDRB     R1,[R5, R1]
        STRB     R1,[R0, #+22]
        ADDS     R5,R5,#+1
??make_arp_answer_from_request_0:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+6
        BCC.N    ??make_arp_answer_from_request_1
        MOVS     R5,#+0
        B.N      ??make_arp_answer_from_request_2
??make_arp_answer_from_request_3:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R5,R4
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R1,R5,R4
        LDRB     R1,[R1, #+28]
        STRB     R1,[R0, #+38]
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        ADDS     R0,R5,R4
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        LDR.N    R1,??DataTable9  ;; ipaddr
        LDRB     R1,[R5, R1]
        STRB     R1,[R0, #+28]
        ADDS     R5,R5,#+1
??make_arp_answer_from_request_2:
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+4
        BCC.N    ??make_arp_answer_from_request_3
        MOVS     R1,R4
        MOVS     R0,#+42
        BL       enc28j60PacketSend
        POP      {R0,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable8:
        DC32     macaddr

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable9:
        DC32     ipaddr

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_echo_reply_from_request:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,R4
        BL       make_eth
        MOVS     R0,R4
        BL       make_ip
        MOVS     R0,#+0
        STRB     R0,[R4, #+34]
        LDRB     R0,[R4, #+36]
        CMP      R0,#+248
        BCC.N    ??make_echo_reply_from_request_0
        LDRB     R0,[R4, #+37]
        ADDS     R0,R0,#+1
        STRB     R0,[R4, #+37]
??make_echo_reply_from_request_0:
        LDRB     R0,[R4, #+36]
        ADDS     R0,R0,#+8
        STRB     R0,[R4, #+36]
        MOVS     R1,R4
        MOVS     R0,R5
        BL       enc28j60PacketSend
        POP      {R0,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_udp_reply_from_request:
        PUSH     {R4-R8,LR}
        MOVS     R6,R0
        MOVS     R4,R1
        MOVS     R7,R2
        MOVS     R5,R3
        MOVS     R8,#+0
        MOVS     R0,R6
        BL       make_eth
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R7,#+221
        BCC.N    ??make_udp_reply_from_request_0
        MOVS     R7,#+220
??make_udp_reply_from_request_0:
        MOVS     R0,#+0
        STRB     R0,[R6, #+16]
        ADDS     R0,R7,#+28
        STRB     R0,[R6, #+17]
        MOVS     R0,R6
        BL       make_ip
        LSRS     R0,R5,#+8
        STRB     R0,[R6, #+36]
        STRB     R5,[R6, #+37]
        MOVS     R0,#+0
        STRB     R0,[R6, #+38]
        ADDS     R0,R7,#+8
        STRB     R0,[R6, #+39]
        MOVS     R0,#+0
        STRB     R0,[R6, #+40]
        MOVS     R0,#+0
        STRB     R0,[R6, #+41]
        B.N      ??make_udp_reply_from_request_1
??make_udp_reply_from_request_2:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        ADDS     R0,R8,R6
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        LDRB     R1,[R8, R4]
        STRB     R1,[R0, #+42]
        ADDS     R8,R8,#+1
??make_udp_reply_from_request_1:
        UXTB     R8,R8            ;; ZeroExt  R8,R8,#+24,#+24
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        CMP      R8,R7
        BCC.N    ??make_udp_reply_from_request_2
        MOVS     R2,#+1
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R1,R7,#+16
        ADDS     R0,R6,#+26
        BL       checksum
        LSRS     R1,R0,#+8
        STRB     R1,[R6, #+40]
        STRB     R0,[R6, #+41]
        MOVS     R1,R6
        UXTB     R7,R7            ;; ZeroExt  R7,R7,#+24,#+24
        ADDS     R0,R7,#+42
        BL       enc28j60PacketSend
        POP      {R4-R8,PC}       ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_tcp_synack_from_syn:
        PUSH     {R4,LR}
        MOVS     R4,R0
        MOVS     R0,R4
        BL       make_eth
        MOVS     R0,#+0
        STRB     R0,[R4, #+16]
        MOVS     R0,#+44
        STRB     R0,[R4, #+17]
        MOVS     R0,R4
        BL       make_ip
        MOVS     R0,#+18
        STRB     R0,[R4, #+47]
        MOVS     R3,#+0
        MOVS     R2,#+1
        MOVS     R1,#+1
        MOVS     R0,R4
        BL       make_tcphead
        MOVS     R2,#+2
        MOVS     R1,#+32
        ADDS     R0,R4,#+26
        BL       checksum
        LSRS     R1,R0,#+8
        STRB     R1,[R4, #+50]
        STRB     R0,[R4, #+51]
        MOVS     R1,R4
        MOVS     R0,#+58
        BL       enc28j60PacketSend
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
get_tcp_data_pointer:
        LDR.N    R0,??DataTable22  ;; info_data_len
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BEQ.N    ??get_tcp_data_pointer_0
        LDR.N    R0,??DataTable17  ;; info_hdr_len
        LDR      R0,[R0, #+0]
        ADDS     R0,R0,#+34
        B.N      ??get_tcp_data_pointer_1
??get_tcp_data_pointer_0:
        MOVS     R0,#+0
??get_tcp_data_pointer_1:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
init_len_info:
        LDRB     R1,[R0, #+16]
        LDRB     R2,[R0, #+17]
        ORRS     R1,R2,R1, LSL #+8
        LDR.N    R2,??DataTable22  ;; info_data_len
        STR      R1,[R2, #+0]
        LDR.N    R1,??DataTable22  ;; info_data_len
        LDR      R1,[R1, #+0]
        SUBS     R1,R1,#+20
        LDR.N    R2,??DataTable22  ;; info_data_len
        STR      R1,[R2, #+0]
        LDRB     R0,[R0, #+46]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSRS     R0,R0,#+4
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        LSLS     R0,R0,#+2
        LDR.N    R1,??DataTable17  ;; info_hdr_len
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable22  ;; info_data_len
        LDR      R0,[R0, #+0]
        LDR.N    R1,??DataTable17  ;; info_hdr_len
        LDR      R1,[R1, #+0]
        SUBS     R0,R0,R1
        LDR.N    R1,??DataTable22  ;; info_data_len
        STR      R0,[R1, #+0]
        LDR.N    R0,??DataTable22  ;; info_data_len
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??init_len_info_0
        LDR.N    R0,??DataTable22  ;; info_data_len
        MOVS     R1,#+0
        STR      R1,[R0, #+0]
??init_len_info_0:
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable17:
        DC32     info_hdr_len

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
fill_tcp_data_p:
        MOVS     R3,R0
        MOVS     R0,R1
        B.N      ??fill_tcp_data_p_0
??fill_tcp_data_p_1:
        ADDS     R12,R0,R3
        STRB     R1,[R12, #+54]
        ADDS     R0,R0,#+1
??fill_tcp_data_p_0:
        LDRB     R1,[R2, #+0]
        ADDS     R2,R2,#+1
        MOV      R12,R1
        UXTB     R12,R12          ;; ZeroExt  R12,R12,#+24,#+24
        CMP      R12,#+0
        BNE.N    ??fill_tcp_data_p_1
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
fill_tcp_data:
        MOVS     R3,R0
        MOVS     R0,R1
        B.N      ??fill_tcp_data_0
??fill_tcp_data_1:
        ADDS     R1,R0,R3
        LDRB     R12,[R2, #+0]
        STRB     R12,[R1, #+54]
        ADDS     R0,R0,#+1
        ADDS     R2,R2,#+1
??fill_tcp_data_0:
        LDRB     R1,[R2, #+0]
        CMP      R1,#+0
        BNE.N    ??fill_tcp_data_1
        BX       LR               ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_tcp_ack_from_any:
        PUSH     {R4,LR}
        MOVS     R4,R0
        MOVS     R0,R4
        BL       make_eth
        MOVS     R0,#+16
        STRB     R0,[R4, #+47]
        LDR.N    R0,??DataTable22  ;; info_data_len
        LDR      R0,[R0, #+0]
        CMP      R0,#+0
        BNE.N    ??make_tcp_ack_from_any_0
        MOVS     R3,#+1
        MOVS     R2,#+0
        MOVS     R1,#+1
        MOVS     R0,R4
        BL       make_tcphead
        B.N      ??make_tcp_ack_from_any_1
??make_tcp_ack_from_any_0:
        MOVS     R3,#+1
        MOVS     R2,#+0
        LDR.N    R0,??DataTable22  ;; info_data_len
        LDR      R1,[R0, #+0]
        MOVS     R0,R4
        BL       make_tcphead
??make_tcp_ack_from_any_1:
        MOVS     R0,#+40
        LSRS     R1,R0,#+8
        STRB     R1,[R4, #+16]
        STRB     R0,[R4, #+17]
        MOVS     R0,R4
        BL       make_ip
        MOVS     R2,#+2
        MOVS     R1,#+28
        ADDS     R0,R4,#+26
        BL       checksum
        LSRS     R1,R0,#+8
        STRB     R1,[R4, #+50]
        STRB     R0,[R4, #+51]
        MOVS     R1,R4
        MOVS     R0,#+54
        BL       enc28j60PacketSend
        POP      {R4,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable22:
        DC32     info_data_len

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
make_tcp_ack_with_data:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        MOVS     R0,#+25
        STRB     R0,[R4, #+47]
        ADDS     R0,R5,#+40
        LSRS     R1,R0,#+8
        STRB     R1,[R4, #+16]
        STRB     R0,[R4, #+17]
        MOVS     R0,R4
        BL       fill_ip_hdr_checksum
        MOVS     R0,#+0
        STRB     R0,[R4, #+50]
        MOVS     R0,#+0
        STRB     R0,[R4, #+51]
        MOVS     R2,#+2
        ADDS     R1,R5,#+28
        ADDS     R0,R4,#+26
        BL       checksum
        LSRS     R1,R0,#+8
        STRB     R1,[R4, #+50]
        STRB     R0,[R4, #+51]
        MOVS     R1,R4
        ADDS     R0,R5,#+54
        BL       enc28j60PacketSend
        POP      {R0,R4,R5,PC}    ;; return

        END
// 
//    20 bytes in section .bss
//     2 bytes in section .data
// 1 412 bytes in section .text
// 
// 1 412 bytes of CODE memory
//    22 bytes of DATA memory
//
//Errors: none
//Warnings: none
