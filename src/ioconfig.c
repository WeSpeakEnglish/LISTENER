#include "ioconfig.h"
#include "stm32F10x.h"
#include "stm32f10x_adc.h"



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
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;            //clock GPIOA
  RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN; // alternate function
  /// ADC////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  

        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
   //PA8 As DIG IN

 //  GPIOA->CRH&=~( GPIO_CRH_CNF8_0);
 //  GPIOA->CRH|=(GPIO_CRH_MODE8_1);
 //  GPIOA->CRH|=(GPIO_CRH_MODE8_0);

 //PA10 PA12 As Push-Pull 2 MHz
     GPIOA->CRH&=~(GPIO_CRH_CNF10_0|GPIO_CRH_CNF12_0);
     GPIOA->CRH|=(GPIO_CRH_MODE10_1|GPIO_CRH_MODE12_1);
     GPIOA->CRH&=~(GPIO_CRH_MODE10_0|GPIO_CRH_MODE12_0);    
 
  
    //// DISABLE JTAG Legs
    

AFIO -> MAPR |= AFIO_MAPR_SWJ_CFG_1;   

//PA15 Open Drain at 50 MHz
    GPIOA->CRH|=(GPIO_CRH_CNF15_0);
    GPIOA->CRH&=~(GPIO_CRH_CNF15_1);
    GPIOA->CRH|=(GPIO_CRH_MODE15_1|GPIO_CRH_MODE15_0);
  
    
    
//---------------------------------------------------------------------------------------------------------
    

  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;            // тактирование линий GPIOB
 // RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN; // alternate function
   
  //PB0 Push-Pull
  GPIOB->CRL&=~(GPIO_CRL_CNF0_0);
  GPIOB->CRL|=(GPIO_CRL_MODE0_1);
  GPIOB->CRL&=~(GPIO_CRL_MODE0_0);  
  GPIOB->BSRR=GPIO_BSRR_BR0;
    
  //PB1 as DIG input
  GPIOB->CRL &=~GPIO_CRL_MODE1;
  GPIOB->CRL &=~GPIO_CRL_CNF1;
  GPIOB->CRL|=GPIO_CRL_CNF1_0; //floating input
  
  //PB2 as output to zero
  GPIOB->CRL&=~(GPIO_CRL_CNF2_0);
  GPIOB->CRL|=(GPIO_CRL_MODE2_1);
  GPIOB->CRL&=~(GPIO_CRL_MODE2_0);  
  GPIOB->BSRR=GPIO_BSRR_BR2;

  //PB3 as output to zero
  GPIOB->CRL&=~(GPIO_CRL_CNF3_0);
  GPIOB->CRL|=(GPIO_CRL_MODE3_1);
  GPIOB->CRL&=~(GPIO_CRL_MODE3_0);  
  GPIOB->BSRR=GPIO_BSRR_BR3;
  
  //PB4 As Input 
  GPIOB->CRL &=~GPIO_CRL_MODE4;
  GPIOB->CRL &=~GPIO_CRL_CNF4;
  GPIOB->CRL |=GPIO_CRL_CNF4_1; //pull 
  GPIOB->BSRR |=  GPIO_BSRR_BS4; //up set to 1 
  
  //PB5 out push-pull
   GPIOB->BSRR=GPIO_BSRR_BR5;
   GPIOB->CRL&=~(GPIO_CRL_CNF5_0);
   GPIOB->CRL|=(GPIO_CRL_MODE5_1);
   GPIOB->CRL&=~(GPIO_CRL_MODE5_0);  
   
  
  //Lines PB6...PB9 Open Drain
  GPIOB->CRL|=(GPIO_CRL_CNF6_0|GPIO_CRL_CNF7_0);
  GPIOB->CRH|=(GPIO_CRH_CNF8_0|GPIO_CRH_CNF9_0);
  GPIOB->CRL&=~(GPIO_CRL_CNF6_1|GPIO_CRL_CNF7_1);
  GPIOB->CRH&=~(GPIO_CRH_CNF8_1|GPIO_CRH_CNF9_1);
  GPIOB->CRL|=(GPIO_CRL_MODE6_1|GPIO_CRL_MODE6_0);
  GPIOB->CRL|=(GPIO_CRL_MODE7_1|GPIO_CRL_MODE7_0);
  GPIOB->CRH|=(GPIO_CRH_MODE8_1|GPIO_CRH_MODE8_0);
  GPIOB->CRH|=(GPIO_CRH_MODE9_1|GPIO_CRH_MODE9_0);
  
   //PB12..PB15 as Inputs    
  GPIOB->CRH &=~GPIO_CRH_MODE12;
  GPIOB->CRH &=~GPIO_CRH_CNF12;
  GPIOB->CRH|=GPIO_CRH_CNF12_0; //floating input
  GPIOB->CRH &=~GPIO_CRH_MODE13;
  GPIOB->CRH &=~GPIO_CRH_CNF13;
  GPIOB->CRH|=GPIO_CRH_CNF13_0; //floating input
  GPIOB->CRH &=~GPIO_CRH_MODE14;
  GPIOB->CRH &=~GPIO_CRH_CNF14;
  GPIOB->CRH|=GPIO_CRH_CNF14_0; //floating input
  GPIOB->CRH &=~GPIO_CRH_MODE15;
  GPIOB->CRH &=~GPIO_CRH_CNF15;
  GPIOB->CRH|=GPIO_CRH_CNF15_0; //floating input
 
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






