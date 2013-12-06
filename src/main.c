#include "timers.h"
#include "ioconfig.h"
#include "timingconfig.h"
#include "lighting.h"
#include "I2Croutines.h"
#include "events.h"



int main(void)
{ 
 ClockInit();    
 ConfigIO(); // i/î ports configure  
 ConfigI2C();                                   // configure I2C first
 LEDs_On(4);
 LedOK_On(1);
 TIM4_init(); // Start KeyBoard Scanning
Touch_Int_Conf();
     while(1){
 ReleaseEvents();
     }   

}

