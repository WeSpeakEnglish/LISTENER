#include "touchscreen.h"
#include "queue.h"
static u8 EdgeTS = 0; // edge touch screen;
u8 PressDownTS = 0;
u8 PressUpTS = 0;

void GenerateTSMessages(u8 Event){

 static union QueueElement_U QueueElem;
 
      QueueElem.EventType = Event;              //pressed eq 0 or unpressed (eq1)
      QueueElem.Keyb_Key = 0x40;        // what key number?
      QueueElem.RCU_Key = 0;
      
      queue_put_elem(&QueueElem.Bits);
 
return;
}

#define PA1_IN (*((volatile unsigned long *) 0x42210104 )) //bit-band of PA1

void EXTI1_IRQHandler(void)
{
 static u8 InsideCounter = 0;

EXTI->PR|=0x01; // clear interrupt

 switch(EdgeTS){
 case 0: 
   PressDownTS = 1;
   EXTI->RTSR|=EXTI_RTSR_TR1; 
   EXTI->FTSR&=~EXTI_FTSR_TR1;
   EdgeTS =1;
   break;
 case 1: 
   PressUpTS = 1;
   EXTI->RTSR&=~EXTI_RTSR_TR1; 
   EXTI->FTSR|=EXTI_FTSR_TR1;
   EdgeTS=0;
 
 }

 
}



