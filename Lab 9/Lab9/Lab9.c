// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: HYDER SHAD, MIRA SEHGAL
// Last modification date: 4/20/2016
// Last Modified: 4/20/2016

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w FiFo on the receiver end 
//    (we suggest implementing and testing this first)

#include <stdint.h>
#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"
#include "globals.h"
//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia
uint32_t input = 0; // ADC Value to be converted to cm
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
char data = 0;	//ASCI FOR TRANSMISSION FROM FIFO
uint32_t Position;  // 32-bit fixed-point 0.001 cm
int32_t TxCounter = 0;
int RxCounter = 0;
char Fifo_Arr[10] = {0,0,0,0,0,0,0,0,0,0};
uint32_t count8 = 0;
//void PortF_Init(void){unsigned long volatile delay
//  // Intialize PortF for hearbeat
//} done in ADC Init

// Get fit from excel and code the convert routine with the constants
// from the curve-fit
uint32_t Convert(uint32_t input){
input = (input * 1000)/2125; 
  return input; //replace with your calibration code
}

// final main program for bidirectional communication
// Sender sends using SysTick Interrupt
// Receiver receives using RX
int main(void){ 
  TExaS_Init();        // Bus clock is 80 MHz
	ST7735_InitR(INITR_REDTAB);
	ADC_Init();   			 // initialize to sample ADC1 and GPIO Ports for use
  UART_Init();      	 // initialize UART1 and Port C pin 4 and 5 for TX/RX
	FiFo_Init();
	Systick_Init();
  EnableInterrupts();
	ST7735_SetCursor(0,0);
	int i = 0;
	TxCounter = 0;
	RxCounter = 0;
	ST7735_OutString("EE 319K Lab 9!\nUniversal\nAsynchronous\nTransmitter Reciever\nADC RULER!!!\n\nMeasurement of\nDistance in\ncentimeters:");
  while(1){
		ST7735_SetCursor(0,10);
		FiFo_Get(&data);
		if(data == 0x02){
		for(i = 0; i<5; i++){
		fifostatus = FiFo_Get(&data);
		ST7735_OutChar(data);
}
fifostatus = FiFo_Get(&data); //gets rid of \r 
fifostatus = FiFo_Get(&data); //and ETX bits
ST7735_OutString("  cm");
}
}
}
void Systick_Init(){
NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
NVIC_ST_RELOAD_R = 2000000;		// reload value
NVIC_ST_CURRENT_R = 0;      // any write to current clears it
NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1
NVIC_ST_CTRL_R = 0x07; //RUN SYSTICK WITH INTERRUPTS
}

void SysTick_Handler(void){ 	//every 25 ms

GPIO_PORTF_DATA_R ^= 0x0C;
UART_OutChar(0x02);
input = ADC_In();
input = Convert(input);
LCD_OutFix(input);
UART_OutChar(0x0D);
UART_OutChar(0x03);
TxCounter++;
}
