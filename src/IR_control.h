#ifndef _IR_control_h
#define _IR_control_h

// interrupt from IRDA
void EXTI3_IRQHandler(void);
void TIM4_IRQHandler(void);
extern u8 periods_received[128]; 
void Update_IR_Repetative(void);
void IR_Solve(void); // test command
u8 IR_Generate_Events(void);

#endif