#ifndef _lighting_h
#define _lighting_h
#include "stm32F10x.h"

void LEDs_On(u8 DAC_value); // lighting LCD
void LedOK_On(u8 Lighted);       // Led "OK"
void LedErr_On(u8 Lighted); // LED "False"

//extern u8 LedFaultOn;
//extern u8 LedOkOn;
#endif