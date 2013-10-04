#include "lighting.h"


void LEDs_On(u8 number_of_LEDs){
  switch(number_of_LEDs){
    case 0:
      GPIOA->ODR &= ~(GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);// log 1 to exit (LED screen off all lines)
      break;
    case 1: 
      GPIOA->ODR &= ~(GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);// log 1 to exit (LED screen on all lines)
      GPIOA->ODR |= (GPIO_ODR_ODR5); //enable one line
      break;
    case 2: 
      GPIOA->ODR &= ~(GPIO_ODR_ODR6|GPIO_ODR_ODR7);// log 1 to exit (LED screen on all lines)
      GPIOA->ODR |= (GPIO_ODR_ODR5|GPIO_ODR_ODR8); //enable two line
      break;
    case 3:
      GPIOA->ODR &= ~(GPIO_ODR_ODR6);// log 1 to exit (LED screen on all lines)
      GPIOA->ODR |= (GPIO_ODR_ODR5|GPIO_ODR_ODR8|GPIO_ODR_ODR7); //enable three lines      
      break;
    case 4:
      GPIOA->ODR |= GPIO_ODR_ODR5|GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8;// log 1 to exit (LED screen on all lines)
      break;
  }
return;
}


void LedOK_On(u8 Lighted){
 if(Lighted) GPIOA->ODR&=~GPIO_ODR_ODR15;// log 0 to line (LED ON) 
 else        GPIOA->ODR|=GPIO_ODR_ODR15;// log 1 to exit (LED off)
return;
}

