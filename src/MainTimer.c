#include "stm32f10x.h"
#include "MainTimer.h"
unsigned int wait=0;
void delay_ms(unsigned int mc);
//***************************************************************************
//function ������������� ��������� �������                                 //
//***************************************************************************
void Timer2_Init(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;    //������ ������������ �� TIM6
  TIM2->PSC	    = 2400-1;                //��������� �������� ��� ������������
  TIM2->ARR     = 10;                    //�����������
  TIM2->DIER    = TIM_DIER_UIE;        	 //��������� ������� �� �������
  //��������� ������������ � �������� ������
  TIM2->CR1     = TIM_CR1_CEN | TIM_CR1_ARPE;
  //��������� ���������� �� �������
  NVIC_EnableIRQ (TIM2_IRQn);

}
//***************************************************************************
//function ��������� ���������� �� TIM3                                    //
//***************************************************************************
void TIM2_IRQHandler(void)
{static char tmp=0;

  if(TIM2->SR & TIM_SR_UIF)
  {
    TIM2->SR &= ~TIM_SR_UIF;     //�������� ����
    //��������� �-� delay;
    if(wait) wait--;
   // if(tmp) tmp--;else{ disk_timerproc();tmp=10;}
  }
}
//***************************************************************************
//function �������� � �������������                                        //
//***************************************************************************
void delay_ms(unsigned int mc)
{
  wait = mc;
  while(wait){};
}
