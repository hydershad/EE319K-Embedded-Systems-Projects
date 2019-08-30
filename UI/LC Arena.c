#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "globals.h"

char Arena_Flag = 0;
int index1;
int index2;
void Set_Flag(void);
int Arena_Map(int R1, int L1, int R2, int L2);

int P1direction = east;
int P2direction = west;
int arrH[4] = {0, 1, 0, -1};
int arrV[4] = {1, 0, -1, 0};

void Set_Flag(){
Arena_Flag = 1;
}
int Arena_Map(int R1, int L1, int R2, int L2){

if(R1 == 1 && L1 ==0){
	if(P1direction == 3){
		P1direction = 0;
	}
	else{
			P1direction++;
	}
}
if(R1 == 0 && L1 ==1){
	if(P1direction == 0){
		P1direction = 3;
	}
	else{
			P1direction--;
	}
}
if(R2 == 1 && L2 ==0){
	if(P2direction == 3){
		P2direction = 0;
	}
	else{
			P2direction++;
	}
}
if(R2 == 0 && L2 ==1){
	if(P2direction == 0){
		P2direction = 3;
	}
	else{
			P2direction--;
	}
}

h1 = arrH[P1direction];	
v1 = arrV[P1direction];	
h2 = arrH[P2direction];	
v2 = arrV[P2direction];	

	return 0; //Positions updated, no collisions (returns 1 for P1 collision, 2 for P2 collision)	
}
