#include "lighting.h"



void LEDs_On(u8 DAC_value){
 // if (DAC_value < MAX_DAC_value){
 // DAC->DHR8R1 = DAC_value;
  switch(DAC_value){
  case 0: DAC->DHR8R1 = 0; break;  
  case 1: DAC->DHR8R1 = CURRENT/1000; break;
  case 2: DAC->DHR8R1 = CURRENT/600; break;
  case 3: DAC->DHR8R1 = CURRENT/400; break;
  case 4: DAC->DHR8R1 = CURRENT/300; break;   
  default: DAC->DHR8R1 = DAC_value; 
 // }
  } 
return;
}


void LedOK_On(u8 Lighted){
 if(Lighted) GPIOA->ODR&=~GPIO_ODR_ODR15;// log 0 to line (LED ON) 
 else        GPIOA->ODR|=GPIO_ODR_ODR15;// log 1 to exit (LED off)
return;
}

void LedErr_On(u8 Lighted){
  if(Lighted)
  GPIOA->ODR&=~GPIO_ODR_ODR12;// log 0 to line (LED ON)
 else        GPIOA->ODR|=GPIO_ODR_ODR12;// log 1 to exit (LED off)
return;
}

