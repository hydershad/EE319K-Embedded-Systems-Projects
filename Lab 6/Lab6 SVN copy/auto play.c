#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "globals.h"
#define G2 1594
#define D2 1064
#define E2 1004
#define C2 1194
#define B2 1265
#define A2 1420
volatile uint32_t j = 0;
volatile uint32_t k = 0;
void Sound_Play(uint32_t period);
void Timer0A_Handler(void);
void Timer0A_Init(uint32_t period);
const int autoplayperiod[43] = {
	G2, G2, D2, D2, E2, E2, D2, C2, C2, B2, B2, A2, A2,
	G2, D2, D2, C2, C2, B2, B2, A2, D2, D2, C2, C2, B2,
B2, A2, G2, G2, D2, D2, E2, E2, D2, C2, C2, B2, B2, A2, A2, G2, 0};

const int autoplaycycle[43] = {
4000, 4000, 4000, 4000, 4000, 4000, 8000,
4000, 4000, 4000, 4000, 4000, 4000, 8000,
4000, 4000, 4000, 4000, 4000, 4000, 8000,
4000, 4000, 4000, 4000, 4000, 4000, 8000,
4000, 4000, 4000, 4000, 4000, 4000, 8000,
4000, 4000, 4000, 4000, 4000, 4000, 8000,
1};

void auto_play(void){
Timer0A_Init(period);
	j=0;
	k=0;
while(k <43){
period = autoplayperiod[j];
Sound_Play(period);
}
TIMER0_CTL_R = 0x00000000;
}
void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout

	j++;
	k++;
	TIMER0_TAILR_R = autoplaycycle[k];
}

