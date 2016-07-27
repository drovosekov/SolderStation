#include "main.h"

u16 power_off_count;	//������� �������������� �������
u8 cursor_cnt_state;	//������� �������������� ������� �������
EncBtnStates encBtn;

SLD_INFO sld;
SLD_INFO fen;

u16 encoder_value=20;

void init_tim(){
    // ������������ ������� � ����� PWM
    TIM_TimeBaseInitTypeDef TIM_BaseConfig;
    // ������������ ������ �������
    TIM_OCInitTypeDef TIM_OCConfig;

    //�������
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    TIM_BaseConfig.TIM_Period = 5;
    TIM_BaseConfig.TIM_Prescaler = 0;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_BaseConfig);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM3->CNT=20; //��������� ��������
    //��������� ��������� ����������
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    //��������� ������
    TIM_Cmd(TIM3, ENABLE);
	//������������� ������� ���������� ���������� �� ������� TIM3
	//NVIC_SetPriority(TIM3_IRQn, 5);
	//��������� ��������� ���������� �� ������� TIM3
	NVIC_EnableIRQ(TIM3_IRQn);


    //��� ��� ����������� ����

    // ��������� ������ �� �������� ������� � 4800 kHz
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    // ������ - 100 ������ => 4800/100 = 48 kHz
    TIM_BaseConfig.TIM_Period = 99;
    TIM_BaseConfig.TIM_Prescaler = (SystemCoreClock / 2400) - 1;
    TIM_BaseConfig.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_BaseConfig);

    // ������������� ����� �������, ����� - PWM1
    TIM_OCConfig.TIM_OCMode = TIM_OCMode_PWM1;
    // ���������� - ����� �������
    TIM_OCConfig.TIM_OutputNState  = TIM_OutputNState_Enable;
    // ����� ������� 50 ������ => 50/100 = 50%
    TIM_OCConfig.TIM_Pulse = 0;
    // ���������� => ����� - ��� ������� (+3.3V)
    TIM_OCConfig.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCConfig.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    // �������������� 1� ����� ������� �1
    TIM_OC1Init(TIM1, &TIM_OCConfig);
    /* enable main output */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, DISABLE);



    //��������� ��� �������
    // ������������� ����� �������, ����� - PWM1
    TIM_OCConfig.TIM_OCMode = TIM_OCMode_Toggle;
    // ���������� - ����� �������
    TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
    // ���������� => ����� - ��� ������� (+3.3V)
    TIM_OCConfig.TIM_OCPolarity = TIM_OCPolarity_Low;
    // �������������� 2� ����� ������� �1
    TIM_OC2Init(TIM4, &TIM_OCConfig);

	TIM_TimeBaseInitTypeDef snd_timebase;
	snd_timebase.TIM_CounterMode = TIM_CounterMode_Up;
	snd_timebase.TIM_Prescaler = 0;
	snd_timebase.TIM_ClockDivision = TIM_CKD_DIV1;
	snd_timebase.TIM_RepetitionCounter = 0;
	snd_timebase.TIM_Period = (SystemCoreClock / (BUZZER_FREQ << 1)) - 1;
	TIM_TimeBaseInit(TIM4, &snd_timebase);
    TIM_Cmd(TIM4, DISABLE);


	//������������� ������� ���������� ���������� �� ������� TIM2
	NVIC_SetPriority(TIM2_IRQn, 3);
	//��������� ��������� ���������� �� ������� TIM2
	NVIC_EnableIRQ(TIM2_IRQn);
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    TIM_BaseConfig.TIM_Prescaler = 23999;
    // ������ - 1 ��� � ������
    TIM_BaseConfig.TIM_Period = 1000;
    // �������������� ������ �2
    TIM_TimeBaseInit(TIM2, &TIM_BaseConfig);
    //��������� ��������� ����������
    TIM_ITConfig(TIM2, TIM_DIER_UIE, ENABLE);
	// ��������� ���� �������
    TIM_Cmd(TIM2, ENABLE);
}


//���������� ���������� �� ������� 2 - ����������� 1 ��� � �������
void TIM2_IRQHandler(void)
{
	TIM_ClearFlag(TIM2, TIM_SR_UIF);//���������� ���� ����������

	PIN_REVERSE(USER_LED_green);	//������ ������ ��� � ������� ����������� �� ����� �����������

	//==���������� �������������� �������==
	if((fen.state == isOff || fen.state == notReady) &&
	   (sld.state == isOff || sld.state == notReady)){
		power_off_count++;
		if(power_off_count > AUTO_POWER_OFF){
			PIN_OFF(RELAY_POWER);
		}
	}else{
		power_off_count = 0;
	}
	//=====================================

	//====������ ���������� �� ������ ������������� ��� ������� ������===
	if(fen.state == isPreOn || fen.state == isSleepMode){
		fen.auto_off++;
		if(fen.auto_off > AUTO_FEN_OFF){
			fen.state = isOff;
		}
	}else{
		fen.auto_off = 0;
	}

	if(sld.state == isPreOn || sld.state == isSleepMode){
		sld.auto_off++;
		if(sld.auto_off > AUTO_SLD_OFF){
			sld.state = isOff;
		}
	}else{
		sld.auto_off = 0;
	}
	//===================================================================

	//=====���������� ������ ������� ������� ��� ��������� ���������=====
	static u16 enc=0;

	if(enc != encoder_value){
		cursor_cnt_state = CURSOR_OFF_TIMEOUT;
		enc = encoder_value;
	}else if(cursor_cnt_state > 1){
		cursor_cnt_state--;
	}else if(cursor_cnt_state == 1){
		cursor_cnt_state = 0;
		encBtn = SEL_OFF;
	}
	//====================================================================
}

//���������� ���������� �� ������� 3 - ������ � ���������
void TIM3_IRQHandler(void)
{
	  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	  {
	    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//���������� ���� ����������

	    if(encoder_value == 0) {encoder_value = 20;}
	    if(TIM3->CR1 & TIM_CR1_DIR){
	    	encoder_value=encoder_value+2;
	    }else if(encoder_value > 0){
	    	encoder_value=encoder_value-2;
	    }


		switch(encBtn){ //������� ���.�������� ������� ����������� ��������
		case SLD_TEMP:  //��������� ����������� ���������
			if(encoder_value > 400) {encoder_value = 400;}
			if(encoder_value < 20)  {encoder_value = 20;}

			if(cursor_cnt_state){sld.temp = encoder_value;}
			break;

		case FEN_TEMP: //��������� ����������� ���������� ������ ���

			if(encoder_value > 400) {encoder_value = 400;}
			if(encoder_value < 20)  {encoder_value = 20;}

			if(cursor_cnt_state){fen.temp = encoder_value;}
			break;

		case FEN_AIRFLOW://��������� ���� ���������� ������ ����
			if(encoder_value > 100) {encoder_value = 100;}
			if(encoder_value < 30)  {encoder_value = 30;}

			if(cursor_cnt_state){
				fen.air_flow = encoder_value;
				TIM_SetCompare1(TIM1, encoder_value);
			}
			break;

		case SEL_OFF:
			break;
		}
	  }
}

//��������� ������� - ������� ��� ���
u8 get_TIM_state(TIM_TypeDef* TIMx){
	return TIMx->CR1 & TIM_CR1_CEN;
}


