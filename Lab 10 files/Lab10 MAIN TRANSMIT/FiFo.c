// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/12/2016 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "globals.h"
int GetI = 0;
int PutI = 1;
int fifostatus = 1;


// Declare state variables for FiFo
//        size, buffer, put and get indexes

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(uint8_t data){
if((PutI + 1)%10 == GetI){
	
return(0); //FULL
}
Fifo_Arr[PutI] = data;
PutI = (PutI + 1)%10;
return 1;
}
// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(uint8_t *datapt)
{
	if((GetI +1)% 10 == PutI){
  return(0); //EMPTY
}
*datapt = Fifo_Arr[GetI];
GetI = (GetI + 1)% 10;
return 1;
}
