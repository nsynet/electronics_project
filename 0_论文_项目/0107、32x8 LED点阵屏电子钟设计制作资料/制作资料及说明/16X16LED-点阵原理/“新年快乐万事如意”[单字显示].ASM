;;;;;;;
;************************************************************
;这是一个汉字显示程序////用16*16的点阵屏显示
;==========================================
;本程序显示的汉字组是“新年快乐万事如意”
;************************************************************
;;;;;;;


ORG  00H

LOOP: MOV A,#0FFH ;豢跏蓟宄?

MOV P0,A   ;磺宄齈0口

       ANL P2,#00   ;；清除P2口

MOV R2,#200  

D100MS: MOV R3,#250 ;；延时100毫秒

       DJNZ R3,$

       DJNZ R2,D100MS

       MOV 20H,#00H ;；取码指针的初值

l100:    MOV R1,#100 ;；每个字的停留时间

L16:    MOV R6,#16 ;；每个字16个码

       MOV R4,#00H ;；扫描指针清零

       MOV R0,20H ;；取码指针存入R0

L3:    MOV A,R4   ;；扫描指针存入A

       MOV P1,A  ; ；扫描输出

       INC R4      ;；扫描指针加1，扫描下一个

       MOV A,R0   ;； 取码指针存入A

       MOV DPTR,#TABLE ;；取数据表的上半部分的代码

       MOVC A,@A+DPTR

       MOV P0,A   ;； 输出到P0

       INC R0     ; ；取码指针加1，取下一个码。

       MOV A,R0

       MOV DPTR,#TABLE ;；取数据表下半部份的代码

       MOVC A,@A+DPTR

       MOV P2,A        ;  ；输出到P2口

       INC R0

MOV R3,#02       ; ；扫描1毫秒

DELAY2:  MOV R5,#248   ; ；

       DJNZ R5,$

       DJNZ R3,DELAY2

       MOV A,#00H    ; ；清除屏幕

       MOV P0,A

       ANL P2,#00H    

       DJNZ R6,L3      ; ；一个字16个码是否完成？

       DJNZ R1,L16     ; ；每个字的停留时间是否到了？

       MOV 20H,R0     ; ；取码指针存入20H

       CJNE R0,#0FFH,L100 ;；8个字256个码是否完成？

       JMP LOOP ;；反复循环

     

 TABLE :

;下面是“新年快乐，万事如意!

db 02H,04H,22H,88H,2AH,92H,0A6H,81H
db 63H,0FEH,26H,80H,6BH,0A0H,22H,94H       ;新
db 02H,18H,3FH,0E0H,22H,00H,42H,00H
db 0C3H,0FFH,46H,00H,02H,00H,00H,00H


db 00H,20H,04H,20H,08H,20H,33H,0E0H
db 0E2H,20H,22H,20H,22H,20H,3FH,0FFH
db 22H,20H,22H,20H,22H,20H,26H,20H        ;年
db 62H,20H,20H,60H,00H,20H,00H,00H

db 01H,00H,0EH,00H,00H,00H,0FFH,0FFH
db 08H,41H,04H,42H,08H,44H,08H,58H
db 0FFH,0E0H,08H,50H,08H,48H,08H,44H      ;快
db 1FH,0C2H,08H,43H,00H,42H,00H,00H

db 00H,00H,01H,02H,03H,04H,3DH,08H
db 21H,30H,21H,02H,21H,01H,2FH,0FEH
db 41H,00H,41H,00H,0C1H,20H,41H,10H     ;乐
db 01H,0CH,03H,06H,01H,00H,00H,00H





db 20H,00H,20H,01H,20H,02H,20H,04H
db 20H,08H,20H,70H,3FH,80H,22H,00H
db 22H,02H,22H,01H,22H,02H,27H,0FCH      ;万
db 22H,00H,60H,00H,20H,00H,00H,00H

db 20H,20H,20H,20H,2EH,0A8H,2AH,0A8H
db 2AH,0A8H,2AH,0AAH,2AH,0A9H,0FFH,0FEH    ;事
db 2AH,0A8H,2AH,0A8H,2AH,0A8H,2AH,0A8H
db 2EH,0FCH,60H,20H,20H,20H,00H,00H

db 08H,00H,08H,41H,0FH,0A2H,0F8H,14H
db 08H,08H,08H,34H,0FH,0C2H,00H,00H
db 0FH,0FEH,08H,04H,08H,04H,08H,04H      ;如
db 08H,04H,1FH,0FEH,08H,00H,00H,00H

db 08H,02H,08H,0CH,48H,00H,4BH,0EEH
db 6AH,0A1H,5AH,0A1H,4AH,0B1H,0CAH,0A9H
db 4AH,0ADH,5AH,0A1H,6AH,0A1H,0CFH,0E7H    ;意
db 4AH,00H,18H,08H,08H,06H,00H,00H

db 00H,00H,00H,00H,00H,00H,00H,00H
db 00H,00H,00H,00H,38H,00H,7FH,0CCH     ;!
db 7FH,0CCH,38H,00H,00H,00H,00H,00H
db 00H,00H,00H,00H,00H,00H,00H,00H
;

end

