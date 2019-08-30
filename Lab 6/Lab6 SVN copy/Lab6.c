// Lab6.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: HYDER SHAD, MIRA SEHGAL
// Date Created: 1/24/2016 
// Last Modified: 3/6/2016 
// Section: THURSDAY 2-3 PM     TA: RANDOM
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "dac.h"
#include "TExaS.h"
#include "globals.h"
int delay;
uint32_t period;

#define C 2389;
#define D 2128;
#define E 1896;
#define G 1594;
/*#define D2 1064;
#define E2 1004;
#define C2 1194;
#define B2 1265;
#define A2 1420;
*/
uint32_t Piano_In(void);
void auto_play(void);
void Sound_Play(uint32_t period);

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

int main(void){    
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
	Piano_Init();
  Sound_Init(period);
	//Timer0A_Init(period);

  // other initialization
  EnableInterrupts();

	SYSCTL_RCGC2_R |= 0x20;
  delay = SYSCTL_RCGC2_R;// ready?
  GPIO_PORTF_DIR_R |= 0x04;
  GPIO_PORTF_AFSEL_R &= ~0x04;
  GPIO_PORTF_DEN_R |= 0x04;
	GPIO_PORTF_AMSEL_R &= ~0x04;      // no analog 
  GPIO_PORTF_PCTL_R &= 0x00; // regular GPIO function
	GPIO_PORTF_LOCK_R = 0x4C4F434B;

while(1){

val = Piano_In();
period = 0;
	
	if((GPIO_PORTE_DATA_R &0x10)== 0x10){
		 auto_play();	
	}
	if (val == 0x01){
		 period = C;
	}
	if (val == 0x02){
		 period = D;
	}
	if (val == 0x04){
		 period = E;
	}
	if (val == 0x08){
	   period = G;
	}
Sound_Play(period);
}
}
