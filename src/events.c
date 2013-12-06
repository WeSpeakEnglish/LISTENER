#include "events.h"
#include "touchscreen.h"
#include "keyboard.h" 


void ReleaseEvents(void){
  if(ReadyFlagKb){    
    ReadyFlagKb = 0;
    SolvePressedKeys();
  }
  if(PressDownTS){ 
  PressDownTS = 0;
  GenerateTSMessages(0); 

  }
  if(PressUpTS){  
  PressUpTS = 0;
  GenerateTSMessages(1);
   }
}



