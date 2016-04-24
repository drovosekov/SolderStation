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

#define AIRFLOW_DELAY_OFF_ms	5000	//���������� �� ���������� ������ ����
#define BUZZER_FREQ			 	975		//������� ����� ����� � ��
#define BEEP_DELAY_ms		 	200		//�������� � �� ����� ��������� ����
#define AFTER_LOGO_DELAY_ms		2000	//����� ����������� �������� ��
#define SYMB_DEGREE				8		//����� ����������������� ������� �������
#define SYMB_UP_ARROW			1		//����� ������� ������� ����� ��� ����� ����������� ���� ��������
#define SYMB_DN_ARROW			20		//����� ������� ������� ���� ��� ������� ����������� ���� �������� (�� ��������� �� ������������ - ������)
#define AUTO_POWER_OFF			600		//������ �� ��������������� ���������� ������� ��� �����������
#define AUTO_SLD_OFF			300		//������ �� �������� ��������� �� ������ ��������� ������ � ����������� ���������
#define AUTO_FEN_OFF			300		//������ �� �������� ���� �� ������ �������� � ����������� ���������
#define CURSOR_OFF_TIMEOUT		10		//������ �� ���������� ������� �������

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
