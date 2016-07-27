#include "main.h"

u16 power_off_count;	//счетчик автоотключения станции
u8 cursor_cnt_state;	//счетчик автоотключения мигания курсора
EncBtnStates encBtn;

SLD_INFO sld;
SLD_INFO fen;

u16 encoder_value=20;

void init_tim(){
    // Конфигурация таймера в режим PWM
    TIM_TimeBaseInitTypeDef TIM_BaseConfig;
    // Конфигурация выхода таймера
    TIM_OCInitTypeDef TIM_OCConfig;

    //энкодер
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    TIM_BaseConfig.TIM_Period = 5;
    TIM_BaseConfig.TIM_Prescaler = 0;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_BaseConfig);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM3->CNT=20; //начальное значение
    //разрешаем генерацию прерывания
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    //запускаем таймер
    TIM_Cmd(TIM3, ENABLE);
	//устанавливаем уровень приоритета прерывания от таймера TIM3
	//NVIC_SetPriority(TIM3_IRQn, 5);
	//разрешаем обработку прерывания от таймера TIM3
	NVIC_EnableIRQ(TIM3_IRQn);


    //ШИМ для вентилятора фена

    // Запускаем таймер на тактовой частоте в 4800 kHz
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    // Период - 100 тактов => 4800/100 = 48 kHz
    TIM_BaseConfig.TIM_Period = 99;
    TIM_BaseConfig.TIM_Prescaler = (SystemCoreClock / 2400) - 1;
    TIM_BaseConfig.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_BaseConfig);

    // Конфигурируем выход таймера, режим - PWM1
    TIM_OCConfig.TIM_OCMode = TIM_OCMode_PWM1;
    // Собственно - выход включен
    TIM_OCConfig.TIM_OutputNState  = TIM_OutputNState_Enable;
    // Пульс длинной 50 тактов => 50/100 = 50%
    TIM_OCConfig.TIM_Pulse = 0;
    // Полярность => пульс - это единица (+3.3V)
    TIM_OCConfig.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCConfig.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    // Инициализируем 1й выход таймера №1
    TIM_OC1Init(TIM1, &TIM_OCConfig);
    /* enable main output */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, DISABLE);



    //генератор для зуммера
    // Конфигурируем выход таймера, режим - PWM1
    TIM_OCConfig.TIM_OCMode = TIM_OCMode_Toggle;
    // Собственно - выход включен
    TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
    // Полярность => пульс - это единица (+3.3V)
    TIM_OCConfig.TIM_OCPolarity = TIM_OCPolarity_Low;
    // Инициализируем 2й выход таймера №1
    TIM_OC2Init(TIM4, &TIM_OCConfig);

	TIM_TimeBaseInitTypeDef snd_timebase;
	snd_timebase.TIM_CounterMode = TIM_CounterMode_Up;
	snd_timebase.TIM_Prescaler = 0;
	snd_timebase.TIM_ClockDivision = TIM_CKD_DIV1;
	snd_timebase.TIM_RepetitionCounter = 0;
	snd_timebase.TIM_Period = (SystemCoreClock / (BUZZER_FREQ << 1)) - 1;
	TIM_TimeBaseInit(TIM4, &snd_timebase);
    TIM_Cmd(TIM4, DISABLE);


	//устанавливаем уровень приоритета прерывания от таймера TIM2
	NVIC_SetPriority(TIM2_IRQn, 3);
	//разрешаем обработку прерывания от таймера TIM2
	NVIC_EnableIRQ(TIM2_IRQn);
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    TIM_BaseConfig.TIM_Prescaler = 23999;
    // Период - 1 раз в минуту
    TIM_BaseConfig.TIM_Period = 1000;
    // Инициализируем таймер №2
    TIM_TimeBaseInit(TIM2, &TIM_BaseConfig);
    //разрешаем генерацию прерывания
    TIM_ITConfig(TIM2, TIM_DIER_UIE, ENABLE);
	// запускаем счет таймера
    TIM_Cmd(TIM2, ENABLE);
}


//обработчик прерывания от таймера 2 - срабатывает 1 раз в секунду
void TIM2_IRQHandler(void)
{
	TIM_ClearFlag(TIM2, TIM_SR_UIF);//Сбрасываем флаг прерывания

	PIN_REVERSE(USER_LED_green);	//просто мигаем раз в секунду светодиодом на плате контроллера

	//==обработчик автоотключения станции==
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

	//====полное отключение из режима предвключения или спящего режима===
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

	//=====отключение режима мигания курсора при изменении установок=====
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

//обработчик прерывания от таймера 3 - работа с энкодером
void TIM3_IRQHandler(void)
{
	  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	  {
	    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//Сбрасываем флаг прерывания

	    if(encoder_value == 0) {encoder_value = 20;}
	    if(TIM3->CR1 & TIM_CR1_DIR){
	    	encoder_value=encoder_value+2;
	    }else if(encoder_value > 0){
	    	encoder_value=encoder_value-2;
	    }


		switch(encBtn){ //задание нач.значения таймера обработчика энкодера
		case SLD_TEMP:  //настройка температуры паяльника
			if(encoder_value > 400) {encoder_value = 400;}
			if(encoder_value < 20)  {encoder_value = 20;}

			if(cursor_cnt_state){sld.temp = encoder_value;}
			break;

		case FEN_TEMP: //настрйока температуры воздушного потока фна

			if(encoder_value > 400) {encoder_value = 400;}
			if(encoder_value < 20)  {encoder_value = 20;}

			if(cursor_cnt_state){fen.temp = encoder_value;}
			break;

		case FEN_AIRFLOW://настройка силы воздушного потока фена
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

//состояние таймера - запущен или нет
u8 get_TIM_state(TIM_TypeDef* TIMx){
	return TIMx->CR1 & TIM_CR1_CEN;
}


