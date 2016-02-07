#include "main.h"

void init_tim(){
    // ������������ ������� � ����� PWM
    TIM_TimeBaseInitTypeDef TIM_BaseConfig;
    // ������������ ������ �������
    TIM_OCInitTypeDef TIM_OCConfig;

    //������ ��������
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    TIM_BaseConfig.TIM_Period = 512;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_BaseConfig);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM3->CNT=22; //��������� ��������
    TIM_Cmd(TIM3, ENABLE);


    //��� ��� ����������� ����
    // ������������� ����� �������, ����� - PWM1
    TIM_OCConfig.TIM_OCMode = TIM_OCMode_PWM1;
    // ���������� - ����� �������
    TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
    // ����� ������� 500 ������ => 500/1000 = 50%
    TIM_OCConfig.TIM_Pulse = 500;
    // ���������� => ����� - ��� ������� (+3.3V)
    TIM_OCConfig.TIM_OCPolarity = TIM_OCPolarity_Low;
    // �������������� 3� ����� ������� �3
    TIM_OC3Init(TIM2, &TIM_OCConfig);

    TIM_Cmd(TIM2, DISABLE);


	//������������� ������� ���������� ���������� �� ������� TIM4
	NVIC_SetPriority(TIM4_IRQn, 2);
	//��������� ��������� ���������� �� ������� TIM4
	NVIC_EnableIRQ(TIM4_IRQn);
    TIM_BaseConfig.TIM_Prescaler = 23999;
    // ������ - 1 ��� � ������
    TIM_BaseConfig.TIM_Period = 60000;
    // �������������� ������ �4
    TIM_TimeBaseInit(TIM4, &TIM_BaseConfig);
    //��������� ��������� ����������
    TIM_ITConfig(TIM4, TIM_DIER_UIE, ENABLE);
    TIM_Cmd(TIM4, DISABLE);


	/* �������� ����� ����������� ����������� ���� */
	//������������� ������� ���������� ���������� �� ������� TIM6
	NVIC_SetPriority(TIM6_DAC_IRQn, 3);
	//��������� ��������� ���������� �� ������� TIM6
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
    // ��������� ������ �� �������� ������� � 1 kHz
    TIM_BaseConfig.TIM_Prescaler = 23999;
    // ������ - 10 ���
    TIM_BaseConfig.TIM_Period = delay_airflow_off_ms; //�������� ���������� � ��
    // �������������� ������ �6
    TIM_TimeBaseInit(TIM6, &TIM_BaseConfig);
    //��������� ��������� ���������� ����� ����� ������� �������
	TIM_GenerateEvent(TIM6, TIM_EGR_UG);
	TIM_ClearFlag(TIM6, TIM_SR_UIF);
    //��������� ��������� ����������
    TIM_ITConfig(TIM6, TIM_DIER_UIE, ENABLE);
	// ��������� ���� �������
    TIM_SelectOnePulseMode(TIM6, TIM_CR1_OPM);
    TIM_Cmd(TIM6, DISABLE);
}


//���������� ���������� �� ������� 6
//�������� ����� ����������� ������� ������ ������ - 3���
void TIM6_DAC_IRQHandler(void)
{
	hd44780_backlight_set(0);

	//TIM_ClearFlag(TIM6, TIM_SR_UIF);//���������� ���� ����������
	TIM_Cmd(TIM6, DISABLE);
}

//��������� ������� - ������� ��� ���
u8 get_TIM_state(TIM_TypeDef* TIMx){
	return TIMx->CR1 & TIM_CR1_CEN;
}
