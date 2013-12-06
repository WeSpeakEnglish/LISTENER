#include "keyboard.h"
#include "stm32f10x.h"
#include "queue.h"

union Keys_U readedKeys;
u8 ReadyFlagKb = 0;


void SetLineKbd(u8 Number){
  switch(Number){
    case 0: 
      GPIOB->ODR|=(GPIO_ODR_ODR1|GPIO_ODR_ODR2|GPIO_ODR_ODR3); // set line to Zero for scan
      GPIOB->ODR&=~GPIO_ODR_ODR0;
      break;
    case 1: 
      GPIOB->ODR|=(GPIO_ODR_ODR0|GPIO_ODR_ODR2|GPIO_ODR_ODR3);
      GPIOB->ODR&=~GPIO_ODR_ODR1;
      break;  
    case 2: 
      GPIOB->ODR|=(GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR3);
      GPIOB->ODR&=~GPIO_ODR_ODR2;
      break;
    case 3: 
      GPIOB->ODR|=(GPIO_ODR_ODR0|GPIO_ODR_ODR1|GPIO_ODR_ODR2);
      GPIOB->ODR&=~GPIO_ODR_ODR3;
  }
return;
}

void ReadLineKbd(u8 Number){

  switch(Number){
    case 0: 
      readedKeys.COL_0 = ((GPIOB->IDR)&0x00F0)>>4; // read tetrade (COLUMN CHECK)
      break;
    case 1: 
      readedKeys.COL_1 = ((GPIOB->IDR)&0x00F0)>>4;
      break;  
    case 2: 
      readedKeys.COL_2 = ((GPIOB->IDR)&0x00F0)>>4;
      break;
    case 3: 
      readedKeys.COL_3 = ((GPIOB->IDR)&0x00F0)>>4;
  }
return;

}

void SolvePressedKeys(void){
  static u16 Lines_Kb[2]={0xFFFF,0xFFFF}; // store conditions, expect regulated 
  static u8 Index = 0;  // index for send data to arrays for decrease rattle
  static u16 OldKeysLine = 0xFFFF; // store old key pressed for compare with it
  
      
  Lines_Kb[Index] = readedKeys.Bits; 
  if(Lines_Kb[0] == Lines_Kb[1]){
    if(OldKeysLine != readedKeys.Bits){
    GenerateKbdMessages(OldKeysLine ^ readedKeys.Bits);
    OldKeysLine  =  readedKeys.Bits;
    }
  }  
  
  Index++;
  Index%=2;
  return;
}

void GenerateKbdMessages(u16 XORedKeyBits){ // receve XOR of Old and New Keys State for resolve events
 u8 Event;
 u8 i;
 static union QueueElement_U QueueElem;
//our KeyMap is
const u8 CharNames[16] = {    0x31,0x34,0x37,0x3E,
                                0x32,0x35,0x38,0x30,
                                0x33,0x36,0x39,0x3F,
                                0x3A,0x3B,0x3C,0x3D
  };   


for(i=0;i<16;i++){
    if(XORedKeyBits & (1<<i)){  //bit to bit lookup for events
      if(readedKeys.Bits & (1<<i)) Event = 1;
      else Event = 0;
     // if(Event){   //press down is Zero
      
      QueueElem.EventType = Event;              //pressed eq 0 or unpressed (eq1)
      QueueElem.Keyb_Key = CharNames[i];        // what key number?
      QueueElem.RCU_Key = 0;
      
      queue_put_elem(&QueueElem.Bits);

    //  }
    
    }
  }
return;
}



