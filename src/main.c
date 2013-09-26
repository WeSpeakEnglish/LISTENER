#include "stm32F10x.h"
#include "stm32f10x_i2c.h"

#include "delays.h"
#include "timers.h"
#include "ioconfig.h"
#include "timingconfig.h"
#include "lighting.h"



GPIO_TypeDef * PORT_Temp;

int main(void)
{ 
 ClockInit();    
 ConfigIO(); // i/î ports configure  

 LEDs_On(4);
 TIM4_init(); // Start KeyBoard Scanning

 //InterruptCS_On();    

 
//     TIM2_init();  // for TEST SIGNAL
//     ButtonConf(); // button enable/disable signal
//     Init_UART1();  
   

     while(1){
 
     }   

}