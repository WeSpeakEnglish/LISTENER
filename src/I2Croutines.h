#ifndef _I2Croutines_h
#define _I2Croutines_h
#include "stm32f10x.h"

extern u8 Status;

void ConfigI2C(void);
void ProcessCommand(u8* pCommand);
void SolveCommand(u8);
#endif
