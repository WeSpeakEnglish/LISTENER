#ifndef _keyboard_h
#define _keyboard_h
#include "stm32f10x.h"


struct Keys{
  unsigned int COL_0 : 6;
  unsigned int COL_1 : 6;
  unsigned int COL_2 : 6;
  unsigned int COL_3 : 6;  
};

union Keys_U{
  struct Keys Keys;
  u32 Bits;
};

extern u8 ReadyFlagKb; // is Event detected?

void SetLineKbd(u8 Number);
void ReadLineKbd(u8 Number);
u8 SolvePressedKeys(void);
void GenerateKbdMessages(u32 XORedKeyBits);
#endif
