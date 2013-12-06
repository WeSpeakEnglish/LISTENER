#ifndef _keyboard_h
#define _keyboard_h
#include "stm32f10x.h"


struct Keys{
  unsigned int COL_0 : 4;
  unsigned int COL_1 : 4;
  unsigned int COL_2 : 4;
  unsigned int COL_3 : 4;  
};

union Keys_U{
  struct Keys;
  u16 Bits;
};

extern u8 ReadyFlagKb; // is Event detected?

void SetLineKbd(u8 Number);
void ReadLineKbd(u8 Number);
void SolvePressedKeys(void);
void GenerateKbdMessages(u16 XORedKeyBits);
#endif