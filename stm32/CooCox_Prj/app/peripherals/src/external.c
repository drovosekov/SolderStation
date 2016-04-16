//===========source description =================================
//http://we.easyelectronics.ru/STM32/stm32-udobnye-vneshnie-preryvaniya.html
//===============================================================

#include "main.h"

void EXTI5_IRQHandler(void) __attribute__((weak));
void EXTI6_IRQHandler(void) __attribute__((weak));
void EXTI7_IRQHandler(void) __attribute__((weak));
void EXTI8_IRQHandler(void) __attribute__((weak));
void EXTI9_IRQHandler(void) __attribute__((weak));
void EXTI10_IRQHandler(void) __attribute__((weak));
void EXTI11_IRQHandler(void) __attribute__((weak));
void EXTI12_IRQHandler(void) __attribute__((weak));
void EXTI13_IRQHandler(void) __attribute__((weak));
void EXTI14_IRQHandler(void) __attribute__((weak));
void EXTI15_IRQHandler(void) __attribute__((weak));

void EXTI9_5_IRQHandler(void)
{
  if (EXTI->PR & (1<<5))
    EXTI5_IRQHandler();
  if (EXTI->PR & (1<<6))
    EXTI6_IRQHandler();
  if (EXTI->PR & (1<<7))
    EXTI7_IRQHandler();
  if (EXTI->PR & (1<<8))
    EXTI8_IRQHandler();
  if (EXTI->PR & (1<<9))
    EXTI9_IRQHandler();
}

void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & (1<<10))
    EXTI10_IRQHandler();
  if (EXTI->PR & (1<<11))
    EXTI11_IRQHandler();
  if (EXTI->PR & (1<<12))
    EXTI12_IRQHandler();
  if (EXTI->PR & (1<<13))
    EXTI13_IRQHandler();
  if (EXTI->PR & (1<<14))
    EXTI14_IRQHandler();
  if (EXTI->PR & (1<<15))
    EXTI15_IRQHandler();
}

void external_init(void){
	EXTI_INIT(FEN_GERKONS_EXT_PORT, FEN_GERKON_PIN, EXTI_MODE_BOTH, 15);	//геркон установки фена на подставку
	EXTI_INIT(SLD_GERKONS_EXT_PORT, SLD_GERKON_PIN, EXTI_MODE_BOTH, 15);	//геркон установки паяльника на подставку
}
