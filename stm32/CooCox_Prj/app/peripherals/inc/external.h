//===========source description =================================
//http://we.easyelectronics.ru/STM32/stm32-udobnye-vneshnie-preryvaniya.html
//===============================================================
#ifndef EXTERNAL_H
#define EXTERNAL_H
#include "main.h"


#define GPIO_TO_INT(GPIO)                               (((uint32_t)(&(GPIO->CRL)) - GPIOA_BASE)>>10)

#define __PIN_TO_EXTI_IRQn(PIN)                         (PIN == 0)? EXTI0_IRQn :\
                                                        (PIN == 1)? EXTI1_IRQn :\
                                                        (PIN == 2)? EXTI2_IRQn :\
                                                        (PIN == 3)? EXTI3_IRQn :\
                                                        (PIN == 4)? EXTI4_IRQn :\
                                                        (PIN <= 9)? EXTI9_5_IRQn : EXTI15_10_IRQn

#define _PIN_TO_EXTI_IRQn(PIN)                          __PIN_TO_EXTI_IRQn(PIN)
#define PIN_TO_EXTI_IRQn(PIN)                           _PIN_TO_EXTI_IRQn(PIN)

#define __PIN_TO_EXTI_HANDLER(PIN)                      EXTI##PIN##_IRQHandler
#define _PIN_TO_EXTI_HANDLER(PIN)                       __PIN_TO_EXTI_HANDLER(PIN)
#define PIN_TO_EXTI_HANDLER(PIN)                        _PIN_TO_EXTI_HANDLER(PIN)

#define EXTI_MODE_DISABLE                               0x00 //обработка события выключена
#define EXTI_MODE_RISE                                  0x01 //событие настроено на подъем фронта
#define EXTI_MODE_FALL                                  0x02 //по спаду
#define EXTI_MODE_BOTH                                  0x03 //по подъему и спаду

#define _EXTI_INIT(GPIO, PIN, EXTI_MODE, NVIC_PRIORITY) do{\
                        AFIO->EXTICR[PIN/4] = (AFIO->EXTICR[PIN/4] & ~((uint16_t)0x0F<<((PIN % 4)<<2)))|(GPIO_TO_INT(GPIO)<<((PIN % 4)<<2));\
                        BIT_BAND_PER(EXTI->FTSR,1UL<<PIN)=!!(EXTI_MODE & 0x02);\
                        BIT_BAND_PER(EXTI->RTSR,1UL<<PIN)=!!(EXTI_MODE & 0x01);\
                        NVIC_SetPriority(PIN_TO_EXTI_IRQn(PIN),NVIC_PRIORITY);\
                        (EXTI_MODE>0)? NVIC_EnableIRQ(PIN_TO_EXTI_IRQn(PIN)): NVIC_DisableIRQ(PIN_TO_EXTI_IRQn(PIN));\
                        EXTI->PR = 1UL<<PIN;\
                        BIT_BAND_PER(EXTI->IMR,1UL<<PIN)=!!(EXTI_MODE);\
                        }while(0)
        
#define EXTI_INIT(GPIO, PIN, EXTI_MODE, NVIC_PRIORITY)  _EXTI_INIT(GPIO, PIN, EXTI_MODE, NVIC_PRIORITY)

//example: EXTI_INIT(A, 9, EXTI_MODE_BOTH, 15);

void external_init(void);

#endif
