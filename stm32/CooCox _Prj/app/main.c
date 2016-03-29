//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------

#include "main.h"

int main()
{
	static u16 oldSolderT = 0, oldAirT = 0;
	//static EncoderModes modeSelected = selSolderTemperature;
	u16 airT = 0, solderT = 0;

	init_All();

	PIN_ON(SOLDER_HEATER);
	PIN_ON(AIR_HEATER);

	while (1)
	{
		//=========Solder=========
		solderT = get_ctrl_buttons_value();//get_solder_temp();
		hd44780_goto_xy(0, 0);
		hd44780_puts("Solder t: ");
		if(oldSolderT < solderT){
			hd44780_write_data(SYMB_UP_ARROW);
		}else{
			hd44780_write_data(SYMB_DN_ARROW);
		}
		lcd_write_dec_auto(solderT);
		hd44780_write_data(SYMB_DEGREE);
		//========================

		//====AirFlow Solder======
		airT = get_airfen_temp();

		hd44780_goto_xy(1, 0);
		hd44780_puts("Air: ");
		if(!PIN_STATE(GERKON_AIR)){
			lcd_write_dec_auto(0);
			hd44780_puts("%   ");

			hd44780_goto_xy(1, 10);
			if(oldAirT < airT){
				hd44780_write_data(SYMB_UP_ARROW);
			}else{
				hd44780_write_data(SYMB_DN_ARROW);
			}
			lcd_write_dec_auto(airT);
			hd44780_write_data(SYMB_DEGREE);
		}else{
			hd44780_puts("off         ");
		}
		//========================

		if (PIN_STATE(GERKON_AIR)) {
			PIN_ON(AIR_HEATER);
			beep(2);
		} else {
			PIN_OFF(AIR_HEATER);
		}

		if(PIN_STATE(GERKON_SOLDER)){
			PIN_ON(SOLDER_HEATER);
		}else{
			PIN_OFF(SOLDER_HEATER);
		}

		oldSolderT = solderT;
		oldAirT = airT;
	}
}

void beep(u8 count){
	while(count--){
		buzzer(ENABLE);
		delay_ms(BEEP_DELAY_ms);
		buzzer(DISABLE);
		if(count) {delay_ms(BEEP_DELAY_ms);}
	}
}

void buzzer(u8 state){
	TIM_Cmd(TIM4, state);
}

void turnon_backlight(){
	//TIM_Cmd(TIM6, DISABLE);
	//TIM_Cmd(TIM6, ENABLE);
	hd44780_backlight_set(1);
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

	turnon_backlight();
	draw_logo();
	beep(3);
	hd44780_clear();

}

void draw_logo(){
	hd44780_goto_xy(0, 1);
	hd44780_puts("Solder station");
	hd44780_goto_xy(1, 1);
	hd44780_puts("Drovosekov.net");
	delay_ms(AFTER_LOGO_DELAY_ms);
}


