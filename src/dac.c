#include "dac.h"
#include "stm32f10x.h"
#include "stm32f10x_dac.h"

 #define  RCC_APB1ENR_DACEN                  ((uint32_t)0x20000000)        /*!< DAC interface clock enable */

void ConfigureDAC1(void){
RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     // clocking GPIOA
RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // DAC ON
DAC->CR |= DAC_CR_BOFF1;                // buff OFF.
DAC->CR      |= DAC_CR_EN1;             // ON CHANNEL 1
DAC->DHR8R1   = 10;// 185;// 246;                   // set value as 60 mA// 155 max eq 80 mA
}