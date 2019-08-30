#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "globals.h"


uint32_t ADC_In(void);

void Systick_Init(){
NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
NVIC_ST_RELOAD_R = 2000000;		// reload value
NVIC_ST_CURRENT_R = 0;      // any write to current clears it
NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1
NVIC_ST_CTRL_R = 0x07; //RUN SYSTICK WITH INTERRUPTS
}

void SysTick_Handler(void){

GPIO_PORTF_DATA_R ^= 0x0C;
ADC_Mailbox = ADC_In();
ADC_Status = 1;
}

