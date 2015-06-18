#ifndef _lighting_h
#define _lighting_h
#include "stm32f10x.h"
#ifndef CURRENT
//#define CURRENT 60000 //60000uA IF WE load firmware to RECEIVER  - just change this 2 strings 
#define CURRENT 40000 //uA 40mA 
#endif
void LEDs_On(u8 DAC_value); // lighting LCD
void LedOK_On(u8 Lighted);       // Led "OK"
void LedErr_On(u8 Lighted); // LED "False"

//extern u8 LedFaultOn;
//extern u8 LedOkOn;
#endif
