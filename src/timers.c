#include "timers.h"
#include "stm32F10x.h"
#include "keyboard.h"
//#include ""


void TIM2_init(void)
{
  /*
   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //��������� ������ �������� ��������� �� TIM2
   TIM2->CNT = 0;
   TIM2->SR = 0;
   TIM2->CR2=0x0;          //�������� �������
   TIM2->CR1=0x0;          //�������� �������?

   TIM2->PSC = 12000;          // ������������ ������� 2
   TIM2->ARR = 999;      //

   TIM2->CR1 &= ~TIM_CR1_DIR;      //������ ���� - 0 (�������� - 1) ������
   TIM2->CR1 &= ~TIM_CR1_UDIS;      //����.-1; ��������� ������� �� �������.������� -0
   TIM2->CR1 &= ~TIM_CR1_CKD;      //clock division
   TIM2->CR1 &= ~TIM_CR1_ARPE;      //��������� 0 - ����� �������� �� ���� ������ �������� ARR;���������=1 �����������
   TIM2->CR1 &= ~TIM_CR1_OPM;      //��������� ������ � ������ ���������� ��������
   TIM2->DIER |= TIM_DIER_UIE;      //&UPDATE INTERRPUT - ���������� �� ������������ &&&

   TIM2->CR1 |= TIM_CR1_URS;
   TIM2->EGR &= ~TIM_EGR_UG;      // ���������� 0 ;update enable  ARR � ��������� �������� �� ���������������� � ������� �������
   TIM2->CR1 |= TIM_CR1_CEN;       //��������� ������2
        NVIC_EnableIRQ (TIM2_IRQn);    //��������� ����������
  */
  
}

void TIM4_init(void){
RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   //��������� ������ �������� ��������� �� TIM7
TIM4->CR1 &=~ TIM_CR1_CEN;

  TIM4->PSC = 0; 
  TIM4->ARR = 23999;
  NVIC_EnableIRQ (TIM4_IRQn);    //��������� ����������
  TIM4->DIER |= TIM_DIER_UIE;
  TIM4->CR1 |= TIM_CR1_CEN;
}




void TIM4_IRQHandler(void)
{
  static u8 InsideState = 0;
 
  
   TIM4->SR &= ~TIM_SR_UIF;        // ������� ���� ���������� 

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

