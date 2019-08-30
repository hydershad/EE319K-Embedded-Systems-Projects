
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "globals.h"
#include "UART.h"
#include "FiFo.h"


//struct sound{
//	uint8_t x;
//	uint8_t y;

//	// number of bytes in the array  const uint8_t *samples;  // pointer to the array }; typedef struct sound Sound_t;  
//// sounds is the array of structs one per sound Sound_t sounds[4] = {{450,startS},{280,shootS},{8,deathS},{1,quietS}};  
//uint32_t cSound; // holds the current sound number (0,1,2,or 3)  
//	
