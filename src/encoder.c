#include "queue.h"
#include "timers.h"
#include "encoder.h"
#include "lighting.h"



#define PA10_IN (*((volatile unsigned long *) 0x4221012C)) //bit-band of PA10

 u8 EncButPressDown = 0; 
 u8 EncButPressUp = 0; 
 u8 EncTurnedDown = 0; 
 u8 EncTurnedUp = 0; 

 void GenerateEncMesgButton(u8 Event){

 static union QueueElement_U QueueElem;
 
      QueueElem.QueEl.EventType = Event;              //pressed eq 0 or unpressed (eq1)
      QueueElem.QueEl.Keyb_Key = 0x3E;        // what key number?
      QueueElem.QueEl.RCU_Key = 0;
      
      queue_put_elem(&QueueElem.Bits);
 
return;
}

 void GenerateEncMsgTurn(u8 Event){

 static union QueueElement_U QueueElem;
 
      QueueElem.QueEl.EventType = 0;              //pressed eq 0 or unpressed (eq1)
      if(!Event) QueueElem.QueEl.Keyb_Key = 0x3A;        // what key number?
      else QueueElem.QueEl.Keyb_Key = 0x3B;        // what key number?
      QueueElem.QueEl.RCU_Key = 0;
      // make 2 events ==> Press Down and Up
      queue_put_elem(&QueueElem.Bits);
      QueueElem.QueEl.EventType = 1;              //pressed eq 0 or unpressed (eq1)
      queue_put_elem(&QueueElem.Bits);
 
return;
}


void TestEnSwitch(void){
  
static u8 EdgeEN = 0; // edge touch screen;
static u8 PinState = 0;
PinState = PA10_IN;
if(PinState != EdgeEN){
  EdgeEN = !EdgeEN;
  if(PinState) 
    EncButPressUp = 1;
  else 
    EncButPressDown = 1;
}
  
return;
}

void TestEnTurn(void){
  
static u16 TimerEN = 32768; // edge touch screen;
static u16 TimerState = 0;
//static u8 Test = 0;

TimerState =  TIM1->CNT;

if(TimerState > TimerEN +8){
  EncTurnedDown = 1;
  TIM1->CNT = 32768;
}  
if(TimerState < TimerEN -8){
  EncTurnedUp = 1;
  TIM1->CNT = 32768;
} 
//  if(TimerState) 
//    EncButPressUp = 1;
//  else 
 //   EncButPressDown = 1;

  
return;
}
