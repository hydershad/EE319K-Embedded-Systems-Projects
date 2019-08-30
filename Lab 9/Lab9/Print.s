; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
	IMPORT   UART_OutChar
    AREA    |.text|, CODE, READONLY, ALIGN=2
    PRESERVE8
	THUMB

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
		
		PUSH {R4, R5, LR} ; SAVE REGISTERS
		
		MOV R4, #1			;R4 COUNTS HOW MANY TIMES DIVISION IS DONE ON VALUE
		MOV R1, #10			;DIVIDE BY TEN EACH TIME TO 
		MOV R2, R0
decascii MOV R5, R2 		;R5 IS R2 RIGHT BEFORE DIVISION SO REMAINDER CAN BE CALCULATED
		UDIV R2, R2, R1
		MUL R3, R2, R1
		SUB R3, R5, R3
		ADD R3, R3, #0x30
		PUSH {R3}
		CMP R2, #0
		BEQ printdec
		ADD R4, R4, #1
		B decascii
	
printdec
		POP {R0}	
		BL ST7735_OutChar
		ADD R4, R4, #-1
		CMP R4, #0
		BNE printdec
exit_ascii POP {R4, R5, PC}	

;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix

		  PUSH {R4, R5, LR}
		  MOV R1, #9999
		  CMP R0, R1
	      BGT oversized	
		  CMP R0, #0
		  BLT oversized
		  MOV R1, #10			;DIVIDE BY TEN EACH TIME TO 
		  MOV R2, R0
		  MOV R4, #4   ; DIVIDE FOUR TIMES
fixedascii  MOV R5, R2 ;R5 IS R2 RIGHT BEFORE DIVISION SO REMAINDER CAN BE CALCULATED
		  UDIV R2, R2, R1
		  MUL R3, R2, R1
		  SUB R3, R5, R3
		  ADD R3, R3, #0x30
		  PUSH {R3}
		  ADD R4, R4, #-1
		  CMP R4, #0
		  BNE fixedascii
		  
printfixed 	POP {R0}
			BL UART_OutChar
			MOV R0, #0x2E		  
			BL UART_OutChar
			POP {R0}
			BL UART_OutChar
			POP {R0}
			BL UART_OutChar
			POP {R0}
			BL UART_OutChar
			
			POP {R4, R5, PC}
			
oversized MOV R0, #0x2A				;ASTERISK SYM
		  BL UART_OutChar
		  MOV R0, #0x2E            ;PERIOD SYM
		  BL UART_OutChar
		  MOV R0, #0x2A
		  BL UART_OutChar
		  MOV R0, #0x2A
		  BL UART_OutChar
		  MOV R0, #0x2A
		  BL UART_OutChar
		  
		  POP {R4, R5, PC}
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
