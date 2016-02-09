//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef MAIN_H_
#define MAIN_H_


//#define STM32F100_DISCOVERY_BOARD

#define delay_airflow_off_ms 5000

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
#include "types.h"
#include "delay.h"

void init_All();
void turnon_backlight();


#endif
