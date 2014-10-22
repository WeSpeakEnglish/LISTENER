#include "touchscreen.h"
#include "queue.h"

u8 PressDownTS = 0;
u8 PressUpTS = 0;

void GenerateTSMessages(u8 Event){

 static union QueueElement_U QueueElem;
 
      QueueElem.QueEl.EventType = Event;              //pressed eq 0 or unpressed (eq1)
      QueueElem.QueEl.Keyb_Key = 0x40;        // what key number?
      QueueElem.QueEl.RCU_Key = 0;
      
      queue_put_elem(&QueueElem.Bits);
 
return;
}

#define PA1_IN (*((volatile unsigned long *) 0x42210104)) //bit-band of PA1
#define PB4_IN (*((volatile unsigned long *) 0x42218110)) //bit-band of PB4_IN 

void EXTI1_IRQHandler(void)
{
 static u8 EdgeTS = 0; // edge touch screen;
 EXTI->PR |= 2;//(1<<1); // clear interrupt

 switch(EdgeTS){
 case 0: 
//   PressDownTS = 1;
   EXTI->RTSR|=EXTI_RTSR_TR1; 
   EXTI->FTSR&=~EXTI_FTSR_TR1;
   EdgeTS =1;
   break;
 case 1: 
//   PressUpTS = 1;
   EXTI->RTSR&=~EXTI_RTSR_TR1; 
   EXTI->FTSR|=EXTI_FTSR_TR1;
   EdgeTS=0;
 
 }
 
}

void TestTSPin(void){ // testing pin, were touchscreen for edge
static u8 EdgeTS = 1; // edge touch screen;
static u8 PinState = 1;
PinState = PB4_IN;
if(PinState != EdgeTS){
  EdgeTS = !EdgeTS;
  if(PinState)PressUpTS = 1;
  else PressDownTS = 1;
}

return;
}


