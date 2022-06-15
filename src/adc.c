#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "adc.h"
#include "queue.h"
#include "beeper.h"
#include "lighting.h"

//#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define PC14_IN (*((volatile unsigned long *) 0x42220138)) //bit-band of PC14 INput

u16 ADC_VAL[16];
u8 ADCdataReady = 0;
u8 CheckVoltagesError = 0;
 union{
  struct VoltErr bits;
  u8 Byte;
  }VErr; 

void ADC_config(void){
 
   GPIO_InitTypeDef GPIO_InitStructure;
   DMA_InitTypeDef DMA_InitStructure; //Variable used to setup the DMA
   ADC_InitTypeDef ADC_InitStructure;  
   
   
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   


        //--Enable DMA1 clock--
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  

        //==Configure DMA1 - Channel1==
           DMA_DeInit(DMA1_Channel1); //Set DMA registers to default values
           DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR; //Address of peripheral the DMA must map to
           DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) & ADC_VAL; //Variable to which ADC values will be stored
           DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
           DMA_InitStructure.DMA_BufferSize = 7; //Buffer size ( because we using 7 channels)
           DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
           DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
           DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
           DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
           DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
           DMA_InitStructure.DMA_Priority = DMA_Priority_Medium ;
           DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

           DMA_Init(DMA1_Channel1, &DMA_InitStructure); //Initialise the DMA



           //==Configure ADC1 - Channels
              RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
              ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
              ADC_InitStructure.ADC_ScanConvMode = ENABLE;
              ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
              ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO; //
              ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
              ADC_InitStructure.ADC_NbrOfChannel = 7; //We using 7 channels

              ADC_Init(ADC1, &ADC_InitStructure); //Initialise ADC1

              //Setup order in which the Channels are sampled....

              ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
              ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
              ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
              ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
              ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_239Cycles5);
              ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_239Cycles5);
              ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 7, ADC_SampleTime_239Cycles5);
              
//       ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 7, ADC_SampleTime);
             RCC_ADCCLKConfig( RCC_PCLK2_Div8);

             ADC_Cmd(ADC1, ENABLE); //Enable ADC1  
              


              //==Calibrate ADC1==

                 //Enable ADC1 reset calibaration register
                 ADC_ResetCalibration(ADC1);
                 while (ADC_GetResetCalibrationStatus(ADC1)); //Check the end of ADC1 reset calibration register

                 //Start ADC1 calibaration
                 ADC_StartCalibration(ADC1);
                 while (ADC_GetCalibrationStatus(ADC1)); //Check the end of ADC1 calibration
       
            
            DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
            DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
            DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, ENABLE);
            NVIC_EnableIRQ(DMA1_Channel1_IRQn); 
            AD_SetTimer(8, 50);
              /* Enable ADC1 DMA */
  

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);               

}
/*
	// timer period = 24000000 Hz /240/200 = 500Hz
	AD_SetTimer(240, 200);
*/
void AD_SetTimer(uint16_t prescaler, uint16_t period)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {
		.TIM_ClockDivision = 0x0,
		.TIM_CounterMode = TIM_CounterMode_Up,
  	.TIM_RepetitionCounter = 0x0000
	};

	TIM_DeInit(TIM3);

	if ((period <= 3) || (prescaler <= 1)) {
		return;
	}

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Period = period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_Cmd(TIM3, ENABLE);

}

void TIM3_IRQHandler(void)
{
  TIM3->SR &= ~TIM_SR_UIF;        // очищаем флаг прерывания 
  return;
}

void DMA1_Channel1_IRQHandler(void){    

    if (DMA_GetITStatus(DMA1_IT_HT1))    {

        DMA_ClearITPendingBit(DMA1_IT_HT1);    
    }
    if (DMA_GetITStatus(DMA1_IT_TC1))    {
        ADCdataReady = 1;
        DMA_ClearITPendingBit(DMA1_IT_TC1);    
    }    
}

u8 AD_CheckVoltages(void){
  static u32 voltageBase = 0;
  static u32 ValueFromADC = 0;
//  static s8 readyToMeasure = -1; // a delay before start measure
  static u8 OldValue = 0; 
  
  if (ADCdataReady){
    ADCdataReady = 0;
 //  if(readyToMeasure > 0){
      if(!voltageBase){
        voltageBase = 2500000/ADC_VAL[4];
 
        VErr.Byte = 0;
        OldValue = 0;
      }
    // check our voltages here
      
     ValueFromADC = voltageBase << 12; // here 3.3V
     if ((ValueFromADC > plus_3V_high_limit)||(ValueFromADC < plus_3V_low_limit)){ 
      VErr.bits.V3Err = 1;
      LedOK_On(0);
      LedErr_On(1);
      BeeperOn(1);
     }
     else 
      VErr.bits.V3Err = 0;  
     
     ValueFromADC = voltageBase * ADC_VAL[0]; // here 5V
     if ((ValueFromADC > plus_5V_high_limit)||(ValueFromADC < plus_5V_low_limit)) 
      VErr.bits.V5Err = 1;
     else 
      VErr.bits.V5Err = 0; 
 
     ValueFromADC = voltageBase * ADC_VAL[1]; // here +5VA
     if ((ValueFromADC > plus_5VA_high_limit)||(ValueFromADC < plus_5VA_low_limit)) 
      VErr.bits.Vplus5Err = 1;
     else 
      VErr.bits.Vplus5Err = 0; 
    
     ValueFromADC = voltageBase * ADC_VAL[2]; // here 12V
     if ((ValueFromADC > plus_12V_high_limit)||(ValueFromADC < plus_12V_low_limit)) 
      VErr.bits.V12Err = 1;
     else 
      VErr.bits.V12Err = 0;     
         
     ValueFromADC = voltageBase * ADC_VAL[3]; // here +15VA
     if ((ValueFromADC > plus_15VA_high_limit)||(ValueFromADC < plus_15VA_low_limit)) 
      VErr.bits.Vplus15Err = 1;
     else 
      VErr.bits.Vplus15Err = 0; 
         
     ValueFromADC = voltageBase * ADC_VAL[5]; // here -5VA
     if ((ValueFromADC > minus_5VA_high_limit)||(ValueFromADC < minus_5VA_low_limit)) 
      VErr.bits.Vminus5Err = 1;
     else 
      VErr.bits.Vminus5Err = 0;   
     
     ValueFromADC = voltageBase * ADC_VAL[6]; // here -15VA
     if ((ValueFromADC > minus_15VA_high_limit)||(ValueFromADC < minus_15VA_low_limit)) 
      VErr.bits.Vminus15Err = 1;
     else 
      VErr.bits.Vminus15Err = 0;  
     
     ValueFromADC = PC14_IN;
     if (ValueFromADC)
       VErr.bits.VisoErr =0;
     else 
       VErr.bits.VisoErr =1; 
     
  //  }
  //  else{
  //  readyToMeasure++;    
  //  }
    
    if(OldValue != VErr.Byte){
     OldValue = VErr.Byte;
    CheckVoltagesError = 1; 
    } 
  }
return VErr.Byte;
}

void GenerateCheck_V_Msg(void){
 // static u8 OldCode = 0;
  static union QueueElement_U QueueElem;
  
  //if(Code != OldCode){
  //  OldCode = Code;
  //  CheckVoltagesError = 0;
 
      QueueElem.QueEl.EventType = 4;              //Error
      QueueElem.QueEl.Keyb_Key =  VErr.Byte;        // what key number?
      QueueElem.QueEl.RCU_Key =   0;
      
      queue_put_elem(&QueueElem.Bits);
 
  //}
return;
}