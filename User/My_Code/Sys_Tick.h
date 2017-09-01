#ifndef	_SYS_TICK_H_
#define _SYS_TICK_H_

#include	"stm32f10x.h"

void Sys_Tick_Init(void);
void SysTick_Handler(void);
void Delay_us(u32 nUS);
void Delay_ms(u32 nMS);




#endif

