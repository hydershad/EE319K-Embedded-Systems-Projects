// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "globals.h"
#define DAC (*((volatile uint32_t *)0x400050FC
int sine = 0;

void DAC_Out(uint8_t sine);
// Code files contain the actual implemenation for public functions
// this file also contains private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
const uint8_t wave[64] = {
	32,35,38,41,44,47,49,52,54,56,58,59,61,062,62,63,63,63,62,62,61,59,58,
	56,54,52,49,47,44,41,38,35,32,29,26,23,20,17,15,12,10,8,6,5,3,2,2,1,
	1,1,2,2,3,5,6,8,10,12,15,17,20,23,26,29};

void DAC_Init(void){
	
SYSCTL_RCGCGPIO_R |= 0x02;
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTB_AMSEL_R &= ~0x3F;      // no analog 
  GPIO_PORTB_PCTL_R &= ~0x00000FFF; // regular GPIO function
  GPIO_PORTB_DIR_R |= 0x3F;      // make PB2-0 out
  GPIO_PORTB_AFSEL_R &= ~0x3F;   // disable alt funct on PB2-0
  GPIO_PORTB_DEN_R |= 0x3F;      // enable digital I/O on PB2-0
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63
// Output: none

//void SysTick_Handler(void){
//	GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^= 0x04;



void DAC_Out(uint8_t sine){
	GPIO_PORTB_DATA_R = sine;
}
