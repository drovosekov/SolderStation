/*
 *	File:	mcu_gpio.c
 *	Date:	07.01.2011
 */

#include "main.h"

//-----------------------------------------------------------------------------
void mcu_gpio_deinit()
{
	//разрешаем тактирование таймеров
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ШИМ для фена

	//разрешаем тактирование портов
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	//настрока выводов МК
	PIN_CONFIGURATION(SOLDER_TEMP_ADC); //уст. температуры паяльника
	PIN_CONFIGURATION(AIRFEN_TEMP_ADC); //уст. температуры фена
	PIN_CONFIGURATION(AIR_FLOW_ADC);	//уст. мощность воздушного потока

	PIN_CONFIGURATION(SOLDER_ONOFF_BUT);//кнопка влючения/отключения паяльника
	PIN_CONFIGURATION(AIRFEN_ONOFF_BUT);//кнопка влючения/отключения фена
	PIN_CONFIGURATION(AIR_FLOW_PWM);	//выход TIM3 CH4 - ШИМ для фена

	PIN_CONFIGURATION(USER_LED);
}
