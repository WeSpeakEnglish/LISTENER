#include "timers.h"
#include "IR_control.h"
#include "lighting.h"
#include "queue.h"

#define minStartSymbolLength 90
#define UnitCode 10922//UL  // Same code for first Word of this IR
#define RepetativeTime 600 // 600*1/2000 around 0.3 s

//debugging
//u16 periods_received_IR[128];
//
u32 Words_IR[2];
u8 index_periods_receiver_IR = 0;
u8 IR_Received_Flag = 0;

u8 IR_KeyCode = 0x00; //code of button
u8 IR_KeyPress = 0x00; // 0x01 - press, 0x02 - release, 0x03 - both

u16 IR_repeat_is_stop = 0; // if IRC send repeat we hold it


#define IR_Compare_W_length 17
const u32 IR_Compare_W[IR_Compare_W_length] = {
  2183276682,   //0
  2718042282,   //1
  167813290,    //2
  671122090,    //3
  671253162,    //4
  167944362,    //5
  2854355082,   //6
  134390442,    //7
  704807082,    //8
  2317492362,   //9
  2149722762,   //*
  2283938442,   //#
  2271882,      //ok
  539134602,    // <<
  2149591690,   // >>
  2686585482,   // ^^
  570460330     //vv
}; 

const u8 IR_CharNames[IR_Compare_W_length] = {
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3E, 0x3F, 0x3E, 0x3C, 0x3D, 0x3A, 0x3B  
};



// interrupt from IRDA
void EXTI3_IRQHandler(void)
{
 static u16 ReceivedPeriod = 0;

EXTI->PR |= EXTI_PR_PR3; // clear interrupt
//TIM4->CR1 &= ~TIM_CR1_CEN;

  ReceivedPeriod = TIM4->CNT;
 
 // periods_received_IR[index_periods_receiver_IR] = ReceivedPeriod;
 
  if(IR_Received_Flag){
  if (ReceivedPeriod < minStartSymbolLength){
    if(index_periods_receiver_IR > 31){ 
     if(ReceivedPeriod > 30) 
       Words_IR[1] = (Words_IR[1]<<1)  + 1;// | (1 << (index_periods_receiver_IR%32)); 
     else Words_IR[1] = (Words_IR[1]<<1);
    }
    else{
     if(ReceivedPeriod > 30) 
       Words_IR[0] = (Words_IR[0]<<1) + 1;// | (1 << (index_periods_receiver_IR%32)); 
     else Words_IR[0] = (Words_IR[0]<<1);   
    }
    index_periods_receiver_IR++;
  }
  }
  
TIM4->CNT = 0x0000; 

  if ((!IR_Received_Flag) && ReceivedPeriod > minStartSymbolLength ){ // start symbol received
     IR_Received_Flag = 1;
  }
  TIM4->CR1 |= TIM_CR1_CEN;

}

void Update_IR_Repetative(void){
if(IR_repeat_is_stop)IR_repeat_is_stop--;
if(IR_repeat_is_stop == 0x01)IR_KeyPress |= 0x02; // release event
}

void IR_Solve(void){ //set release or not
  static u8 index = 0;
  static u32 CompareValue;
  u32 const* p_IR_Compare_W = IR_Compare_W ;
  
  if (Words_IR[0] == 2){
   if (Words_IR[1] == 0)
     IR_repeat_is_stop = RepetativeTime;  
  }
  else{
  // if(Words_IR[0] == UnitCode){
     for(index = 0; index < IR_Compare_W_length; index++){
       CompareValue = *(p_IR_Compare_W++);
       if ((Words_IR[1] == CompareValue)){
        IR_KeyCode = IR_CharNames[index]; // select Code
        IR_repeat_is_stop = RepetativeTime; // dead line timer
        IR_KeyPress |= 0x01; // press event 
 //      }
     }  
   }
  }
 Words_IR[0] = 0;
 Words_IR[1] = 0;
 index_periods_receiver_IR = 0;
 IR_Received_Flag = 0;

}

u8 IR_Generate_Events(void){
 u8 EventGenerated = 0;
 static union QueueElement_U QueueElem;

  if(IR_KeyPress & 0x01){ 
       QueueElem.QueEl.EventType = 0;              //pressed eq 0 or unpressed (eq1)
       QueueElem.QueEl.Keyb_Key = IR_KeyCode; 
       QueueElem.QueEl.RCU_Key = 0;
      queue_put_elem(&QueueElem.Bits);
      IR_KeyPress &= ~0x01;
      EventGenerated = 1;
  }
  if(IR_KeyPress & 0x02){
       QueueElem.QueEl.EventType = 1;              //pressed eq 0 or unpressed (eq1)
       QueueElem.QueEl.Keyb_Key = IR_KeyCode; 
       QueueElem.QueEl.RCU_Key = 0;
      queue_put_elem(&QueueElem.Bits);
      IR_KeyPress &= ~0x02;
      IR_KeyCode = 0x00; //Reset key Code
      EventGenerated = 1;
  }
  return EventGenerated;
  
}


void TIM4_IRQHandler(void)
{

TIM4->SR &= ~TIM_SR_UIF;        // clear flag
TIM4->CR1 &=~ TIM_CR1_CEN;        // disable interrupt 
//LedErr_On(InsideState%2);

IR_Solve();
return;
}

