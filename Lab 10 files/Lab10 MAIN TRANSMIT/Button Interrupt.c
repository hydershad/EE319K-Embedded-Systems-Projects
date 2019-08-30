// EdgeInterrupt.c
// Runs on LM4F120 or TM4C123
// Request an interrupt on the falling edge of PF4 (when the user
// button is pressed) and increment a counter in the interrupt.  Note
// that button bouncing is not addressed.
// Daniel Valvano
// September 14, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
   Volume 1, Program 9.4
   
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Volume 2, Program 5.6, Section 5.5

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// user button connected to PF4 (increment counter on falling edge)

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "globals.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// global variable visible in Watch window of debugger
// increments at least once per button press

void ButtonEdge_Init(void){                          
   
  GPIO_PORTE_PCTL_R &= ~0x00FF0000; // configure PE3-0 as GPIO
  GPIO_PORTE_AMSEL_R = 0;       //     disable analog functionality on PE3-0
  GPIO_PORTE_PDR_R |= 0x3B;     //     enable weak pull-down on PF3-0
  GPIO_PORTE_IS_R &= ~0x3B;     // (d) PE3-0 is edge-sensitive
  GPIO_PORTE_IBE_R &= ~0x3B;    //     PE3-0 is not both edges
  GPIO_PORTE_IEV_R |= 0x3B;    //     PE3-0 rising edge event
  GPIO_PORTE_ICR_R = 0x3B;      // (e) clear flagS 3-0
  GPIO_PORTE_IM_R |= 0x3B;      // (f) arm interrupt on PE3-0 *** No IME bit as mentioned in Book ***
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000040; // (g) priority 2
  NVIC_EN0_R = 0x00000010;      // (h) enable interrupt 4 in NVIC

	
//  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
//  GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0
//  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
//  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
//  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4,0
//  GPIO_PORTF_PCTL_R &= ~0x000F000F; //  configure PF4,0 as GPIO
//  GPIO_PORTF_AMSEL_R &= ~0x11;  //     disable analog functionality on PF4,0
//  GPIO_PORTF_PDR_R |= 0x11;     //     enable weak pull-up on PF4,0
//  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
//  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
//  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event
//  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
//  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
//  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
//  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
	
}
void GPIOPortE_Handler(void){
  GPIO_PORTE_ICR_R = 0x3F;      // acknowledge flag4
	
	if((GPIO_PORTE_DATA_R & 0x01) == 0x01){
		rightb1 = 1;
		}	
	if((GPIO_PORTE_DATA_R & 0x02) == 0x02){
		leftb1 = 1;
		}
	if((GPIO_PORTE_DATA_R & 0x08) == 0x08){
		rightb2 = 1;
		}
	if((GPIO_PORTE_DATA_R & 0x20) == 0x20){
		leftb2= 1; 
		}
Delay(3);
	Arena_Flag = 1;

}
void GPIOPortF_Handler(void){
	GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
	 if((GPIO_PORTF_DATA_R & 0x01) == 0x01){
		if((boost1==0) && (boost1count>0)){ 
		boost1 = 8;
		boost1count--;
		}
	}
	if((GPIO_PORTF_DATA_R & 0x01) == 0x01){
		if((boost2==0) && (boost2count>0)){ 
		boost2 = 8;
		boost2count--;
		}
	}
}
