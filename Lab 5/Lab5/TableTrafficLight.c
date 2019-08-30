// ***** 0. Documentation Section *****
// TableTrafficLight.c for (Lab 10 edX), Lab 5 EE319K
// Runs on LM4F120/TM4C123
// Program written by: HYDER SHAD, MIRA SEHGAL
// Date Created: 1/24/2015 
// Last Modified: 3/9/2016
// Section: THURSDAY 2-3PM     TA: RANDOM
// Lab number: 5
// ***********Hardware connections***********
// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE0 (1=pedestrian present)
// north/south car detector connected to PE2 (1=car present)
// east/west car detector connected to PE1 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)
// 7406N LED Logic Driver (connected to LED Port B connections)
// 0.1uF capacitor for voltage smoothing, connected between 5V Vcc input to 7406N Driver and GND (5V source GND)
// 6 220 ohm resistors 5% tolerance for each of the six street LEDs
// three normally open push button switches connected to each of three inputs (PE0-2)
// Arduino (Adafruit Metro Mini) supplying 5V source (and 5V GND)
// jumpercables as needed to make connections
// ***** 1. Pre-processor Directives Section *****
#include <stdint.h>
#include <stdlib.h>
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "Systick.h"

// ***** 2. Global Declarations Section *****

#define GoS 0 
#define WS 1
#define WSx 2
#define WSw 3 
#define GoX 4
#define FlashOn1 5 
#define FlashOff1 6
#define FlashOn2 7
#define FlashOff2 8 
#define NoXw 9
#define NoXs 10
#define GoW 11
#define WW 12
#define WWx 13
#define WWs 14
//make sure hashdefines are in order of FSM state/structures
typedef struct FSMnode FSM_t;

struct FSMnode{	
	uint8_t trafficOut;
	uint8_t pedestrianOut;
	uint32_t wait;
	uint8_t next[8];
};

// say pb0 is west indicator
const FSM_t FSM[15]= {
	{0x0C, 0x02, 200, {GoS, WS, WS, WS, GoS, WSx, WSw, WS}}, //GoS, go south //each position represents a hex value from whatever port you set up, so if no requests, 
	{0x14, 0x02, 100, {GoW, GoX, GoW, GoX, GoW, GoX, GoW, GoX}}, //WS, wait south
	{0x14, 0x02, 100, {GoX, GoX, GoX, GoX, GoX, GoX, GoX, GoX}}, //WSx, wait south to go to GoX
	{0x14, 0x02, 100, {GoW, GoW, GoW, GoW, GoW, GoW, GoW, GoW}}, //WSw, wait south to go to GoW
	{0x24, 0x08, 200, {FlashOn1, GoX, FlashOn1, FlashOn1, FlashOn1, FlashOn1, FlashOn1, FlashOn1}}, //GoX, Ped Xing/Xwalk
	{0x24, 0x02, 50, {FlashOff1, FlashOff1, FlashOff1, FlashOff1, FlashOff1, FlashOff1, FlashOff1, FlashOff1}}, //FlashOn1
	{0x24, 0x00, 50, {FlashOn2, FlashOn2, FlashOn2, FlashOn2, FlashOn2, FlashOn2, FlashOn2, FlashOn2}}, //FlashOff1
	{0x24, 0x02, 50, {FlashOff2, FlashOff2, FlashOff2, FlashOff2, FlashOff2, FlashOff2, FlashOff2, FlashOff2}}, //FlashOn2
	{0x24, 0x00, 50, {GoS, GoX, GoW, NoXw, GoS, NoXs, GoS, GoW}}, //FlashOff2
	{0x24, 0x02, 100, {GoW, GoW, GoW, GoW, GoW, GoW, GoW, GoW}}, //NoXw, flash and go straight to GoW
	{0x24, 0x02, 100, {GoS, GoS, GoS, GoS, GoS, GoS, GoS, GoS}}, //NoXs, flash and go straight to GoS
	{0x21, 0x02, 200, {GoW, WW, GoW, WWx, WW, WW, WWs, WW}}, //GoW
	{0x22, 0x02, 100, {GoS, GoX, GoS, GoX, GoS, GoX, GoS, GoS}}, //WW
	{0x22, 0x02, 100, {GoX, GoX, GoX, GoX, GoX, GoX, GoX, GoX}}, //WWx
	{0x22, 0x02, 100, {GoS, GoS, GoS, GoS, GoS, GoS, GoS, GoS}}, //WWs
	//state zero is Go south which is first element in the structure, if input is x01, then still do go west, so you need a duplicate GoW in the 1 position, the next state is WWS if x02 
	//is pressed because south traffic is requested, and so on and so forth including situations where multiple sensors trigered, like x03, 200 means wait 10ms x 200 to get 2s
};

uint8_t state;
uint32_t delay = 0;
// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
int main(void){

	SYSCTL_RCGC2_R |= 0x02;
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTB_DIR_R |= 0x3F;    // 
  GPIO_PORTB_AFSEL_R &= ~0x3F; // disable alt func on PB2-0
  GPIO_PORTB_DEN_R |= 0x3F;
	
	SYSCTL_RCGC2_R |= 0x20;
  delay = SYSCTL_RCGC2_R;// ready?
  GPIO_PORTF_DIR_R |= 0x0A;    // 
  GPIO_PORTF_AFSEL_R &= ~0x0A; // disable alt func on PB2-0
  GPIO_PORTF_DEN_R |= 0x0A;

	SYSCTL_RCGC2_R |= 0x10;
  delay = SYSCTL_RCGC2_R;// ready?
  GPIO_PORTE_DIR_R &= ~0x07;    // 
  GPIO_PORTE_AFSEL_R &= ~0x07; // disable alt func on PB2-0
  GPIO_PORTE_DEN_R |= 0x07;	
	
SysTick_Init();
TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate grader and set system clock to 80 MHz

EnableInterrupts(); 
state = GoS;
  while(1){
		GPIO_PORTB_DATA_R = FSM[state].trafficOut;
		GPIO_PORTF_DATA_R = FSM[state].pedestrianOut;
		SysTick_Wait10ms(FSM[state].wait);
		state = FSM[state].next[GPIO_PORTE_DATA_R &0x07];
  }
}
