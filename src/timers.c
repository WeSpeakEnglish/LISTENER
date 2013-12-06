#include "timers.h"
#include "stm32F10x.h"
#include "keyboard.h"
//#include ""


void TIM2_init(void)
{
  /*
   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //разрешить подачу тактовых импульсов на TIM2
   TIM2->CNT = 0;
   TIM2->SR = 0;
   TIM2->CR2=0x0;          //обнуляем регистр
   TIM2->CR1=0x0;          //обнуляем регистр?

   TIM2->PSC = 12000;          // предделитель таймера 2
   TIM2->ARR = 999;      //

   TIM2->CR1 &= ~TIM_CR1_DIR;      //прямой счёт - 0 (обратный - 1) отсчёт
   TIM2->CR1 &= ~TIM_CR1_UDIS;      //запр.-1; разрешаем событие по перепол.таймера -0
   TIM2->CR1 &= ~TIM_CR1_CKD;      //clock division
   TIM2->CR1 &= ~TIM_CR1_ARPE;      //запрещаем 0 - когда прграмно не надо менять значение ARR;разрешаем=1 перзагрузку
   TIM2->CR1 &= ~TIM_CR1_OPM;      //Запрещаем работу в режиме одиночного импульса
   TIM2->DIER |= TIM_DIER_UIE;      //&UPDATE INTERRPUT - прерывание по переполнению &&&

   TIM2->CR1 |= TIM_CR1_URS;
   TIM2->EGR &= ~TIM_EGR_UG;      // сбрасываем 0 ;update enable  ARR и прескалер грузятся из предварительного в теневой регистр
   TIM2->CR1 |= TIM_CR1_CEN;       //запускаем таймер2
        NVIC_EnableIRQ (TIM2_IRQn);    //разрешаем прерывание
  */
  
}

void TIM4_init(void){
RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   //разрешить подачу тактовых импульсов на TIM7
TIM4->CR1 &=~ TIM_CR1_CEN;

  TIM4->PSC = 0; 
  TIM4->ARR = 23999;
  NVIC_EnableIRQ (TIM4_IRQn);    //разрешаем прерывание
  TIM4->DIER |= TIM_DIER_UIE;
  TIM4->CR1 |= TIM_CR1_CEN;
}




void TIM4_IRQHandler(void)
{
  static u8 InsideState = 0;
 
  
   TIM4->SR &= ~TIM_SR_UIF;        // очищаем флаг прерывания 

  switch(InsideState){
    case 0: 
      SetLineKbd(0);
      break;
    case 1:
      ReadLineKbd(0);
      break;
    case 2: 
      SetLineKbd(1);
      break;
    case 3:
      ReadLineKbd(1);
      break;
    case 4: 
      SetLineKbd(2);
      break;
    case 5:
      ReadLineKbd(2);
      break;
    case 6: 
      SetLineKbd(3);
      break;
    case 7:
      ReadLineKbd(3);  
      ReadyFlagKb = 1;  
   }
  InsideState++;
  InsideState %= 8; 
  return;
}

