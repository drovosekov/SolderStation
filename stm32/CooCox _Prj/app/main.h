//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef MAIN
#define MAIN

#define STM32F100_DISCOVERY_BOARD

#define delay_airflow_off_ms 10000

#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "delay\delay.h"
#include "hd44780\hd44780.h"
#include "peripherals/inc/mcu_adc.h"
#include "peripherals/inc/mcu_gpio.h"
#include "peripherals/inc/mcu_tim.h"

void init_All();
void blink_led();
void turnon_backlight();
u8 get_TIM_state(TIM_TypeDef* TIMx);

#endif
