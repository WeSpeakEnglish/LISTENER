#include "timers.h"
#include "stm32F10x.h"
#include "keyboard.h"
#include "touchscreen.h"
#include "lighting.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_tim.h"
#include "encoder.h"
#include "IR_control.h"


#define encoderStepsPerRevolution 64

u8 encoder_capture_is_ready = 0;


void TIM1_init(void){
TIM_Cmd(TIM1, DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);


  TIM_TimeBaseInitStruct.TIM_Period = 65535;//encoderStepsPerRevolution - 1;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
  TIM_Cmd(TIM1, DISABLE);


  TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

  TIM_ClearFlag(TIM1,TIM_FLAG_Update);
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
  TIM1->CNT = 32768;
  TIM_Cmd(TIM1, ENABLE);
}

void TIM2_init(void)
{
   RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //��������� ������ �������� ��������� �� TIM2
   TIM2->CNT = 0;
   TIM2->SR = 0;
   TIM2->CR2=0x0;          //�������� �������
   TIM2->CR1=0x0;          //�������� �������?

   TIM2->PSC = 0;          // ������������ ������� 2
   TIM2->ARR = 2399;      //

 //  TIM2->CR1 &= ~TIM_CR1_DIR;      //������ ���� - 0 (�������� - 1) ������
 //  TIM2->CR1 &= ~TIM_CR1_UDIS;      //����.-1; ��������� ������� �� �������.������� -0
 //  TIM2->CR1 &= ~TIM_CR1_CKD;      //clock division
 //  TIM2->CR1 &= ~TIM_CR1_ARPE;      //��������� 0 - ����� �������� �� ���� ������ �������� ARR;���������=1 �����������
 //  TIM2->CR1 &= ~TIM_CR1_OPM;      //��������� ������ � ������ ���������� ��������
   TIM2->DIER |= TIM_DIER_UIE;      //&UPDATE INTERRPUT - ���������� �� ������������ &&&

   TIM2->CR1 |= TIM_CR1_URS;
   TIM2->EGR &= ~TIM_EGR_UG;      // ���������� 0 ;update enable  ARR � ��������� �������� �� ���������������� � ������� �������
 //  TIM2->CR1 |= TIM_CR1_CEN;       //��������� ������2
   NVIC_EnableIRQ (TIM2_IRQn);    //��������� ����������
  
}



void TIM4_init(void){
RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;   //��������� ������ �������� ��������� �� TIM4
TIM4->CR1 &=~ TIM_CR1_CEN;

  TIM4->PSC = 999;  //50 uS tick
  TIM4->ARR = 1900;//overflow at 255; 
  TIM4->CR1 &= ~TIM_CR1_CEN;  
  TIM4->DIER |= TIM_DIER_UIE;
//  TIM4->CR1 |= TIM_CR1_CEN;
  NVIC_EnableIRQ (TIM4_IRQn);    //��������� ����������

}

//#define FORWARD 0
//#define BACKWARD 1
/*
void TIM1_CC_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

      encoder_capture_is_ready = 1;

 //   captured_direction = (TIM1->CR1 & TIM_CR1_DIR ? FORWARD : BACKWARD);
  }
}
*/
//TIM1->CNT


void TIM2_IRQHandler(void)
{
static  u8 InsideState = 0; 
  TIM2->SR &= ~TIM_SR_UIF;        // ������� ���� ���������� 
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




void IWDG_init(void){
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
 IWDG_SetPrescaler(IWDG_Prescaler_32); //32000/32 = 1 kHz clock
 IWDG_SetReload(999); //1 sek
 IWDG_ReloadCounter();
 IWDG_Enable();
}

void SysTick_Handler (void)
 {
 static u8 InsideState = 0;
 switch(InsideState){
    case 0: 
      SetLineKbd(0);
      break;
    case 1:  
      TestEnSwitch();// read SW Encoder 
      break;
    case 2:
      ReadLineKbd(0);
      break;
    case 3: 
      TestEnTurn();
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

  Update_IR_Repetative();

  
  
 }

