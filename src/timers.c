#include "timers.h"
#include "stm32F10x.h"
#include "keyboard.h"
#include "touchscreen.h"
#include "lighting.h"
#include "stm32f10x_iwdg.h"

void TIM2_init(void)
{
   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //разрешить подачу тактовых импульсов на TIM2
   TIM2->CNT = 0;
   TIM2->SR = 0;
   TIM2->CR2=0x0;          //обнуляем регистр
   TIM2->CR1=0x0;          //обнуляем регистр?

   TIM2->PSC = 0;          // предделитель таймера 2
   TIM2->ARR = 2399;      //

 //  TIM2->CR1 &= ~TIM_CR1_DIR;      //прямой счёт - 0 (обратный - 1) отсчёт
 //  TIM2->CR1 &= ~TIM_CR1_UDIS;      //запр.-1; разрешаем событие по перепол.таймера -0
 //  TIM2->CR1 &= ~TIM_CR1_CKD;      //clock division
 //  TIM2->CR1 &= ~TIM_CR1_ARPE;      //запрещаем 0 - когда прграмно не надо менять значение ARR;разрешаем=1 перзагрузку
 //  TIM2->CR1 &= ~TIM_CR1_OPM;      //Запрещаем работу в режиме одиночного импульса
   TIM2->DIER |= TIM_DIER_UIE;      //&UPDATE INTERRPUT - прерывание по переполнению &&&

   TIM2->CR1 |= TIM_CR1_URS;
   TIM2->EGR &= ~TIM_EGR_UG;      // сбрасываем 0 ;update enable  ARR и прескалер грузятся из предварительного в теневой регистр
 //  TIM2->CR1 |= TIM_CR1_CEN;       //запускаем таймер2
   NVIC_EnableIRQ (TIM2_IRQn);    //разрешаем прерывание
  
}

void TIM4_init(void){
RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   //разрешить подачу тактовых импульсов на TIM4
TIM4->CR1 &=~ TIM_CR1_CEN;

  TIM4->PSC = 0; 
  TIM4->ARR = 11999;//23999;
  NVIC_EnableIRQ (TIM4_IRQn);    //разрешаем прерывание
  TIM4->DIER |= TIM_DIER_UIE;
  TIM4->CR1 |= TIM_CR1_CEN;
}


void TIM2_IRQHandler(void)
{
static  u8 InsideState = 0; 
  TIM2->SR &= ~TIM_SR_UIF;        // очищаем флаг прерывания 
  InsideState++;
  InsideState%=2;
if(InsideState) 
GPIOB->BSRR |= GPIO_BSRR_BS0;
else{
     GPIOB->BSRR |= GPIO_BSRR_BR0; // reset
     TIM2->CR1 &= ~TIM_CR1_CEN;   
}
return;
}

void TIM4_IRQHandler(void)
{
  static u8 InsideState = 0;

  TIM4->SR &= ~TIM_SR_UIF;        // очищаем флаг прерывания 
//LedErr_On(InsideState%2);
  switch(InsideState){
    case 0: 
      SetLineKbd(0);
      break;
    case 2:
      ReadLineKbd(0);
      break;
    case 4: 
      SetLineKbd(1);
      break;
    case 6:
      ReadLineKbd(1);
      break;
    case 8: 
      SetLineKbd(2);
      break;
    case 10:
      ReadLineKbd(2);
      break;
    case 12: 
      SetLineKbd(3);
      break;
    case 14:
      ReadLineKbd(3);  
      ReadyFlagKb = 1;  
   }
  InsideState++;

  
  InsideState %= 16; 
  TestTSPin();

  return;
}

void IWDG_init(void){
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
IWDG_SetPrescaler(IWDG_Prescaler_32); //32000/32 = 1 kHz clock
IWDG_SetReload(999); //1 sek
 IWDG_ReloadCounter();
 IWDG_Enable();
}

