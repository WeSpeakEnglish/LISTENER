#ifndef _timers_h
#define _timers_h
#include "stm32f10x.h"






void TIM2_IRQHandler(void);
void TIM2_init(void); // use for pulse generation
void TIM3_init(void);
void TIM4_init(void);
void IWDG_init(void);
#endif


