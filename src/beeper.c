#include "beeper.h"


void BeeperOn(u8 Beep){

  if (!Beep)  
  GPIOA->BSRR |=  GPIO_BSRR_BR10;
  else
  GPIOA->BSRR |=  GPIO_BSRR_BS10;



}