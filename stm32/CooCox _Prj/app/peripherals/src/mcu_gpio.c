/*
 *	File:	mcu_gpio.c
 *	Date:	07.01.2011
 */

#include "main.h"

//-----------------------------------------------------------------------------
void mcu_gpio_deinit()
{
	//��������� ������������ ��������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��� ��� ����

	//��������� ������������ ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	//�������� ������� ��
	PIN_CONFIGURATION(SOLDER_TEMP_ADC); //���. ����������� ���������
	PIN_CONFIGURATION(AIRFEN_TEMP_ADC); //���. ����������� ����
	PIN_CONFIGURATION(AIR_FLOW_ADC);	//���. �������� ���������� ������

	PIN_CONFIGURATION(SOLDER_ONOFF_BUT);//������ ��������/���������� ���������
	PIN_CONFIGURATION(AIRFEN_ONOFF_BUT);//������ ��������/���������� ����
	PIN_CONFIGURATION(AIR_FLOW_PWM);	//����� TIM3 CH4 - ��� ��� ����

	PIN_CONFIGURATION(USER_LED);
}
