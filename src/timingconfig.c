#include "stm32F10x.h"
#include "timingconfig.h"

void ClockInit(void){
     RCC->CR|=RCC_CR_HSEON; // Включить генератор HSE.
     while (!(RCC->CR & RCC_CR_HSERDY)) {} // Ожидание готовности HSE.
     RCC->CFGR &=~RCC_CFGR_SW; // Очистить биты SW0, SW1.
     RCC->CFGR |= RCC_CFGR_SW_HSE; // Выбрать HSE для тактирования SW0=1.
  /* Включаем буфер предвыборки FLASH */
      FLASH->ACR |= FLASH_ACR_PRFTBE;
  /* Конфигурируем Flash на 2 цикла ожидания для работы на 24 MHz */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

/* HCLK = SYSCLK */
/* AHB Prescaler = 1 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1; 

/* PCLK2 = HCLK */
/* APB2 Prescaler = 1 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1; 

/* PCLK1 = HCLK */
/* APB1 Prescaler = 2 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2; 

/* Clear old value*/
RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));

/* Set new params */
/* RCC_CFGR_PLLSRC_HSE -- set HSE to input */
/* RCC_CFGR_PLLMULL3 -- multiplier 3 set to 24 MHz*/
RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL3);

/* Turn on PLL */
RCC->CR |= RCC_CR_PLLON;

/* While PLL is not ready */
while((RCC->CR & RCC_CR_PLLRDY) == 0) 
{
// expecting
}

/* Is PLL run? Shoose PLL as system clock */
RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

/* Wait shoose PLL */
while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)  
{
// expecting
}
return;
}

