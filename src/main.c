#include "timers.h"
#include "ioconfig.h"
#include "timingconfig.h"
#include "lighting.h"
#include "I2Croutines.h"
#include "events.h"
#include "dac.h"
#include "adc.h"
#include "defines.h"


int main(void)
{ 
 ClockInit();    
 IWDG_init(); //WATCH DOG
   
 ConfigIO(); // i/î ports configure  
 ConfigureDAC1();
 ADC_config();
 ConfigI2C();
 // configure I2C first
 LedOK_On(1); 
 LEDs_On(4);
 LedErr_On(0);
 TIM1_init(); // encoder support
 TIM2_init(); //Pulse generation 
 TIM4_init(); // for IRDA usage
 SysTick_Config(11999); // Start KeyBoard Scanning
 IRDA_Int_Conf(); // Configure IRDA Interrupt
 Touch_Int_Conf();

     while(1){
 ReleaseEvents();
#ifndef _RECEIVER
 AD_CheckVoltages(); //IF WE dont need to control voltages (in RECEIVER for EXAMPLE) - just comment this string
#endif 
//GPIOA->BSRR |=  GPIO_BSRR_BS10; //up set to 1 
     }   

}

