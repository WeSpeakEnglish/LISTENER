#include "events.h"
#include "touchscreen.h"
#include "keyboard.h" 
#include "stm32f10x_iwdg.h"
#include "encoder.h"
#include "lighting.h"
#include "IR_control.h"
#include "adc.h"

void ReleaseEvents(void){
  u8 shot =0;
  if(ReadyFlagKb){    
    ReadyFlagKb = 0;
    if(SolvePressedKeys())
      shot =1;
  }
  if(PressDownTS){ 
  PressDownTS = 0;
  GenerateTSMessages(0); 
  shot =1;
  }
  if(PressUpTS){  
  PressUpTS = 0;
  GenerateTSMessages(1);
  shot =1;
   }
  if(EncButPressDown){ 
  EncButPressDown = 0;
  GenerateEncMesgButton(0); 
  shot =1;
  }
  if(EncButPressUp){  
  EncButPressUp = 0;
  GenerateEncMesgButton(1);
  shot =1;
   } 
  
   if(EncTurnedDown){ 
  EncTurnedDown = 0;
  GenerateEncMsgTurn(1);
  shot =1;
  }
  if(EncTurnedUp){  
  EncTurnedUp = 0;
  GenerateEncMsgTurn(0);
  shot =1;
   } 
  if(IR_Generate_Events()) shot =1; 
  
  if(CheckVoltagesError){
  shot = 1;
  CheckVoltagesError = 0;
  GenerateCheck_V_Msg();
  }
 
 if (shot)
   StartShot(); 
   IWDG_ReloadCounter(); 
   

}

void StartShot(void){
   TIM2->CR1 |= TIM_CR1_CEN;  //Pulse will go
}


