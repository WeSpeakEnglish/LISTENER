#include "ioconfig.h"
#include "stm32F10x.h"



void GPIO_Int_Conf(void){
  RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;    //
  AFIO->EXTICR[0]&=~AFIO_EXTICR1_EXTI0_PA; //EXTI0  (A0 as result)
  EXTI->IMR|=EXTI_IMR_MR0;                           //
  EXTI->RTSR|=EXTI_RTSR_TR0;                          //
  NVIC_EnableIRQ (EXTI0_IRQn);          //

return;
}

void IRDA_Int_Conf(void){
  RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;    //
  AFIO->EXTICR[2]&=~AFIO_EXTICR1_EXTI2_PA; //EXTI2  (A2 as result)
  EXTI->IMR|=EXTI_IMR_MR2;                           //
  EXTI->RTSR|=EXTI_RTSR_TR2;                          //
  NVIC_EnableIRQ (EXTI2_IRQn);          //

return;
}

void Touch_Int_Conf(void){
  NVIC_InitTypeDef  NVIC_InitStructure;
  
  RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;    //
  AFIO->EXTICR[1]&=~AFIO_EXTICR1_EXTI1_PA; //EXTI1  (A1 as result)
  EXTI->IMR|=EXTI_IMR_MR1;                           //
  
   EXTI->RTSR&=~EXTI_RTSR_TR1; 
   EXTI->FTSR|=EXTI_FTSR_TR1;
 // EdgeTS=0;
   
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //set low
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
                          //
  //NVIC_EnableIRQ (EXTI1_IRQn);          //

return;
}
/*
void InterruptCS_On(void){
 NVIC_SetPriority(EXTI9_5_IRQn, 15);
 NVIC_EnableIRQ(EXTI0_IRQn);
 EXTI->IMR &= ~EXTI_IMR_MR0;
 EXTI->EMR &= ~EXTI_EMR_MR0;  
 RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;              // Alternative functions clocking
 AFIO->EXTICR[2]|=AFIO_EXTICR3_EXTI8_PC;        // AFIO_EXTICR3 
 EXTI->FTSR |= EXTI_FTSR_TR8; // falling edge 
 EXTI->RTSR |= EXTI_RTSR_TR8; //and rising edge
 
 EXTI->IMR|=EXTI_IMR_MR8;                       // Line 8 in mask (enable interrupt)
 NVIC_EnableIRQ (EXTI9_5_IRQn); 
}
*/


void ConfigIO(void){
  //------------------------------------------------------------------------------------------------------
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;            //тактирование линий GPIOA
  
  //PA2
  GPIOA->CRL|=(GPIO_CRL_CNF1_1);
  GPIOA->CRL&=~(GPIO_CRL_CNF1_0);
  GPIOA->CRL&=~(GPIO_CRL_MODE1_1); 
  GPIOA->CRL&=~(GPIO_CRL_MODE1_0);
  GPIOA->ODR|=(GPIO_ODR_ODR1);
  
   //PA5 PA6 PA7 PA8 As Push-Pull 50 MHz
   GPIOA->CRL&=~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF6_0|GPIO_CRL_CNF7_0);
   GPIOA->CRL|=(GPIO_CRL_MODE5_1 | GPIO_CRL_MODE6_1|GPIO_CRL_MODE7_1);
   GPIOA->CRL|=(GPIO_CRL_MODE5_0 | GPIO_CRL_MODE6_0|GPIO_CRL_MODE7_0);
   GPIOA->CRH&=~( GPIO_CRH_CNF8_0);
   GPIOA->CRH|=(GPIO_CRH_MODE8_1);
   GPIOA->CRH|=(GPIO_CRH_MODE8_0);

 //PA9 PA10 PA11 PA12 As Push-Pull 2 MHz
    GPIOA->CRH&=~(GPIO_CRH_CNF9_0 | GPIO_CRH_CNF10_0|GPIO_CRH_CNF11_0|GPIO_CRH_CNF12_0);
    GPIOA->CRH|=(GPIO_CRH_MODE9_1 | GPIO_CRH_MODE10_1|GPIO_CRH_MODE11_1|GPIO_CRH_MODE12_1);
    GPIOA->CRH&=~(GPIO_CRH_MODE9_0 | GPIO_CRH_MODE10_0|GPIO_CRH_MODE11_0|GPIO_CRH_MODE12_0);    
 
    
    //// DISABLE JTAG Legs
    
RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN;
AFIO -> MAPR |= AFIO_MAPR_SWJ_CFG_1;   

//PA15 Open Drain at 50 MHz
    GPIOA->CRH|=(GPIO_CRH_CNF15_0);
    GPIOA->CRH&=~(GPIO_CRH_CNF15_1);
    GPIOA->CRH|=(GPIO_CRH_MODE15_1|GPIO_CRH_MODE15_0);
  
    
    
//---------------------------------------------------------------------------------------------------------
    

  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;            // тактирование линий GPIOB
  //Lines PB0...PB3 Open Drain
    GPIOB->CRL|=(GPIO_CRL_CNF0_0|GPIO_CRL_CNF1_0|GPIO_CRL_CNF2_0|GPIO_CRL_CNF3_0);
    GPIOB->CRL&=~(GPIO_CRL_CNF0_1|GPIO_CRL_CNF1_1|GPIO_CRL_CNF2_1|GPIO_CRL_CNF3_1);
    GPIOB->CRL|=(GPIO_CRL_MODE0_1|GPIO_CRL_MODE0_0|GPIO_CRL_MODE1_1|GPIO_CRL_MODE1_0|
                 GPIO_CRL_MODE2_1|GPIO_CRL_MODE2_0|GPIO_CRL_MODE3_1|GPIO_CRL_MODE3_0); 
}

void EXTI0_IRQHandler(void)
{
 static u8 InsideCounter = 0;
  
   if (InsideCounter%2)
    {
   NVIC_EnableIRQ (TIM2_IRQn);    //разрешаем прерывание таймера
    }
    else
    {
   NVIC_DisableIRQ (TIM2_IRQn);    //запретить прерывание таймера

     }  
 EXTI->PR|=0x01; // clear interrupt
 InsideCounter++;
}



void EXTI2_IRQHandler(void)
{
 static u8 InsideCounter = 0;

 EXTI->PR|=0x01; // clear interrupt
 InsideCounter++;
}






