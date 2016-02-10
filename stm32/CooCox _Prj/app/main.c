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
	u8 symbUpTemp = 0;

	init_All();

	PIN_ON(SOLDER_HEATER);
	PIN_ON(AIR_HEATER);

	turnon_backlight();
	hd44780_goto_xy(0, 0);
	hd44780_printf(" Solder station \n drovosekov.net");
	delay_ms(3000);
	hd44780_clear();

	while (1)
	{
		hd44780_goto_xy(0, 0);

		//if(modeSelected == selSolderTemperature){
		solderT = get_solder_settemp();
		symbUpTemp = (oldSolderT < solderT) ? 1 : 20;
		hd44780_printf("Solder t: %c%d%c  \n", symbUpTemp, solderT, 8);

		airT = get_airfen_settemp();

		hd44780_printf("Air: ");
		if(PIN_STATE(SELECT_BTN)){
			hd44780_printf("%d%%  ", get_airfen_airflow_perc_value());

			hd44780_goto_xy(1, 10);
			symbUpTemp = (oldAirT < airT) ? 1 : 20;
			hd44780_printf("%c%d%c  ", symbUpTemp, airT, 8);
		}else{
			hd44780_printf("off         ");
		}

		//if(PIN_STATE(USER_BTN)){
			turnon_backlight();
		//}

		if(PIN_STATE(GERKON_SOLDER)){
			PIN_OFF(SOLDER_HEATER);
			PIN_ON(SOLDER_GREEN_LED);
		}else{
			PIN_OFF(SOLDER_GREEN_LED);
			PIN_ON(SOLDER_HEATER);
		}

		if(PIN_STATE(GERKON_AIR)){
			PIN_OFF(AIR_HEATER);
			PIN_ON(AIR_GREEN_LED);
		}else{
			PIN_OFF(AIR_GREEN_LED);
			PIN_ON(AIR_HEATER);
		}

		oldSolderT = solderT;
		oldAirT = airT;
	}
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
}



