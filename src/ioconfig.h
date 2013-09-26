#ifndef _ioconfig_h
#define _ioconfig_h


void GPIO_Int_Conf(void);
void IRDA_Int_Conf(void);
void Touch_Int_Conf(void);
void EXTI0_IRQHandler(void); //interrupt handler
void EXTI1_IRQHandler(void); //interrupt handler
void EXTI2_IRQHandler(void); //interrupt handler
void ConfigIO(void);
void ConfigI2C(void);
//void InterruptCS_On(void);
#endif