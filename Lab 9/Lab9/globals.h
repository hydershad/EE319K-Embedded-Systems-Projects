#include <stdint.h>
extern int ADC_Status;
extern int ADC_Mailbox;
extern char data;
extern char Fifo_Arr[10];
extern int GetI;
extern int PutI;
extern uint32_t input;
extern int fifostatus;
extern int RxCounter;
extern uint32_t FiFo_Put(char data);
extern int delay;
extern void Systick_Init(void);
extern void Output_Init(void);
