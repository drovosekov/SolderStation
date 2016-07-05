/*
 *	File:	mcu_gpio.c
 *	Date:	07.01.2011
 */

#include "main.h"

//-----------------------------------------------------------------------------
void mcu_gpio_init()
{
	//��������� ������������ ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //��� ��� ����������� ����
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);	//���. �������������� ������ ������� 1

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��������� ������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��������� ��������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //��������� ��� �������

	//��������� ������������ ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//�������� ������� ��
	PIN_CONFIGURATION(SOLDER_TEMP_ADC); //���. ����������� ���������
	PIN_CONFIGURATION(AIRFEN_TEMP_ADC); //���. ����������� ����
	PIN_CONFIGURATION(BUTTONS_ADC); 	//������

	PIN_CONFIGURATION(SOLDER_HEATER);	//��������/���������� ���������
	PIN_CONFIGURATION(AIR_HEATER);		//��������/���������� ����

	PIN_CONFIGURATION(SOLDER_LEDBTN);	//��������� ��������� ���������
	PIN_CONFIGURATION(AIRFEN_LEDBTN);	//��������� ��������� ����

	PIN_CONFIGURATION(AIR_FLOW_PWM);	//����� TIM1 CH2N - ��� ��� ����
	PIN_CONFIGURATION(AIR_HEATER);		//���./����. ����������� ����
	PIN_CONFIGURATION(SOLDER_HEATER);	//���./����. ����������� ���������

	PIN_CONFIGURATION(RELAY_POWER);		//���� ������� �������
	PIN_CONFIGURATION(RELAY_FEN);		//���� ������ 220� �� ���

	PIN_CONFIGURATION(GERKON_AIR);
	PIN_CONFIGURATION(GERKON_SOLDER);

	PIN_CONFIGURATION(ENCODER_A);
	PIN_CONFIGURATION(ENCODER_B);

	PIN_CONFIGURATION(BUZZER);

	PIN_CONFIGURATION(USER_LED_green);
#ifdef USER_LED_blue
	PIN_CONFIGURATION(USER_LED_blue);
#endif
#ifdef STM32F100_DISCOVERY_BOARD
	PIN_CONFIGURATION(USER_BTN);
#endif
}
