#include "delays.h"

#define Del40 70000

void delay_40ms(void){
  u32 delay=0;
for(delay=Del40;delay;delay--);
  return;
}
void delay_1s(void){
   u32 delay=0;
for(delay=25;delay>0;delay--)delay_40ms(); 
}