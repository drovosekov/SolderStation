//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------

#include "main.h"

static u8 i=0;

int main()
{
	init_All();

	while (1)
	{
		hd44780_goto_xy(0, 0);

		hd44780_printf("Solder t: %c%d%c\n", 1, 260, 8);

		hd44780_printf("Air: ");
		if(PIN_STATE(USER_BTN)){
			hd44780_printf("%d%%", 87);

			hd44780_goto_xy(1, 10);
			hd44780_printf("%c%d%c", 20, 155, 8);
		}else{
			hd44780_printf("off         ");
		}

		if(PIN_STATE(USER_BTN)){
			turnon_backlight();
		}

		delay_ms(50);

		blink_led();
	}
}

void turnon_backlight(){
	TIM_Cmd(TIM6, DISABLE);
	TIM_Cmd(TIM6, ENABLE);
	hd44780_backlight_set(1);
}


void blink_led(){
#ifdef STM32F100_DISCOVERY_BOARD
	if(get_TIM_state(TIM6)){
		PIN_ON(USER_LED_blue);
	}else{
		PIN_OFF(USER_LED_blue);
	}
#endif
	if(i % 2 == 0){
		PIN_ON(USER_LED);
	}else{
		PIN_OFF(USER_LED);
	}
	i++;
	if(i==256){i=0;}
}

void init_All(){

	SystemInit();

	//configure SysTimer to 1 us
	while(SysTick_Config(SystemCoreClock / 240000));

	mcu_gpio_deinit();

	hd44780_init();


	u8 user_char[8]; //Сюда будем записывать пользовательский символ

	user_char[0]=0b11000; // симовл
	user_char[1]=0b11000; // градус C
	user_char[2]=0b00000; //
	user_char[3]=0b00110; //
	user_char[4]=0b01001; //
	user_char[5]=0b01000; //
	user_char[6]=0b01001;
	user_char[7]=0b00110;

	hd44780_set_user_char(0, user_char);

	user_char[0]=0b00000;
	user_char[1]=0b00100; // стрелка вверх
	user_char[2]=0b01110; //
	user_char[3]=0b10101; //
	user_char[4]=0b00100; //
	user_char[5]=0b00100; //
	user_char[6]=0b00100; //
	user_char[7]=0b00000;

	hd44780_set_user_char(1, user_char);


	//init_adc();

	init_tim();
}



//состояние таймера - запущен или нет
u8 get_TIM_state(TIM_TypeDef* TIMx){
	return TIMx->CR1 & TIM_CR1_CEN;
}
