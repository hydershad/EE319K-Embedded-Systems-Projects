#include <stdint.h>
#define pause (GPIO_PORTB_DATA_R & 0x01)
#define select1 (GPIO_PORTE_DATA_R & 0x10)
#define select2 (GPIO_PORTE_DATA_R & 0x20)

extern int ADC_Status;
extern int ADC_Mailbox;
extern int sine;
extern uint8_t data;
extern uint8_t Fifo_Arr[10];
extern int GetI;
extern int PutI;
extern uint32_t input;
extern int fifostatus;
extern int RxCounter;
extern int TxCounter;
extern uint32_t FiFo_Put(uint8_t data);
extern const uint8_t wave[64];
extern void DAC_Out(uint8_t sine);
extern void DAC_Init(void);
extern uint32_t period;
//above is ADC/UART/DAC stuff

extern int delay;

extern void Systick_Init(int sysreload);
extern void Systick(uint32_t sysreload);
extern int sysreload;
extern void Output_Init(void);

extern int delaytime;


extern void Timer0_Init(void(*task)(void), uint32_t period);
extern void Sound_Tron_Theme(void);
extern void (*PeriodicTask0)(void);
extern void Sound_Init(void);
extern void Sound_Play(const uint8_t *pt, uint32_t count);
extern void Sound_Killed(void);
extern void Sound_Shoot(void);
