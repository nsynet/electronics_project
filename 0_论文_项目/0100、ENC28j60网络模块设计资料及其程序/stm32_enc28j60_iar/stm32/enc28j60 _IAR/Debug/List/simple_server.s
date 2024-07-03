///////////////////////////////////////////////////////////////////////////////
//                                                                            /
//                                                      22/Apr/2009  23:41:43 /
// IAR ANSI C/C++ Compiler V5.20.2.21007/W32 EVALUATION for ARM               /
// Copyright 1999-2008 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode     =  thumb                                                   /
//    Endian       =  little                                                  /
//    Source file  =  F:\b百利胜\enc28j60\enc28j60 _IAR\TCPIP\simple_server.c /
//    Command line =  "F:\b百利胜\enc28j60\enc28j60                           /
//                    _IAR\TCPIP\simple_server.c" -lcN                        /
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
//                    _IAR\Debug\List\simple_server.s                         /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME simple_server

        EXTERN enc28j60Init
        EXTERN enc28j60PacketReceive
        EXTERN enc28j60PhyWrite
        EXTERN enc28j60clkout
        EXTERN eth_type_is_arp_and_my_ip
        EXTERN eth_type_is_ip_and_my_ip
        EXTERN fill_tcp_data
        EXTERN fill_tcp_data_p
        EXTERN get_tcp_data_pointer
        EXTERN init_ip_arp_udp_tcp
        EXTERN init_len_info
        EXTERN make_arp_answer_from_request
        EXTERN make_echo_reply_from_request
        EXTERN make_tcp_ack_from_any
        EXTERN make_tcp_ack_with_data
        EXTERN make_tcp_synack_from_syn
        EXTERN make_udp_reply_from_request
        EXTERN strcpy
        EXTERN strlen
        EXTERN strncmp

        PUBLIC WebSide
        PUBLIC analyse_get_url
        PUBLIC print_webpage
        PUBLIC simple_server
        PUBLIC verify_password


        SECTION `.data`:DATA:NOROOT(2)
mymac:
        DATA
        DC8 84, 85, 88, 16, 0, 36, 0, 0

        SECTION `.data`:DATA:NOROOT(2)
myip:
        DATA
        DC8 192, 168, 1, 25

        SECTION `.data`:DATA:NOROOT(2)
baseurl:
        DATA
        DC8 "http://192.168.1.25/"
        DC8 0, 0, 0

        SECTION `.data`:DATA:NOROOT(2)
mywwwport:
        DATA
        DC32 80

        SECTION `.data`:DATA:NOROOT(2)
myudpport:
        DATA
        DC32 1200

        SECTION `.bss`:DATA:NOROOT(2)
buf:
        DS8 1504

        SECTION `.data`:DATA:NOROOT(2)
password:
        DATA
        DC8 "123456"
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "HTTP/1.0 200 OK\\r\\nCont...">`:
        DATA
        DC8 48H, 54H, 54H, 50H, 2FH, 31H, 2EH, 30H
        DC8 20H, 32H, 30H, 30H, 20H, 4FH, 4BH, 0DH
        DC8 0AH, 43H, 6FH, 6EH, 74H, 65H, 6EH, 74H
        DC8 2DH, 54H, 79H, 70H, 65H, 3AH, 20H, 74H
        DC8 65H, 78H, 74H, 2FH, 68H, 74H, 6DH, 6CH
        DC8 0DH, 0AH, 50H, 72H, 61H, 67H, 6DH, 61H
        DC8 3AH, 20H, 6EH, 6FH, 2DH, 63H, 61H, 63H
        DC8 68H, 65H, 0DH, 0AH, 0DH, 0AH, 0
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "<center><p>LED\\312\\344\\263\\366: ">`:
        DATA
        DC8 "<center><p>LED\312\344\263\366: "
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "<font color=\\"#00FF00\\"...">`:
        DATA
        DC8 "<font color=\"#00FF00\"> \301\301</font>"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
        DATA
        DC8 "\303\360"
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant " <small><a href=\\"">`:
        DATA
        DC8 " <small><a href=\""
        DC8 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "\\">[\\313\\242\\320\\302]</a></small><`:
        DATA
        DC8 "\">[\313\242\320\302]</a></small></p>\012<p><a href=\""
        DC8 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "/0\\">\\271\\330\\261\\325LED</a><p>">`:
        DATA
        DC8 "/0\">\271\330\261\325LED</a><p>"
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "/1\\">\\277\\252\\306\\364LED</a><p>">`:
        DATA
        DC8 "/1\">\277\252\306\364LED</a><p>"
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "</center><hr><br>STM3...">`:
        DATA
        DC8 3CH, 2FH, 63H, 65H, 6EH, 74H, 65H, 72H
        DC8 3EH, 3CH, 68H, 72H, 3EH, 3CH, 62H, 72H
        DC8 3EH, 53H, 54H, 4DH, 33H, 32H, 46H, 31H
        DC8 30H, 33H, 56H, 42H, 54H, 36H, 20H, 0BBH
        DC8 0F9H, 0D3H, 0DAH, 55H, 43H, 4FH, 53H, 2DH
        DC8 49H, 49H, 20H, 56H, 32H, 2EH, 38H, 35H
        DC8 20H, 57H, 45H, 42H, 20H, 0CDH, 0F8H, 0D2H
        DC8 0B3H, 0CAH, 0D4H, 0D1H, 0E9H, 2DH, 2DH, 2DH
        DC8 2DH, 0D1H, 0F4H, 0D6H, 0BEH, 0D3H, 0C2H, 0AH
        DC8 0
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "HTTP/1.0 200 OK\\r\\nCont...">_1`:
        DATA
        DC8 48H, 54H, 54H, 50H, 2FH, 31H, 2EH, 30H
        DC8 20H, 32H, 30H, 30H, 20H, 4FH, 4BH, 0DH
        DC8 0AH, 43H, 6FH, 6EH, 74H, 65H, 6EH, 74H
        DC8 2DH, 54H, 79H, 70H, 65H, 3AH, 20H, 74H
        DC8 65H, 78H, 74H, 2FH, 68H, 74H, 6DH, 6CH
        DC8 0DH, 0AH, 0DH, 0AH, 3CH, 68H, 31H, 3EH
        DC8 32H, 30H, 30H, 20H, 4FH, 4BH, 3CH, 2FH
        DC8 68H, 31H, 3EH, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "GET ">`:
        DATA
        DC8 "GET "
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "HTTP/1.0 200 OK\\r\\nCont...">_2`:
        DATA
        DC8 "HTTP/1.0 200 OK\015\012Content-Type: text/html\015\012\015\012"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "<p>Usage: ">`:
        DATA
        DC8 "<p>Usage: "
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "password</p>">`:
        DATA
        DC8 "password</p>"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
        DATA
        DC8 "/ "
        DC8 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "HTTP/1.0 401 Unauthor...">`:
        DATA
        DC8 48H, 54H, 54H, 50H, 2FH, 31H, 2EH, 30H
        DC8 20H, 34H, 30H, 31H, 20H, 55H, 6EH, 61H
        DC8 75H, 74H, 68H, 6FH, 72H, 69H, 7AH, 65H
        DC8 64H, 0DH, 0AH, 43H, 6FH, 6EH, 74H, 65H
        DC8 6EH, 74H, 2DH, 54H, 79H, 70H, 65H, 3AH
        DC8 20H, 74H, 65H, 78H, 74H, 2FH, 68H, 74H
        DC8 6DH, 6CH, 0DH, 0AH, 0DH, 0AH, 3CH, 68H
        DC8 31H, 3EH, 34H, 30H, 31H, 20H, 55H, 6EH
        DC8 61H, 75H, 74H, 68H, 6FH, 72H, 69H, 7AH
        DC8 65H, 64H, 3CH, 2FH, 68H, 31H, 3EH, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "e=no_cmd">`:
        DATA
        DC8 "e=no_cmd"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
        DATA
        DC8 "t=1"

        SECTION `.rodata`:CONST:NOROOT(2)
        DATA
        DC8 "t=0"

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "e=no_such_cmd">`:
        DATA
        DC8 "e=no_such_cmd"
        DC8 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
`?<Constant "e=invalid_pw">`:
        DATA
        DC8 "e=invalid_pw"
        DC8 0, 0, 0

        SECTION `.rodata`:CONST:NOROOT(2)
WebSide:
        DATA
        DC8 3CH, 70H, 3EH, 3CH, 62H, 3EH, 3CH, 73H
        DC8 70H, 61H, 6EH, 20H, 6CH, 61H, 6EH, 67H
        DC8 3DH, 45H, 4EH, 2DH, 55H, 53H, 3EH, 41H
        DC8 2FH, 44H, 20H, 3CH, 2FH, 73H, 70H, 61H
        DC8 6EH, 3EH, 0CEH, 0C2H, 0B6H, 0C8H, 0BCH, 0E0H
        DC8 0CAH, 0D3H, 3CH, 73H, 70H, 61H, 6EH, 20H
        DC8 6CH, 61H, 6EH, 67H, 3DH, 45H, 4EH, 2DH
        DC8 55H, 53H, 3EH, 3AH, 28H, 3CH, 2FH, 73H
        DC8 70H, 61H, 6EH, 3EH, 0B7H, 0B6H, 0CEH, 0A7H
        DC8 3CH, 73H, 70H, 61H, 6EH, 0DH, 0AH, 6CH
        DC8 61H, 6EH, 67H, 3DH, 45H, 4EH, 2DH, 55H
        DC8 53H, 3EH, 30H, 7EH, 3CH, 73H, 74H, 31H
        DC8 3AH, 63H, 68H, 6DH, 65H, 74H, 63H, 6EH
        DC8 76H, 20H, 55H, 6EH, 69H, 74H, 4EH, 61H
        DC8 6DH, 65H, 3DH, 22H, 0A1H, 0E3H, 43H, 22H
        DC8 20H, 53H, 6FH, 75H, 72H, 63H, 65H, 56H
        DC8 61H, 6CH, 75H, 65H, 3DH, 22H, 35H, 30H
        DC8 22H, 20H, 48H, 61H, 73H, 53H, 70H, 61H
        DC8 63H, 65H, 3DH, 22H, 46H, 61H, 6CH, 73H
        DC8 65H, 22H, 0DH, 0AH, 4EH, 65H, 67H, 61H
        DC8 74H, 69H, 76H, 65H, 3DH, 22H, 46H, 61H
        DC8 6CH, 73H, 65H, 22H, 20H, 4EH, 75H, 6DH
        DC8 62H, 65H, 72H, 54H, 79H, 70H, 65H, 3DH
        DC8 22H, 31H, 22H, 20H, 54H, 43H, 53H, 43H
        DC8 3DH, 22H, 30H, 22H, 20H, 77H, 3AH, 73H
        DC8 74H, 3DH, 22H, 6FH, 6EH, 22H, 3EH, 35H
        DC8 30H, 0A1H, 0E3H, 43H, 3CH, 2FH, 73H, 74H
        DC8 31H, 3AH, 63H, 68H, 6DH, 65H, 74H, 63H
        DC8 6EH, 76H, 3EH, 29H, 3CH, 2FH, 73H, 70H
        DC8 61H, 6EH, 3EH, 3CH, 2FH, 62H, 3EH, 3CH
        DC8 2FH, 70H, 3EH, 0DH, 0AH, 0DH, 0AH, 3CH
        DC8 74H, 61H, 62H, 6CH, 65H, 20H, 63H, 6CH
        DC8 61H, 73H, 73H, 3DH, 4DH, 73H, 6FH, 4EH
        DC8 6FH, 72H, 6DH, 61H, 6CH, 54H, 61H, 62H
        DC8 6CH, 65H, 20H, 62H, 6FH, 72H, 64H, 65H
        DC8 72H, 3DH, 31H, 20H, 63H, 65H, 6CH, 6CH
        DC8 73H, 70H, 61H, 63H, 69H, 6EH, 67H, 3DH
        DC8 30H, 20H, 63H, 65H, 6CH, 6CH, 70H, 61H
        DC8 64H, 64H, 69H, 6EH, 67H, 3DH, 30H, 20H
        DC8 77H, 69H, 64H, 74H, 68H, 3DH, 35H, 34H
        DC8 30H, 0DH, 0AH, 20H, 73H, 74H, 79H, 6CH
        DC8 65H, 3DH, 27H, 77H, 69H, 64H, 74H, 68H
        DC8 3AH, 34H, 30H, 35H, 2EH, 30H, 70H, 74H
        DC8 3BH, 6DH, 73H, 6FH, 2DH, 63H, 65H, 6CH
        DC8 6CH, 73H, 70H, 61H, 63H, 69H, 6EH, 67H
        DC8 3AH, 30H, 63H, 6DH, 3BH, 62H, 61H, 63H
        DC8 6BH, 67H, 72H, 6FH, 75H, 6EH, 64H, 3AH
        DC8 72H, 65H, 64H, 3BH, 62H, 6FH, 72H, 64H
        DC8 65H, 72H, 3AH, 6FH, 75H, 74H, 73H, 65H
        DC8 74H, 20H, 34H, 2EH, 35H, 70H, 74H, 3BH
        DC8 0DH, 0AH, 20H, 6DH, 73H, 6FH, 2DH, 70H
        DC8 61H, 64H, 64H, 69H, 6EH, 67H, 2DH, 61H
        DC8 6CH, 74H, 3AH, 30H, 63H, 6DH, 20H, 30H
        DC8 63H, 6DH, 20H, 30H, 63H, 6DH, 20H, 30H
        DC8 63H, 6DH, 27H, 3EH, 0DH, 0AH, 20H, 3CH
        DC8 74H, 72H, 20H, 73H, 74H, 79H, 6CH, 65H
        DC8 3DH, 27H, 6DH, 73H, 6FH, 2DH, 79H, 66H
        DC8 74H, 69H, 2DH, 69H, 72H, 6FH, 77H, 3AH
        DC8 30H, 3BH, 6DH, 73H, 6FH, 2DH, 79H, 66H
        DC8 74H, 69H, 2DH, 66H, 69H, 72H, 73H, 74H
        DC8 72H, 6FH, 77H, 3AH, 79H, 65H, 73H, 3BH
        DC8 6DH, 73H, 6FH, 2DH, 79H, 66H, 74H, 69H
        DC8 2DH, 6CH, 61H, 73H, 74H, 72H, 6FH, 77H
        DC8 3AH, 79H, 65H, 73H, 27H, 3EH, 0DH, 0AH
        DC8 20H, 20H, 3CH, 74H, 64H, 20H, 73H, 74H
        DC8 79H, 6CH, 65H, 3DH, 27H, 70H, 61H, 64H
        DC8 64H, 69H, 6EH, 67H, 3AH, 30H, 63H, 6DH
        DC8 20H, 30H, 63H, 6DH, 20H, 30H, 63H, 6DH
        DC8 20H, 30H, 63H, 6DH, 27H, 3EH, 0DH, 0AH
        DC8 20H, 20H, 3CH, 74H, 61H, 62H, 6CH, 65H
        DC8 20H, 63H, 6CH, 61H, 73H, 73H, 3DH, 4DH
        DC8 73H, 6FH, 4EH, 6FH, 72H, 6DH, 61H, 6CH
        DC8 54H, 61H, 62H, 6CH, 65H, 20H, 62H, 6FH
        DC8 72H, 64H, 65H, 72H, 3DH, 30H, 20H, 63H
        DC8 65H, 6CH, 6CH, 73H, 70H, 61H, 63H, 69H
        DC8 6EH, 67H, 3DH, 30H, 20H, 63H, 65H, 6CH
        DC8 6CH, 70H, 61H, 64H, 64H, 69H, 6EH, 67H
        DC8 3DH, 30H, 0DH, 0AH, 20H, 20H, 20H, 73H
        DC8 74H, 79H, 6CH, 65H, 3DH, 27H, 6DH, 73H
        DC8 6FH, 2DH, 63H, 65H, 6CH, 6CH, 73H, 70H
        DC8 61H, 63H, 69H, 6EH, 67H, 3AH, 30H, 63H
        DC8 6DH, 3BH, 6DH, 73H, 6FH, 2DH, 70H, 61H
        DC8 64H, 64H, 69H, 6EH, 67H, 2DH, 61H, 6CH
        DC8 74H, 3AH, 30H, 63H, 6DH, 20H, 30H, 63H
        DC8 6DH, 20H, 30H, 63H, 6DH, 20H, 30H, 63H
        DC8 6DH, 27H, 3EH, 0DH, 0AH, 20H, 20H, 20H
        DC8 3CH, 74H, 72H, 20H, 73H, 74H, 79H, 6CH
        DC8 65H, 3DH, 27H, 6DH, 73H, 6FH, 2DH, 79H
        DC8 66H, 74H, 69H, 2DH, 69H, 72H, 6FH, 77H
        DC8 3AH, 30H, 3BH, 6DH, 73H, 6FH, 2DH, 79H
        DC8 66H, 74H, 69H, 2DH, 66H, 69H, 72H, 73H
        DC8 74H, 72H, 6FH, 77H, 3AH, 79H, 65H, 73H
        DC8 3BH, 6DH, 73H, 6FH, 2DH, 79H, 66H, 74H
        DC8 69H, 2DH, 6CH, 61H, 73H, 74H, 72H, 6FH
        DC8 77H, 3AH, 79H, 65H, 73H, 27H, 3EH, 0DH
        DC8 0AH, 20H, 20H, 20H, 20H, 3CH, 74H, 64H
        DC8 20H, 73H, 74H, 79H, 6CH, 65H, 3DH, 27H
        DC8 62H, 61H, 63H, 6BH, 67H, 72H, 6FH, 75H
        DC8 6EH, 64H, 3AH, 6CH, 69H, 6DH, 65H, 3BH
        DC8 70H, 61H, 64H, 64H, 69H, 6EH, 67H, 3AH
        DC8 30H, 63H, 6DH, 20H, 30H, 63H, 6DH, 20H
        DC8 30H, 63H, 6DH, 20H, 30H, 63H, 6DH, 27H
        DC8 3EH, 0DH, 0AH, 20H, 20H, 20H, 20H, 3CH
        DC8 70H, 20H, 63H, 6CH, 61H, 73H, 73H, 3DH
        DC8 4DH, 73H, 6FH, 4EH, 6FH, 72H, 6DH, 61H
        DC8 6CH, 3EH, 3CH, 73H, 70H, 61H, 6EH, 20H
        DC8 6CH, 61H, 6EH, 67H, 3DH, 45H, 4EH, 2DH
        DC8 55H, 53H, 3EH, 26H, 6EH, 62H, 73H, 70H
        DC8 3BH, 3CH, 2FH, 73H, 70H, 61H, 6EH, 3EH
        DC8 3CH, 2FH, 70H, 3EH, 0DH, 0AH, 20H, 20H
        DC8 20H, 20H, 3CH, 2FH, 74H, 64H, 3EH, 0DH
        DC8 0AH, 0
        DC8 0, 0

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
verify_password:
        PUSH     {R7,LR}
        MOVS     R2,#+5
        MOVS     R1,R0
        LDR.N    R0,??DataTable4  ;; password
        BL       strncmp
        SUBS     R0,R0,#+1
        SBCS     R0,R0,R0
        LSRS     R0,R0,#+31
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        POP      {R1,PC}          ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
analyse_get_url:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,#+0
        MOVS     R0,R4
        BL       verify_password
        CMP      R0,#+0
        BNE.N    ??analyse_get_url_0
        MOVS     R0,#-1
        B.N      ??analyse_get_url_1
??analyse_get_url_2:
        ADDS     R5,R5,#+1
        ADDS     R4,R4,#+1
??analyse_get_url_0:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+0
        BEQ.N    ??analyse_get_url_3
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+10
        BCS.N    ??analyse_get_url_3
        LDRB     R0,[R4, #+0]
        CMP      R0,#+45
        BCC.N    ??analyse_get_url_3
        LDRB     R0,[R4, #+0]
        CMP      R0,#+123
        BCS.N    ??analyse_get_url_3
        LDRB     R0,[R4, #+0]
        CMP      R0,#+47
        BNE.N    ??analyse_get_url_2
        ADDS     R4,R4,#+1
??analyse_get_url_3:
        LDRB     R0,[R4, #+0]
        CMP      R0,#+58
        BCS.N    ??analyse_get_url_4
        LDRB     R0,[R4, #+0]
        CMP      R0,#+48
        BCC.N    ??analyse_get_url_4
        LDRSB    R0,[R4, #+0]
        SUBS     R0,R0,#+48
        SXTB     R0,R0            ;; SignExt  R0,R0,#+24,#+24
        B.N      ??analyse_get_url_1
??analyse_get_url_4:
        MVNS     R0,#+1
??analyse_get_url_1:
        POP      {R1,R4,R5,PC}    ;; return

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
print_webpage:
        PUSH     {R3-R5,LR}
        MOVS     R4,R0
        MOVS     R5,R1
        LDR.N    R2,??print_webpage_0+0x4  ;; `?<Constant "HTTP/1.0 200 OK\\r\\nCont...">`
        MOVS     R1,#+0
        MOVS     R0,R4
        BL       fill_tcp_data_p
        LDR.N    R2,??print_webpage_0+0x8  ;; `?<Constant "<center><p>LED\\312\\344\\263\\366: ">`
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??print_webpage_1
        LDR.N    R2,??print_webpage_0+0xC  ;; `?<Constant "<font color=\\"#00FF00\\"...">`
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
        B.N      ??print_webpage_2
??print_webpage_1:
        ADR.N    R2,??print_webpage_0  ;; 0xC3, 0xF0, 0x00, 0x00
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
??print_webpage_2:
        LDR.N    R2,??print_webpage_0+0x10  ;; `?<Constant " <small><a href=\\"">`
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
        LDR.W    R2,??DataTable5  ;; baseurl
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data
        LDR.N    R2,??DataTable4  ;; password
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data
        LDR.N    R2,??print_webpage_0+0x14  ;; `?<Constant "\\">[\\313\\242\\320\\302]</a></small><`
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
        LDR.W    R2,??DataTable5  ;; baseurl
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data
        LDR.N    R2,??DataTable4  ;; password
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??print_webpage_3
        LDR.N    R2,??print_webpage_0+0x18  ;; `?<Constant "/0\\">\\271\\330\\261\\325LED</a><p>">`
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
        B.N      ??print_webpage_4
??print_webpage_3:
        LDR.N    R2,??print_webpage_0+0x1C  ;; `?<Constant "/1\\">\\277\\252\\306\\364LED</a><p>">`
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
??print_webpage_4:
        LDR.N    R2,??print_webpage_0+0x20  ;; `?<Constant "</center><hr><br>STM3...">`
        MOVS     R1,R0
        MOVS     R0,R4
        BL       fill_tcp_data_p
        POP      {R1,R4,R5,PC}    ;; return
        Nop      
        DATA
??print_webpage_0:
        DC8      0xC3, 0xF0, 0x00, 0x00
        DC32     `?<Constant "HTTP/1.0 200 OK\\r\\nCont...">`
        DC32     `?<Constant "<center><p>LED\\312\\344\\263\\366: ">`
        DC32     `?<Constant "<font color=\\"#00FF00\\"...">`
        DC32     `?<Constant " <small><a href=\\"">`
        DC32     `?<Constant "\\">[\\313\\242\\320\\302]</a></small><`
        DC32     `?<Constant "/0\\">\\271\\330\\261\\325LED</a><p>">`
        DC32     `?<Constant "/1\\">\\277\\252\\306\\364LED</a><p>">`
        DC32     `?<Constant "</center><hr><br>STM3...">`

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable4:
        DC32     password

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
simple_server:
        PUSH     {R4-R6,LR}
        SUB      SP,SP,#+32
        MOVS     R5,#+0
        MOVS     R6,#+0
        MOVS     R4,#+0
        LDR.N    R0,??simple_server_0+0xC  ;; mymac
        BL       enc28j60Init
        LDR.N    R0,??simple_server_0+0x10  ;; mywwwport
        LDR      R2,[R0, #+0]
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LDR.N    R1,??simple_server_0+0x14  ;; myip
        LDR.N    R0,??simple_server_0+0xC  ;; mymac
        BL       init_ip_arp_udp_tcp
        MOVW     R1,#+1956
        MOVS     R0,#+20
        BL       enc28j60PhyWrite
        MOVS     R0,#+2
        BL       enc28j60clkout
        B.N      ??simple_server_1
??simple_server_2:
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       make_arp_answer_from_request
??simple_server_1:
        LDR.N    R1,??simple_server_0+0x18  ;; buf
        MOVW     R0,#+1500
        BL       enc28j60PacketReceive
        MOVS     R4,R0
        CMP      R4,#+0
        BEQ.N    ??simple_server_1
        MOVS     R1,R4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       eth_type_is_arp_and_my_ip
        CMP      R0,#+0
        BNE.N    ??simple_server_2
        MOVS     R1,R4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       eth_type_is_ip_and_my_ip
        CMP      R0,#+0
        BEQ.N    ??simple_server_1
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+23]
        CMP      R0,#+1
        BNE.N    ??simple_server_3
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+34]
        CMP      R0,#+8
        BNE.N    ??simple_server_3
        MOVS     R1,R4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       make_echo_reply_from_request
        B.N      ??simple_server_1
??simple_server_3:
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+23]
        CMP      R0,#+6
        BNE.W    ??simple_server_4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+36]
        CMP      R0,#+0
        BNE.W    ??simple_server_4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+37]
        LDR.N    R1,??simple_server_0+0x10  ;; mywwwport
        LDR      R1,[R1, #+0]
        CMP      R0,R1
        BNE.N    ??simple_server_4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+47]
        TST      R0,#0x2
        BEQ.N    ??simple_server_5
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       make_tcp_synack_from_syn
        B.N      ??simple_server_1
??simple_server_5:
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+47]
        TST      R0,#0x10
        BEQ.N    ??simple_server_4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       init_len_info
        BL       get_tcp_data_pointer
        MOVS     R6,R0
        CMP      R6,#+0
        BNE.N    ??simple_server_6
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+47]
        TST      R0,#0x1
        BEQ.N    ??simple_server_1
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       make_tcp_ack_from_any
        B.N      ??simple_server_1
??simple_server_6:
        MOVS     R2,#+4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        ADDS     R1,R6,R0
        LDR.N    R0,??simple_server_0+0x1C  ;; `?<Constant "GET ">`
        BL       strncmp
        CMP      R0,#+0
        BEQ.N    ??simple_server_7
        LDR.N    R2,??simple_server_0+0x20  ;; `?<Constant "HTTP/1.0 200 OK\\r\\nCont...">_1`
        MOVS     R1,#+0
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       fill_tcp_data_p
        MOVS     R4,R0
        B.N      ??simple_server_8
??simple_server_7:
        MOVS     R2,#+2
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        ADDS     R0,R6,R0
        ADDS     R1,R0,#+4
        ADR.N    R0,??simple_server_0  ;; 0x2F, 0x20, 0x00, 0x00
        BL       strncmp
        CMP      R0,#+0
        BNE.N    ??simple_server_9
        LDR.N    R2,??simple_server_0+0x24  ;; `?<Constant "HTTP/1.0 200 OK\\r\\nCont...">_2`
        MOVS     R1,#+0
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       fill_tcp_data_p
        MOVS     R4,R0
        LDR.N    R2,??simple_server_0+0x28  ;; `?<Constant "<p>Usage: ">`
        MOVS     R1,R4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       fill_tcp_data_p
        MOVS     R4,R0
        LDR.N    R2,??DataTable5  ;; baseurl
        MOVS     R1,R4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       fill_tcp_data
        MOVS     R4,R0
        LDR.N    R2,??simple_server_0+0x2C  ;; `?<Constant "password</p>">`
        MOVS     R1,R4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       fill_tcp_data_p
        MOVS     R4,R0
??simple_server_9:
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        ADDS     R0,R6,R0
        ADDS     R0,R0,#+5
        BL       analyse_get_url
        MOVS     R6,R0
        MOVS     R0,#-1
        SXTB     R6,R6            ;; SignExt  R6,R6,#+24,#+24
        CMP      R6,R0
        BNE.N    ??simple_server_10
        LDR.N    R2,??simple_server_0+0x30  ;; `?<Constant "HTTP/1.0 401 Unauthor...">`
        MOVS     R1,#+0
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       fill_tcp_data_p
        MOVS     R4,R0
??simple_server_10:
        SXTB     R6,R6            ;; SignExt  R6,R6,#+24,#+24
        CMP      R6,#+1
        BNE.N    ??simple_server_11
        MOVS     R5,#+1
??simple_server_11:
        SXTB     R6,R6            ;; SignExt  R6,R6,#+24,#+24
        CMP      R6,#+0
        BNE.N    ??simple_server_12
        MOVS     R5,#+0
??simple_server_12:
        MOVS     R1,R5
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       print_webpage
        MOVS     R4,R0
??simple_server_8:
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       make_tcp_ack_from_any
        MOVS     R1,R4
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       make_tcp_ack_with_data
        B.N      ??simple_server_1
??simple_server_4:
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+23]
        CMP      R0,#+17
        BNE.W    ??simple_server_1
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+36]
        CMP      R0,#+4
        BNE.W    ??simple_server_1
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+37]
        CMP      R0,#+176
        BNE.W    ??simple_server_1
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        LDRB     R0,[R0, #+39]
        SUBS     R4,R0,#+8
        LDR.N    R0,??simple_server_0+0x34  ;; buf + 42
        BL       verify_password
        CMP      R0,#+0
        BEQ.N    ??simple_server_13
        MOVS     R6,#+0
??simple_server_14:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        CMP      R6,R4
        BCS.N    ??simple_server_15
        ADDS     R6,R6,#+1
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        ADDS     R0,R6,R0
        LDRB     R0,[R0, #+42]
        CMP      R0,#+44
        BNE.N    ??simple_server_14
        ADDS     R6,R6,#+1
??simple_server_15:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R6,#+2
        BCC.N    ??simple_server_16
        UXTB     R4,R4            ;; ZeroExt  R4,R4,#+24,#+24
        SUBS     R0,R4,#+3
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        CMP      R0,R6
        BLT.N    ??simple_server_16
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        ADDS     R0,R6,R0
        LDRB     R0,[R0, #+43]
        CMP      R0,#+61
        BEQ.N    ??simple_server_17
??simple_server_16:
        LDR.N    R1,??simple_server_0+0x38  ;; `?<Constant "e=no_cmd">`
        ADD      R0,SP,#+0
        BL       strcpy
        B.N      ??simple_server_18
??simple_server_17:
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        ADDS     R0,R6,R0
        LDRB     R0,[R0, #+42]
        CMP      R0,#+116
        BNE.N    ??simple_server_19
        UXTB     R6,R6            ;; ZeroExt  R6,R6,#+24,#+24
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        ADDS     R0,R6,R0
        LDRB     R0,[R0, #+44]
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+49
        BNE.N    ??simple_server_20
        ADR.N    R1,??simple_server_0+0x4  ;; "t=1"
        ADD      R0,SP,#+0
        BL       strcpy
        B.N      ??simple_server_18
??simple_server_20:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+48
        BNE.N    ??simple_server_21
        ADR.N    R1,??simple_server_0+0x8  ;; "t=0"
        ADD      R0,SP,#+0
        BL       strcpy
        B.N      ??simple_server_18
??simple_server_21:
        UXTB     R0,R0            ;; ZeroExt  R0,R0,#+24,#+24
        CMP      R0,#+63
        BNE.N    ??simple_server_19
        ADR.N    R1,??simple_server_0+0x8  ;; "t=0"
        ADD      R0,SP,#+0
        BL       strcpy
        B.N      ??simple_server_18
??simple_server_19:
        LDR.N    R1,??simple_server_0+0x3C  ;; `?<Constant "e=no_such_cmd">`
        ADD      R0,SP,#+0
        BL       strcpy
        B.N      ??simple_server_18
??simple_server_13:
        LDR.N    R1,??simple_server_0+0x40  ;; `?<Constant "e=invalid_pw">`
        ADD      R0,SP,#+0
        BL       strcpy
??simple_server_18:
        ADD      R0,SP,#+0
        BL       strlen
        LDR.N    R1,??simple_server_0+0x44  ;; myudpport
        LDR      R3,[R1, #+0]
        MOVS     R2,R0
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        ADD      R1,SP,#+0
        LDR.N    R0,??simple_server_0+0x18  ;; buf
        BL       make_udp_reply_from_request
        B.N      ??simple_server_1
        DATA
??simple_server_0:
        DC8      0x2F, 0x20, 0x00, 0x00
        DC8      "t=1"
        DC8      "t=0"
        DC32     mymac
        DC32     mywwwport
        DC32     myip
        DC32     buf
        DC32     `?<Constant "GET ">`
        DC32     `?<Constant "HTTP/1.0 200 OK\\r\\nCont...">_1`
        DC32     `?<Constant "HTTP/1.0 200 OK\\r\\nCont...">_2`
        DC32     `?<Constant "<p>Usage: ">`
        DC32     `?<Constant "password</p>">`
        DC32     `?<Constant "HTTP/1.0 401 Unauthor...">`
        DC32     buf + 42
        DC32     `?<Constant "e=no_cmd">`
        DC32     `?<Constant "e=no_such_cmd">`
        DC32     `?<Constant "e=invalid_pw">`
        DC32     myudpport

        SECTION `.text`:CODE:NOROOT(2)
        DATA
??DataTable5:
        DC32     baseurl

        END
// 
// 1 504 bytes in section .bss
//    52 bytes in section .data
// 1 476 bytes in section .rodata
// 1 020 bytes in section .text
// 
// 1 020 bytes of CODE  memory
// 1 476 bytes of CONST memory
// 1 556 bytes of DATA  memory
//
//Errors: none
//Warnings: none
