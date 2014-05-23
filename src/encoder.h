#ifndef _encoder_h
#define _encoder_h


extern  u8 EncButPressDown; 
extern  u8 EncButPressUp; 
extern  u8 EncTurnedDown; 
extern  u8 EncTurnedUp; 

void GenerateEncMesgButton(u8 Event);
void GenerateEncMsgTurn(u8 Event);
void TestEnSwitch(void);
void TestEnTurn(void); 
#endif