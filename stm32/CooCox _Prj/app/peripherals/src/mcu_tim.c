#include "main.h"

void init_tim(){
    // Конфигурация таймера в режим PWM
    TIM_TimeBaseInitTypeDef TIM_BaseConfig;
    // Конфигурация выхода таймера
    TIM_OCInitTypeDef TIM_OCConfig;

    //ШИМ для вентилятора фена
    // Конфигурируем выход таймера, режим - PWM1
    TIM_OCConfig.TIM_OCMode = TIM_OCMode_PWM1;
    // Собственно - выход включен
    TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
    // Пульс длинной 500 тактов => 500/1000 = 50%
    TIM_OCConfig.TIM_Pulse = 500;
    // Полярность => пульс - это единица (+3.3V)
    TIM_OCConfig.TIM_OCPolarity = TIM_OCPolarity_Low;
    // Инициализируем 3й выход таймера №3
    TIM_OC3Init(TIM2, &TIM_OCConfig);

    TIM_Cmd(TIM2, DISABLE);


	//устанавливаем уровень приоритета прерывания от таймера TIM4
	NVIC_SetPriority(TIM4_IRQn, 2);
	//разрешаем обработку прерывания от таймера TIM4
	NVIC_EnableIRQ(TIM4_IRQn);
    TIM_BaseConfig.TIM_Prescaler = 23999;
    // Период - 1 раз в минуту
    TIM_BaseConfig.TIM_Period = 60000;
    // Инициализируем таймер №4
    TIM_TimeBaseInit(TIM4, &TIM_BaseConfig);
    //разрешаем генерацию прерывания
    TIM_ITConfig(TIM4, TIM_DIER_UIE, ENABLE);
    TIM_Cmd(TIM4, DISABLE);


	/* задержка перед отключением вентилятора фена */
	//устанавливаем уровень приоритета прерывания от таймера TIM6
	NVIC_SetPriority(TIM6_DAC_IRQn, 3);
	//разрешаем обработку прерывания от таймера TIM6
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
    // Запускаем таймер на тактовой частоте в 1 kHz
    TIM_BaseConfig.TIM_Prescaler = 23999;
    // Период - 10 сек
    TIM_BaseConfig.TIM_Period = delay_airflow_off_ms; //задержка отключения в мс
    // Инициализируем таймер №6
    TIM_TimeBaseInit(TIM6, &TIM_BaseConfig);
    //отключаем генирацию прерывания сразу после запуска таймера
	TIM_GenerateEvent(TIM6, TIM_EGR_UG);
	TIM_ClearFlag(TIM6, TIM_SR_UIF);
    //разрешаем генерацию прерывания
    TIM_ITConfig(TIM6, TIM_DIER_UIE, ENABLE);
	// запускаем счет таймера
    TIM_SelectOnePulseMode(TIM6, TIM_CR1_OPM);
    TIM_Cmd(TIM6, DISABLE);
}


//обработчик прерывания от таймера 6
//задержка перед отключением ручного режима полива - 3мин
void TIM6_DAC_IRQHandler(void)
{
	hd44780_backlight_set(0);

	//TIM_ClearFlag(TIM6, TIM_SR_UIF);//Сбрасываем флаг прерывания
	TIM_Cmd(TIM6, DISABLE);
}
