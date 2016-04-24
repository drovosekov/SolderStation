//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef MAIN_H_
#define MAIN_H_


//#define STM32F100_DISCOVERY_BOARD

#define AIRFLOW_DELAY_OFF_ms	5000	//милисекунд до отключения мотора фена
#define BUZZER_FREQ			 	975		//частота звука биппа в Гц
#define BEEP_DELAY_ms		 	200		//интервал в мс между сигналами бипп
#define AFTER_LOGO_DELAY_ms		2000	//время отображения заставки мс
#define SYMB_DEGREE				8		//номер пользовательского символа градуса
#define SYMB_UP_ARROW			1		//номер символа стрелки вверх при росте температуры выше заданной
#define SYMB_DN_ARROW			20		//номер символа стрелки вниз при падении температуры ниже заданной (по умолчанию не отображается - пробел)
#define AUTO_POWER_OFF			600		//секунд до автоматического отключения станции при бездействии
#define AUTO_SLD_OFF			300		//секнуд до перевода паяльника из режима экономной работы в выключенное состояние
#define AUTO_FEN_OFF			300		//секнуд до перевода фена из режима ожидания в выключенное состояние
#define CURSOR_OFF_TIMEOUT		10		//секунд до отключения мигания курсора

#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>

#include "misc/inc/types.h"
#include "misc/inc/delay.h"
#include "misc/inc/display.h"
#include "hd44780/hd44780.h"
#include "hd44780/pcf8574.h"
#include "misc/inc/bitbanding.h"
#include "peripherals/inc/mcu_adc.h"
#include "peripherals/inc/mcu_gpio.h"
#include "peripherals/inc/mcu_tim.h"
#include "peripherals/inc/buttons.h"
#include "peripherals/inc/external.h"

void init_All(void);
void buzzer(u8 state);
void beep(u8 count);

#endif
