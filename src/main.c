#include "timers.h"
#include "ioconfig.h"
#include "timingconfig.h"
#include "lighting.h"
#include "I2Croutines.h"
#include "events.h"
#include "dac.h"
#include "adc.h"
#include "defines.h"
#include <string.h>
#include <stdio.h>


#include "ff.h"
#include "diskio.h"
#include "MainTimer.h"
#define PB4_IN (*((volatile unsigned long *) 0x42218110))
#define PA12_OUT (*((volatile unsigned long *) 0x422101b0))

DWORD AccSize;				/* Work register for fs command */
char buff[128]={"Hi my darling"};
extern char ready;

FATFS fs;                    //структура, с которой будем связывать диск
FIL   file;
FRESULT res;
UINT nRead, nWritten;
int symbols;
uint8_t startWrite;
uint32_t Delay;

int CheckButton1(){
  
  if(PB4_IN==0 ){
    Delay =300000;
    if(startWrite ==0 ){ 
    startWrite = 1;

     while(PB4_IN==0 ) ;
    while (Delay--);
    PA12_OUT = 1; 
    f_mount(0,&fs);  
    res = f_open(&file, "analog.m", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
      symbols = sprintf(buff,"a1 = [");
      f_write(&file, buff, symbols, &nWritten); 
    }
    else{
  startWrite = 0;  
  while(PB4_IN==0 ) ;
  while (Delay--);
  PA12_OUT = 0;
  symbols = sprintf(buff,"];");
  f_write(&file, buff, symbols, &nWritten); 
  f_close(&file);
  f_mount(0,0);
 
    }
  }
 
return 0;
}



int main(void)
{ 
  uint32_t Counter = 0;
 ClockInit();   
 // InitClk();
 //IWDG_init(); //WATCH DOG
   
 ConfigIO(); // i/о ports configure  
 ConfigureDAC1();
 ADC_config();
 ConfigI2C();
 Timer2_Init(); 
 // configure I2C first
 //LedOK_On(1); 
 //L//EDs_On(4);
// LedErr_On(0);
 TIM1_init(); // encoder support
 //TIM2_init(); //Pulse generation 
 //TIM4_init(); // for IRDA usage
 //SysTick_Config(11999); // Start KeyBoard Scanning
 //IRDA_Int_Conf(); // Configure IRDA Interrupt
 //Touch_Int_Conf();

     while(1){ 
       
CheckButton1();
 // f_write(&file, buff, 128, &nWritten);
       if(startWrite == 1){
  if(ADCdataReady){
    
  symbols = sprintf(buff,"%d; ",ADC_VAL[3]);
  f_write(&file, buff, symbols, &nWritten); 
  if(!(Counter++ % 20)){
  symbols = sprintf(buff,"\n");
  f_write(&file, buff, symbols, &nWritten);
  }
  
  ADCdataReady =0;
  }
       }
 //ReleaseEvents();
//#ifndef _RECEIVER
 //AD_CheckVoltages(); //IF WE dont need to control voltages (in RECEIVER for EXAMPLE) - just comment this string
//#endif 
//GPIOA->BSRR |=  GPIO_BSRR_BS10; //up set to 1 
     }   

}

