// UART.c
// Runs on LM4F120/TM4C123
// Provide functions that setup and interact with UART
// Last Modified: 4/12/2016 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "globals.h"
void UART_Init(void){
	
	SYSCTL_RCGCUART_R |= 0x00000002;  // activate UART1
	delay = 0;
	delay = 0;
  SYSCTL_RCGCGPIO_R |= 0x00000004;  // activate port C
	delay = 0;
	delay = 0;
  UART1_CTL_R &= ~0x00000001;    // disable UART
  UART1_IBRD_R = 50;   				  // IBRD = int(80,000,000/(16*100,000)) = int(50)
  UART1_FBRD_R = 0;     				// FBRD = round(0.0000 * 64) = 0
  UART1_LCRH_R = 0x00000070;  	// 8 bit, no parity bits, one stop, FIFOs
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFF00FF)|0x00004000; // UART0=priority 2
	UART1_IFLS_R &= ~0x3F;     //clear FIFO INTERRUPT FIELDS
	UART1_IFLS_R |= 0x10;  			//set up for trigger flags when fifo is 1/2 empty or full
	UART1_IM_R = 0X10;
	NVIC_EN0_R = NVIC_EN0_INT6; //enable interrupts
  UART1_CTL_R |= 0x0301;     // enable RXE, TXE and UART
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000; // set up portC for UART
  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1
  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
}

//------------UART_InChar------------
// Wait for new input,
// then return ASCII code
// Input: none
// Output: char read from UART
// *** Need not be busy-wait if you know when to call
char UART_InChar(void){
while((UART1_FR_R & 0x10) !=0){}
return((char)(UART1_DR_R & 0xFF));
}
//------------UART_OutChar------------
// Wait for new input,
// then return ASCII code
// Input: none
// Output: char read from UART
void UART_OutChar(char data){
while((UART1_FR_R & 0x20) !=0){}
UART1_DR_R = data;
}

void UART1_Handler(void){
char indata = 0;
GPIO_PORTF_DATA_R ^= 0x0C;
while((UART1_FR_R & 0x10) == 0){
indata = UART_InChar();
FiFo_Put(indata);
}
UART1_ICR_R = 0x10;
RxCounter++;
GPIO_PORTF_DATA_R ^= 0x0C;
}
