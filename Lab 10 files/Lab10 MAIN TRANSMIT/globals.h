#include <stdint.h>
#define select2 (GPIO_PORTB_DATA_R & 0x01)
#define select1 (GPIO_PORTB_DATA_R & 0x10)
#define pause (GPIO_PORTB_DATA_R & 0x02)
#define north 0
#define east 1
#define south 2
#define west 3

extern int ADC_Status;
extern int ADC_Mailbox;
extern int sine;
extern uint8_t transdata;
extern uint8_t Fifo_Arr[10];
extern int GetI;
extern int PutI;
extern uint32_t input;
extern int fifostatus;
extern int RxCounter;
extern int TxCounter;
extern uint32_t FiFo_Put(uint8_t data);
extern int digital;
//above is ADC/UART/DAC stuff

extern int delay;
extern uint32_t speed;
extern void Systick_Init(int sysreload);
extern void Systick(uint32_t sysreload);
extern int sysreload;
extern void Output_Init(void);

extern int delaytime;
extern void P1Bike(void);
extern void P2Bike(void);

extern void ButtonEdge_Init(void);
extern int rightb1;
extern int leftb1;
extern int rightb2;
extern int leftb2;
extern char Arena_Flag;

extern int Arena_Map(int R1, int L1, int R2, int L2);
extern void Set_Flag(void);
extern void Timer0_Init(void(*task)(void), uint32_t period);

extern int h1;
extern int v1;
extern int h2;
extern int v2;
extern int P1direction;
extern int P2direction;
extern int boost1;
extern int boost2;
extern int boost1count;
extern int boost2count;
//images 
extern const unsigned short pixel1[];
extern const unsigned short pixel2[];

extern const unsigned short tronhome[];
extern const unsigned short tronboot[];
extern const unsigned short selectw[] ;
extern const unsigned short cycle1[];
extern const unsigned short gameover[];
extern const unsigned short cycle2[] ;
extern const unsigned short bootdisc[];
extern const unsigned short win[];
extern int collisions(uint8_t x , uint8_t y);
extern void Delay(uint32_t count);
