; IO.s
; Student names: HYDER SHAD
; Last modification date: 3/30/2016
; Runs on LM4F120/TM4C123

; EE319K lab 7 device driver for the switch and LED.
; You are allowed to use any switch and any LED,
; although the Lab suggests the SW1 switch PF4 and Red LED PF1

; As part of Lab 7, students need to implement these three functions

; negative logic SW2 connected to PF0 on the Launchpad
; red LED connected to PF1 on the Launchpad
; blue LED connected to PF2 on the Launchpad
; green LED connected to PF3 on the Launchpad
; negative logic SW1 connected to PF4 on the Launchpad

        EXPORT   IO_Init
        EXPORT   IO_Touch
        EXPORT   IO_HeartBeat

GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_AFSEL_R EQU 0x40025420
GPIO_PORTF_PUR_R   EQU 0x40025510
GPIO_PORTF_DEN_R   EQU 0x4002551C
GPIO_PORTF_LOCK_R  EQU 0x40025520
GPIO_PORTF_CR_R    EQU 0x40025524
GPIO_PORTF_AMSEL_R EQU 0x40025528
GPIO_PORTF_PCTL_R  EQU 0x4002552C
GPIO_LOCK_KEY      EQU 0x4C4F434B  ; Unlocks the GPIO_CR register
	
GPIO_PORTA_DATA_R  EQU 0x400043FC
GPIO_PORTA_DIR_R   EQU 0x40004400
GPIO_PORTA_AFSEL_R EQU 0x40004420
GPIO_PORTA_DEN_R   EQU 0x4000451C	
GPIO_PORTA_PCTL_R  EQU 0x4000452C	
	
PF0       EQU 0x40025004
PF1       EQU 0x40025008
PF2       EQU 0x40025010
PF3       EQU 0x40025020
PF4       EQU 0x40025040
LEDS      EQU 0x40025038
RED       EQU 0x02
BLUE      EQU 0x04
GREEN     EQU 0x08
SWITCHES  EQU 0x40025044
SW1       EQU 0x10                 ; on the left side of the Launchpad board
SW2       EQU 0x01                 ; on the right side of the Launchpad board
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
heartcount EQU 555000
bouncecount EQU 160000	
        AREA    |.text|, CODE, READONLY, ALIGN=2
		THUMB



;------------IO_Init------------
; Initialize GPIO Port for a switch and an LED
; Input: none
; Output: none
; This is a public function
; Invariables: This function must not permanently modify registers R4 to R11
IO_Init
; 1) activate clock for Port F
    LDR R0, =SYSCTL_RCGCGPIO_R      
    LDR R1, [R0]                    
    ORR R1, #0X20						;SET BIT 5 HIGH TO ENABLE PORT F CLOCK	
    STR R1, [R0]                    
    NOP
    NOP                             	;ALLOW TIME TO FINISH ACTIVATING (2+ CYCLES)
    NOP

;2) UNLOCK
	LDR R0, =GPIO_PORTF_LOCK_R
	LDR R1, =GPIO_LOCK_KEY
	STR R1, [R0]
	
; 3) set direction register
	
	LDR R0, =GPIO_PORTF_DIR_R      		;SET BIT 4 LOW FOR BUTTON INPUT 2 HIGH FOR LED OUTPUT
	LDR R1, [R0]
	ORR R1, #0X0E
	STR R1, [R0]
	
; 5) configure as GPIO
    
	LDR R0, =GPIO_PORTF_PCTL_R              
    MOV R1, #0X0             			;CLEAR PORT CONTROL FIELD TO SET UP PINS FOR GPIO
    STR R1, [R0]                  
	
; 6) regular port function
    
	LDR R0, =GPIO_PORTF_AFSEL_R     	;DISABLE ALT FUNCTIONS FOR PIN BY SETTING BITS TO ZERO
    MOV R1, #0
    STR R1, [R0]
	
; 7) enable digital port

	LDR R0, =GPIO_PORTF_DEN_R       	;R1 = &GPIO_PORTD_DEN_R
    ORR R1, #0X1E              			;ENABLE DIGITAL I/O ON PIN 2 AND 4
    STR R1, [R0]
    
;* * * * * * * * End of IO_Init * * * * * * * *

;------------IO_HeartBeat------------
; Toggle the output state of the LED.
; Input: none
; Output: none
; This is a public function
; Invariables: This function must not permanently modify registers R4 to R11
IO_HeartBeat
    
	PUSH {R0, LR}
	LDR R0, =GPIO_PORTF_DATA_R
	LDR R1, [R0]
	EOR R1, R1, #0x02
	STR R1, [R0]
	BL heartdelay
	EOR R1, R1, #0x02
	STR R1, [R0]
	BL heartdelay
	EOR R1, R1, #0x04
	STR R1, [R0]
	BL heartdelay
	EOR R1, R1, #0x04
	STR R1, [R0]
	BL heartdelay
	EOR R1, R1, #0x08
	STR R1, [R0]
	BL heartdelay
	EOR R1, R1, #0x08
	STR R1, [R0]
	BL heartdelay
	
	POP {R0, PC}						; EXIT SUBROUTINE CALL
	
	
	
heartdelay  LDR R2, =heartcount			;time delay, count down from calculated value for 80 MHz Clock, with delay taking 5 cycles each time					
heartsub 	ADD R2, #-1						;subtract 1 from countdown					
			CMP R2, #0						;compare to see if countdown has reached 0
			BNE heartsub						;countdown = 0, then exit delay subroutine, otherwise loop to count
			BX LR							;return to IO_HeartBeat
;* * * * * * * * End of IO_HeartBeat * * * * * * * *

;------------IO_Touch------------
; First: wait for the of the release of the switch
; and then: wait for the touch of the switch
; Input: none
; Input: none
; Output: none
; This is a public function
; Invariables: This function must not permanently modify registers R4 to R11
IO_Touch

			 MOV R12, LR

wait_release LDR R0, =GPIO_PORTF_DATA_R			;LOAD PF4 SWITCH DATA	
			 LDR R1, [R0]
			 AND R1, R1, #0x10					;CHECK FOR SWITCH INPUT
			 CMP R1, #0
			 BNE wait_release					;IF SWITCH NOT RELEASED YET, KEEP SPINNING
			 
wait_touch   BL delay10ms			 			;GO TO DEBOUNCE DELAY
			 LDR R0, =GPIO_PORTF_DATA_R			;NOW CHECK TO SEE IF SWITCH PRESSSED/REPRESSED
			 LDR R1, [R0]
			 AND R1, R1, #0x10
			 CMP R1, #0
			 BEQ wait_touch						;IF NOT PRESSED, SPIN UNTIL SWITCH IS PRESSED
			 BL delay10ms
			 MOV LR, R12						;EXIT TOUCH SUBROUTINE
			 BX LR
			 
delay10ms 	 LDR R2, =bouncecount
bouncesub 	 ADD R2, #-1						;subtract 1 from countdown					
			 CMP R2, #0						;compare to see if countdown has reached 0
			 BNE bouncesub						;countdown = 0, then exit delay subroutine, otherwise loop to count
			 BX LR								;return to DEBOUNCE SUBROUTINE SECTION

;* * * * * * * * End of IO_Touch * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file