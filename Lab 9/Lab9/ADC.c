// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "globals.h"
int delay = 0;
// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x20;
  delay = SYSCTL_RCGCGPIO_R;// ready?
  GPIO_PORTF_DIR_R |= 0x0E;    // 
  GPIO_PORTF_AFSEL_R &= ~0x0E; // disable alt func
  GPIO_PORTF_DEN_R |= 0x0E;			//
	
	SYSCTL_RCGCGPIO_R |= 0x10;
	delay = SYSCTL_RCGCGPIO_R;		// ready?
  GPIO_PORTE_DIR_R &= ~0x10;    // PE4 IS INPUT
  GPIO_PORTE_AFSEL_R |= 0x10; 	// enable alt func
  GPIO_PORTE_DEN_R &= ~0x10;		//DISABLE DIGITAL
	GPIO_PORTE_AMSEL_R |= 0x10;		//ENABLE ANALOG
  
	SYSCTL_RCGCADC_R |= 0x01;  												//ACTIVATE ADC0
	delay = SYSCTL_RCGCGPIO_R;												// ready?
	ADC0_ACTSS_R &= ~0x0008; 													//DISABLE SAMPLE SEQUENCER 3 FOR SETUP
	ADC0_PC_R = 0x01; 																//SET SAMPLE RATE TO 125KHz
	ADC0_SSPRI_R = 0x0123; 														//SEQUENCER 3 IS HIGH PRIORITY
	ADC0_EMUX_R &= ~0xF000; 													//SEQUENCE 3 IS SOFTWARE TRIGGERED
	ADC0_SSMUX3_R = ((ADC0_SSMUX3_R & 0xFFFFFFF0) + 1); //CLEAR SS3 FIELD AND SET CHANNEL TO 1(PE4/2 not sure which)
	ADC0_SSCTL3_R = 0x0006;														//STUFF....ASK TA
	ADC0_IM_R &= ~0x0008; 														//DISABLE SS3 INTERRUPTS (USING STYTICK AT 40Hz)
	ADC0_SAC_R = 0x06;															 	// A N means 2^N (16 here) samples are averaged; 0<=N<=6
	ADC0_ACTSS_R |= 0x0008; 													//RE-ENABLE SS3
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){
int digital = 0;
ADC0_PSSI_R = 0x0008; 									//INITIATE SS3 FOR DATA ACQUISITION
while((ADC0_RIS_R & 0x08)==0){}; 				//BUSY WAIT FOR SAMPLING
digital = ADC0_SSFIFO3_R & 0xFFF; 			//GET 12 BIT RESULT
ADC0_ISC_R = 0x0008; 										//ACKNOWLEDGE COMPLETION
return digital;
}
