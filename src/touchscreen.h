#ifndef _touchscreen_h
#define _touchscreen_h

#include "stm32f10x.h"

extern u8 PressDownTS;
extern u8 PressUpTS;

void GenerateTSMessages(u8 Event);
void EXTI1_IRQHandler(void);
#endif