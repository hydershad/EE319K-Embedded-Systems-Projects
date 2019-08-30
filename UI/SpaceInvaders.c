// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "globals.h"
#include "UART.h"
#include "FiFo.h"
int P1Index = 0;
int P2Index = 0;
int checkIndex = 0;
struct P1pos{
	uint8_t x;
	uint8_t y;
};
typedef struct P1pos P1Arr_t;
P1Arr_t P1pos[4000];

struct P2pos{
	uint8_t x;
	uint8_t y;
};
typedef struct P2pos P2Arr_t;
P2Arr_t P2pos[4000];
///////////////////////////////////////////////////////////

int delay = 0;
int sysreload = 4000000;		//systick reload value
uint32_t level = 0;

int rightb1 = 0; 	//PE0
int leftb1 = 0; 	//PE1
int rightb2 = 0; 	//PE3
int leftb2 = 0; 	//PE4
int boost1 = 0;
int boost2 = 0;
int boost1count = 0;
int boost2count = 0;

int h1 = 0;
int v1 = 0;
int h2 = 0;
int v2 = 0;

void Ports_Init(void);
void DisableInterrupts(void); 					// Disable interrupts
void EnableInterrupts(void);  					// Enable interrupts
void Delay(uint32_t count); 						// time delay in 0.1 seconds
void ST7735_SetRotation(uint8_t m);
void Systick_Init(int sysreload);
void GameOver(void);
void LightCycle(void);


struct Bike1 {
  uint32_t x;      // x coordinate
  uint32_t y;      // y coordinate
  const unsigned short *image; // ptr->image
  char life;            // 0=dead, 1=alive
};
typedef struct Bike1 P1;
P1 Player1[4];
struct Bike2 {
  uint32_t x;      // x coordinate
  uint32_t y;      // y coordinate
  const unsigned short *image; // ptr->image
  char life;            // 0=dead, 1=alive
};
typedef struct Bike2 P2;
P2 Player2[4];

int MoveP1(int h, int v){
	if(boost1>0){
		h = h*2;
		v = v*2;
		boost1--;
	}
	if((Player1[0].x)> 159 ){ //check x position
		Player1[3].life = 0;
		return 1;		//error, do nothing
		}
	if((Player1[1].y)> 120 ){ //check y position
		Player1[3].life = 0;
		return 1;  //error, do nothing
		}
while(h !=0 || v !=0){
		if(h<0){
      Player1[0].x -= 1;
			h++;
    }
		if(0<h){
		 Player1[0].x += 1;
		 h--;
    }
			if(v<0){
      Player1[1].y -= 1;
		v++;
    }
		if(0<v){
		 Player1[1].y += 1;
			v--;
    }
		P1pos[P1Index].x = Player1[0].x;
		P1pos[P1Index].y = Player1[1].y;
		checkIndex = P1Index;
		while(checkIndex !=0){
		if((P1pos[P1Index].x == P2pos[checkIndex].x)&&(P1pos[P1Index].y == P2pos[checkIndex].y)){
			Player1[3].life = 0;
		}
		checkIndex--;
		}
		checkIndex = P1Index-1;
		if(checkIndex>0){
		while(checkIndex !=0){
		if((P1pos[P1Index].x == P1pos[checkIndex].x)&&(P1pos[P1Index].y == P1pos[checkIndex].y)){
			Player1[3].life = 0;
		}
		checkIndex--;
		}
	}
		P1Index++;
		if(Player1[1].y <128){
		ST7735_DrawBitmap(Player1[0].x, Player1[1].y, pixel1, 1, 1);
	}
}
		//transmit image data to second display
		UART_OutChar(0x02);		//start transmission
		UART_OutChar(0x0E);		//current screen
		UART_OutChar(Player1[0].x);		//p1 data
		UART_OutChar(Player1[1].y);
		UART_OutChar(Player2[0].x);		//p2 data
		UART_OutChar(Player2[1].y);
		UART_OutChar(0x00);						//extra
		UART_OutChar(0x03);						//ETX			
return 0; 
}
void Player2_Init(void){
	int i;
  for(i=0;i<4;i++){
    Player2[i].x = 159;
    Player2[i].y = 60;
    Player2[i].image = &cycle2[80];
    Player2[i].life = 1;
	}
  P2direction = west;
		boost2count = 3;
	ST7735_DrawBitmap(Player2[0].x, Player2[1].y, cycle2, 40, 15);
}
int MoveP2(int h, int v){ 
		if(boost2>0){
		h = h*2;
		v = v*2;
		boost2--;
	}
	if((Player2[0].x)> 159 ){ //check x position
		Player2[3].life = 0;
		return 1;		//error, do nothing
		}
	if((Player2[1].y)> 120 ){ //check y position
		Player2[3].life = 0;
		return 1;  //error, do nothing
		}
 while(h !=0 || v !=0){
		if(h<0){
      Player2[0].x -= 1;
			h++;
    }
		if(0<h){
		 Player2[0].x += 1;
		 h--;
    }
			if(v<0){
      Player2[1].y -= 1;
			v++;
    }
		if(0<v){
		 Player2[1].y += 1;
		 v--;
    }
		P2pos[P2Index].x = Player2[0].x;
		P2pos[P2Index].y = Player2[1].y;
				checkIndex = P2Index;
		while(checkIndex !=0){
		if((P2pos[P2Index].x == P1pos[checkIndex].x)&&(P2pos[P2Index].y == P1pos[checkIndex].y)){
			Player2[3].life = 0;
		}
		checkIndex--;
		}
			checkIndex = P2Index-1;
		if(checkIndex>0){
		while(checkIndex !=0){
		if((P2pos[P2Index].x == P2pos[checkIndex].x)&&(P2pos[P2Index].y == P2pos[checkIndex].y)){
			Player2[3].life = 0;
		}
		checkIndex--;
		}
	}
		P2Index++;
		if(Player2[1].y <128){
		ST7735_DrawBitmap(Player2[0].x, Player2[1].y, pixel2, 1	, 1);
	}
}
 				//transmit image data to second display
		UART_OutChar(0x02);		//start transmission
		UART_OutChar(0x0E);		//current screen
		UART_OutChar(Player1[0].x);		//p1 data
		UART_OutChar(Player1[1].y);
		UART_OutChar(Player2[0].x);		//p2 data
		UART_OutChar(Player2[1].y);
		UART_OutChar(0x00);						//extra
		UART_OutChar(0x03);						//ETX
 return 0;
}
// *************************** MAIN ***************************

int main(void){
  TExaS_Init();  // set system clock to 80 MHz
  //Random_Init(1);
	Ports_Init();
	GetI = 0;					//FIFO Init
	PutI = 1;
	fifostatus = 1;
	UART_Init();
	ADC_Init();
	GPIO_PORTF_DATA_R = 0x0C;	
	Systick_Init(sysreload);
	ST7735_InitR(INITR_REDTAB);
	ST7735_SetRotation(3);
  ST7735_FillScreen(0x0000);   							  //set screen to black
	
while((select1 || select2) == 0){
}
while((select1 || select2)!= 0){}	
while(1){
		//transmit image data to second display
		UART_OutChar(0x02);		//start transmission
		UART_OutChar(0x0B);		//current screen
		UART_OutChar(Player1[0].x);		//p1 data
		UART_OutChar(Player1[1].y);
		UART_OutChar(Player2[0].x);		//p2 data
		UART_OutChar(Player2[1].y);
		UART_OutChar(0xA0);						//SOUND
		UART_OutChar(0x03);						//ETX	

ST7735_DrawBitmap(0,119, tronhome, 160, 120); //MAIN MENU LEVEL ONE OPTION
ST7735_DrawBitmap(53 ,60, selectw, 50, 3); 
ST7735_DrawBitmap(53, 90, selectw, 50, 3);
ST7735_SetCursor(11,7);
ST7735_OutString("START");
Delay(20);
while((select1 || select2) == 0){}
if((select1 || select2) !=0 ){		//ENTER LC BATTLE
LightCycle();
}
while((select1 || select2) != 0){}
}//CYCLE THROUGH MAIN MENU AGAIN
}
void LightCycle(){
	ST7735_FillScreen(0x0000);
	Player1_Init();
	Player2_Init();
	
		UART_OutChar(0x02);		//start transmission
		UART_OutChar(0x0D);		//current screen
		UART_OutChar(Player1[0].x);		//p1 data
		UART_OutChar(Player1[1].y);
		UART_OutChar(Player2[0].x);		//p2 data
		UART_OutChar(Player2[1].y);
		UART_OutChar(0x00);						//extra
		UART_OutChar(0x03);						//ETX	
	
	ST7735_SetCursor(5,6);
	ST7735_OutString("PLAYER 1 READY?");
	Delay(45);
	ST7735_FillScreen(0x0000);
	ST7735_DrawBitmap(Player1[0].x, Player1[1].y, cycle1, 40, 15);
	
	ST7735_SetCursor(12,6);
	ST7735_OutString("SET!");
	Delay(45);
	ST7735_FillScreen(0x0000);
	ST7735_DrawBitmap(Player1[0].x, Player1[1].y, cycle1, 40, 15);

	ST7735_SetCursor(12,6);
	ST7735_OutString("GO!!!");
	Delay(45);
	ButtonEdge_Init();
	Timer0_Init(&Set_Flag, speed);
	Player1_Init();
	Player2_Init();
	ST7735_FillScreen(0x0000);
while((Player1[3].life) == 1	&& (Player2[3].life )== 1){
	while(Arena_Flag == 0){}
Arena_Map(rightb1, leftb1, rightb2, leftb2);
MoveP1(h1, v1);
MoveP2(h2, v2);

	Arena_Flag = 0;
	
	rightb1= 0;
	leftb1= 0;
	rightb2= 0;
	leftb2= 0;
}	
GameOver();
}
void GameOver(void){
if(Player1[3].life == 0){	
		UART_OutChar(0x02);		//start transmission
		UART_OutChar(0xFF);		//current screen
		UART_OutChar(Player1[0].x);		//p1 data
		UART_OutChar(Player1[1].y);
		UART_OutChar(Player2[0].x);		//p2 data
		UART_OutChar(Player2[1].y);
		UART_OutChar(0xF0);						//SOUND
		UART_OutChar(0x03);						//ETX	
ST7735_FillScreen(0xFFFF);
ST7735_DrawBitmap(0,119, gameover, 160, 120);
}
if(Player1[3].life == 1){	
		UART_OutChar(0x02);		//start transmission
		UART_OutChar(0x0F);		//current screen
		UART_OutChar(Player1[0].x);		//p1 data
		UART_OutChar(Player1[1].y);
		UART_OutChar(Player2[0].x);		//p2 data
		UART_OutChar(Player2[1].y);
		UART_OutChar(0xF0);						//SOUND
		UART_OutChar(0x03);						//ETX
ST7735_FillScreen(0x0000);
ST7735_DrawBitmap(0,119, win, 160, 120);
}
int i = 0;
for(i=0; i<4000; i++){
P1pos[i].x = 0;
P1pos[i].y = 0;
	P2pos[i].x = 0;
	P2pos[i].y = 0;
}
while((select1 || select2) == 0){}
while((select1 || select2) != 0){}
		UART_OutChar(0x02);		//start transmission
		UART_OutChar(0xEE);		//current screen
		UART_OutChar(Player1[0].x);		//p1 data
		UART_OutChar(Player1[1].y);
		UART_OutChar(Player2[0].x);		//p2 data
		UART_OutChar(Player2[1].y);
		UART_OutChar(0xF0);						//SOUND
		UART_OutChar(0x03);						//ETX	
	
ST7735_FillScreen(0x0000);
	ST7735_SetCursor(0, 0);
	ST7735_OutString("EE 319K Spring 2016\nLab 10: Tron\nDesigned, Programmed\nand Built by:\n\nMira Sehgal\nHyder Shad");
while((select1 || select2) == 0){}
while((select1 || select2) != 0){}
}
void Systick_Init(int sysreload){
NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
NVIC_ST_RELOAD_R = sysreload;// reload value
NVIC_ST_CURRENT_R = 0;      // any write to current clears it
NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1
NVIC_ST_CTRL_R = 0x07; //RUN SYSTICK WITH INTERRUPTS
}

void Systick(uint32_t sysreload){
NVIC_ST_RELOAD_R = sysreload;
}

void SysTick_Handler(void){
GPIO_PORTF_DATA_R ^= 0x0C;
if((Player1[3].life ==1)&&(Player2[3].life == 1)){
	level = ADC_In();

	if(level<2000){
		Timer0_Init(&Set_Flag, speed);
	}
	else{
	Timer0_Init(&Set_Flag, (speed/2));
	}
}
	 if((GPIO_PORTB_DATA_R & 0x40) == 0x40){
		if((boost1==0) && (boost1count>0)){ 
		boost1 = 8;
		boost1count--;
		}
	}
	if((GPIO_PORTB_DATA_R & 0x80) == 0x80){
		if((boost2==0) && (boost2count>0)){ 
		boost2 = 8;
		boost2count--;
		}
	}
		
if(pause == 1){
	GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R &~0X0E);
	GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R | 0X02);
	while(pause == 1){}; 	//wait for release
	while(pause == 0){};	//wait for touch
	while(pause == 1){};	//wait for release
	GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R &~0X0E);
	GPIO_PORTF_DATA_R = 0x0C;	
}

}
void Delay(uint32_t count){
volatile int delaytime;
  while(count>0){
   delaytime = 200000; //.001
    while(delaytime){
	  	delaytime--;
    }
    count--;
  }
}

void Ports_Init(){
	
	SYSCTL_RCGCGPIO_R |= 0x02;
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTB_DIR_R &= ~0xFF;    
  GPIO_PORTB_AFSEL_R &= ~0xFF; // disable alt func on PB2-0
  GPIO_PORTB_DEN_R |= 0xFF;
	GPIO_PORTB_PDR_R |= 0xFF;
	
	SYSCTL_RCGCGPIO_R |= 0x20;
  delay = SYSCTL_RCGCGPIO_R;// ready?
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R |= 0x0E;    // 
  GPIO_PORTF_AFSEL_R &= ~0x0E; // disable alt func
  GPIO_PORTF_DEN_R |= 0x0E;			//
	
	SYSCTL_RCGCGPIO_R |= 0x10;
	delay = SYSCTL_RCGCGPIO_R;		// ready?
  GPIO_PORTE_DIR_R &= ~0x7F;    // PE5-0 IS INPUT
  GPIO_PORTE_AFSEL_R &= ~0x7F; 	
  GPIO_PORTE_DEN_R |= 0x7F;		
	GPIO_PORTE_AMSEL_R &= ~0x7F;
  //ADC Init
  GPIO_PORTE_AFSEL_R |= 0x10; 	// enable alt func
  GPIO_PORTE_DEN_R &= ~0x10;		//DISABLE DIGITAL
	GPIO_PORTE_AMSEL_R |= 0x10;		//ENABLE ANALOG
}
