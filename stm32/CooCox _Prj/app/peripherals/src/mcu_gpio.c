/*
 *	File:	mcu_gpio.c
 *	Date:	07.01.2011
 */

#include "main.h"

//-----------------------------------------------------------------------------
void mcu_gpio_deinit()
{
	//��������� ������������ ��������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��������� ��������
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //��� ��� ����
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //�������� ����� ����������� ����������� ����

	//��������� ������������ ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//�������� ������� ��
	PIN_CONFIGURATION(SOLDER_TEMP_ADC); //���. ����������� ���������
	PIN_CONFIGURATION(AIRFEN_TEMP_ADC); //���. ����������� ����

	PIN_CONFIGURATION(SOLDER_HEATER);	//��������/���������� ���������
	PIN_CONFIGURATION(AIR_HEATER);		//��������/���������� ����
	PIN_CONFIGURATION(SOLDER_GREEN_LED);//������� ��������� ���������
	PIN_CONFIGURATION(AIR_GREEN_LED);	//������� ��������� ����

	PIN_CONFIGURATION(AIR_FLOW_PWM);	//����� TIM2 CH1 - ��� ��� ����
	PIN_CONFIGURATION(AIR_HEATER);		//���./����. ����������� ����
	PIN_CONFIGURATION(SOLDER_HEATER);	//���./����. ����������� ���������

	PIN_CONFIGURATION(SELECT_BTN);
	PIN_CONFIGURATION(GERKON_AIR);
	PIN_CONFIGURATION(GERKON_SOLDER);

	PIN_CONFIGURATION(ENCODER_A);
	PIN_CONFIGURATION(ENCODER_B);

	PIN_CONFIGURATION(USER_LED_green);
#ifdef USER_LED_blue
	PIN_CONFIGURATION(USER_LED_blue);
#endif
#ifdef STM32F100_DISCOVERY_BOARD
	PIN_CONFIGURATION(USER_BTN);
#endif
}
