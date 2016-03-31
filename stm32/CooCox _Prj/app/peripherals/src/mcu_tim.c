#include "main.h"

void init_tim(){
    // Конфигурация таймера в режим PWM
    TIM_TimeBaseInitTypeDef TIM_BaseConfig;
    // Конфигурация выхода таймера
    TIM_OCInitTypeDef TIM_OCConfig;

    //работа энкодера
    TIM_TimeBaseStructInit(&TIM_BaseConfig);
    TIM_BaseConfig.TIM_Period = 512;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_BaseConfig);
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM3->CNT=22; //начальное значение
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
    TIM_Cmd(TIM2, DISABLE);


    /* ежесекундный таймер */
	//устанавливаем уровень приоритета прерывания от таймера TIM7
	NVIC_SetPriority(TIM7_IRQn, 2);
	//разрешаем обработку прерывания от таймера TIM7
	NVIC_EnableIRQ(TIM7_IRQn);
    // Запускаем таймер на тактовой частоте в 1 kHz
    TIM_BaseConfig.TIM_Prescaler = 23999;
    // Период - 1000 тактов => 1000/1000 = 1 Hz
    TIM_BaseConfig.TIM_Period = 1000;
    // Инициализируем таймер №7
    TIM_TimeBaseInit(TIM7, &TIM_BaseConfig);
    //разрешаем генерацию прерывания
    TIM_ITConfig(TIM7, TIM_DIER_UIE, ENABLE);
	// запускаем счет таймера
	TIM_Cmd(TIM7, DISABLE);
}


//состояние таймера - запущен или нет
u8 get_TIM_state(TIM_TypeDef* TIMx){
	return TIMx->CR1 & TIM_CR1_CEN;
}
