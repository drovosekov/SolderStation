#include "main.h"

u16 power_off_count;	//счетчик авототключения станции
u8 cursor_cnt_state;	//счетчик автоотключения мигания курсора
EncBtnStates encBtn;

SLD_INFO sld;
SLD_INFO fen;

void init_tim(){
    // Конфигурация таймера в режим PWM
    TIM_TimeBaseInitTypeDef TIM_BaseConfig;
    // Конфигурация выхода таймера
    TIM_OCInitTypeDef TIM_OCConfig;

    //энкодер
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    TIM_BaseConfig.TIM_Period = 512;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_BaseConfig);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    //TIM3->CNT=0; //начальное значение
    TIM_Cmd(TIM3, ENABLE);


    //ШИМ для вентилятора фена
    // Конфигурируем выход таймера, режим - PWM1
    TIM_OCConfig.TIM_OCMode = TIM_OCMode_PWM1;
    // Собственно - выход включен
    TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
    // Пульс длинной 500 тактов => 500/1000 = 50%
    TIM_OCConfig.TIM_Pulse = 500;
    // Полярность => пульс - это единица (+3.3V)
    TIM_OCConfig.TIM_OCPolarity = TIM_OCPolarity_Low;
    // Инициализируем 1й выход таймера №1
    TIM_OC1Init(TIM1, &TIM_OCConfig);
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
	snd_timebase.TIM_ClockDivision = 0;
	snd_timebase.TIM_RepetitionCounter = 0;
	snd_timebase.TIM_Period = (SystemCoreClock / (BUZZER_FREQ << 1)) - 1;
	TIM_TimeBaseInit(TIM4, &snd_timebase);
    TIM_Cmd(TIM4, DISABLE);


	//устанавливаем уровень приоритета прерывания от таймера TIM2
	NVIC_SetPriority(TIM2_IRQn, 3);
	//разрешаем обработку прерывания от таймера TIM2
	NVIC_EnableIRQ(TIM2_IRQn);
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
	if(cursor_cnt_state > 1){
		cursor_cnt_state--;
	}else if(cursor_cnt_state == 1){
		cursor_cnt_state = 0;
		encBtn = FEN_TEMP;
	}
	//====================================================================
}


//состояние таймера - запущен или нет
u8 get_TIM_state(TIM_TypeDef* TIMx){
	return TIMx->CR1 & TIM_CR1_CEN;
}
