#include "stm32F10x.h"
#include "delays.h"
#include "timers.h"
#include "ioconfig.h"
#include "timingconfig.h"
#include "lighting.h"
#include "I2Croutines.h"



int main(void)
{ 
 ClockInit();    
 ConfigIO(); // i/� ports configure  
 ConfigI2C();                                   // configure I2C first
 
 LEDs_On(4);
 LedOK_On(1);
 TIM4_init(); // Start KeyBoard Scanning

     while(1){
 
     }   

}