#include "keyboard.h"
#include "stm32f10x.h"
#include "queue.h"

union Keys_U readedKeys;
u8 ReadyFlagKb = 0;


void SetLineKbd(u8 Number){
  switch(Number){
    case 0: 
      GPIOB->ODR|=(GPIO_ODR_ODR7|GPIO_ODR_ODR8|GPIO_ODR_ODR9); // set line to Zero for scan
      GPIOB->ODR&=~GPIO_ODR_ODR6;
      break;
    case 1: 
      GPIOB->ODR|=(GPIO_ODR_ODR6|GPIO_ODR_ODR8|GPIO_ODR_ODR9);
      GPIOB->ODR&=~GPIO_ODR_ODR7;
      break;  
    case 2: 
      GPIOB->ODR|=(GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR9);
      GPIOB->ODR&=~GPIO_ODR_ODR8;
      break;
    case 3: 
      GPIOB->ODR|=(GPIO_ODR_ODR6|GPIO_ODR_ODR7|GPIO_ODR_ODR8);
      GPIOB->ODR&=~GPIO_ODR_ODR9;
  }
return;
}

void ReadLineKbd(u8 Number){

  switch(Number){
    case 0: 
      readedKeys.Keys.COL_0 = ((GPIOB->IDR)&0xF000)>>12; // read tetrade (COLUMN CHECK)
      readedKeys.Keys.COL_0 += ((GPIOB->IDR)&0x0006)<<3;
      break;
    case 1: 
      readedKeys.Keys.COL_1 = ((GPIOB->IDR)&0xF000)>>12;
      readedKeys.Keys.COL_1 += ((GPIOB->IDR)&0x0006)<<3;
      break;  
    case 2: 
      readedKeys.Keys.COL_2 = ((GPIOB->IDR)&0xF000)>>12;
      readedKeys.Keys.COL_2 += ((GPIOB->IDR)&0x0006)<<3;
      break;
    case 3: 
      readedKeys.Keys.COL_3 = ((GPIOB->IDR)&0xF000)>>12;
      readedKeys.Keys.COL_3 += ((GPIOB->IDR)&0x0006)<<3;
  }
return;

}

u8 SolvePressedKeys(void){
  static u32 Lines_Kb[2]={0x00FFFFFF,0x00FFFFFF}; // store conditions, expect regulated 
  static u8 Index = 0;  // index for send data to arrays for decrease rattle
  static u32 OldKeysLine = 0x00FFFFFF; // store old key pressed for compare with it
  static u8 Pressed = 0;
  
      
  Lines_Kb[Index] = readedKeys.Bits; 
  if(Lines_Kb[0] == Lines_Kb[1]){
    if(OldKeysLine != readedKeys.Bits){
    GenerateKbdMessages(OldKeysLine ^ readedKeys.Bits);
    OldKeysLine  =  readedKeys.Bits;
    Pressed = 1;
    }
    else{
    Pressed = 0;
    }
  }  
  
   Index++;
   Index%=2;
  return Pressed;
}


#define BUT_POINT 0x2E
#define BUT_F1 0x41
#define BUT_F2 0x42
#define BUT_F3 0x43
#define BUT_UP 0x3A
#define BUT_DOWN 0x3B
#define BUT_LEFT 0x3C
#define BUT_RIGHT 0x3D
#define BUT_EXIT 0x3F
#define BUT_OK 0x3E
#define BUT_DEL 0x40


void GenerateKbdMessages(u32 XORedKeyBits){ // receve XOR of Old and New Keys State for resolve events
 static u8 Event;
 static u8 i;
 static union QueueElement_U QueueElem;
//our KeyMap is
const u8 CharNames[24] = {    BUT_F1, BUT_F2, BUT_F3, BUT_UP, BUT_LEFT, BUT_DOWN,
                              0x31, 0x34, 0x37, BUT_POINT, BUT_OK, 0x00,
                              0x32, 0x35, 0x38, 0x30, BUT_RIGHT, BUT_EXIT,
                              0x33, 0x36, 0x39, BUT_DEL, 0x00, 0x00
  };   


for(i=0;i<24;i++){
    if(XORedKeyBits & (1<<i)){  //bit to bit lookup for events
      if(readedKeys.Bits & (1<<i)) Event = 1;
      else Event = 0;
     // if(Event){   //press down is Zero
      
      QueueElem.QueEl.EventType = Event;              //pressed eq 0 or unpressed (eq1)
      QueueElem.QueEl.Keyb_Key = CharNames[i];        // what key number?
      QueueElem.QueEl.RCU_Key = 0;
      queue_put_elem(&QueueElem.Bits);

    //  }
    
    }
  }
return;
}



