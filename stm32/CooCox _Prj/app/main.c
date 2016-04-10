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
	//static EncoderModes modeSelected = selSolderTemperature;
	SolderingStates state_sld = notReady;
	SolderingStates state_air = notReady;

	init_All();

	PIN_ON(SOLDER_HEATER);
	PIN_ON(AIR_HEATER);
	// запускаем счет таймера
    TIM_Cmd(TIM2, ENABLE);
    PIN_REVERSE(USER_LED_green);

	while (1)
	{
		//////// FOR DEBUG ONLY!! //////////
		PIN_ON(GERKON_SOLDER);
		PIN_ON(GERKON_AIR);
		//////// FOR DEBUG ONLY!! //////////

		check_buttons_states(&state_sld, &state_air);

		//=========Solder=========
		hd44780_goto_xy(0, 0);
		if(!PIN_STATE(GERKON_SOLDER) && state_sld==notReady){
			hd44780_puts("Sld is out stand");
			PIN_OFF(SOLDER_HEATER);
		}else if(!PIN_STATE(GERKON_SOLDER) && (state_sld == isOn || state_sld == isSleepMode)){
			printSolderInfoLCD();
		}else{
			if(state_sld != isOff) {beep(1);}
			state_sld = isOff;
			hd44780_puts("Sld: off         ");
			PIN_OFF(SOLDER_HEATER);
		}
		//========================

		//========Fen Solder======
		hd44780_goto_xy(1, 0);
		if(!PIN_STATE(GERKON_AIR) && state_air==notReady){
			hd44780_puts("Fen is out stand");
			PIN_OFF(AIR_HEATER);
		}else if(!PIN_STATE(GERKON_AIR) && (state_air == isOn || state_air == isSleepMode)){
			printFenInfoLCD();
		}else{
			if(state_air != isOff) {beep(1);}
			state_air = isOff;
			hd44780_puts("Fen: off        ");
			PIN_OFF(AIR_HEATER);
		}
		//========================

		printDot();
	}
}

void check_buttons_states(SolderingStates *sld, SolderingStates *fen){
	EncBtnStates	encBtn = SLD_TEMP;
	static u8 btnPressed = 0;

	switch (get_ctrl_button_state()){
	case BTN_SOLDER:
		if(btnPressed) break;
		if(*sld == isOn){
			*sld = isOff;
			PIN_OFF(SOLDER_HEATER);
		}else{
			*sld = isOn;
			PIN_ON(SOLDER_HEATER);
		}
		btnPressed = 1;
		break;

	case BTN_FEN:
		if(btnPressed) break;
		if(*fen == isOn){
			*fen = isOff;
			PIN_OFF(AIR_HEATER);
		}else{
			*fen = isOn;
			PIN_ON(AIR_HEATER);
		}
		btnPressed = 1;
		break;

	case BTN_ENCODER:
		if(btnPressed) break;
		switch(encBtn){
		case SLD_TEMP:
			encBtn = FEN_AIRFLOW;
			break;
		case FEN_AIRFLOW:
			encBtn = FEN_TEMP;
			break;
		case FEN_TEMP:
			encBtn = SLD_TEMP;
			break;
		}
		btnPressed = 1;
		break;

	case BTN_NONE:
		btnPressed = 0;
		break;

	}
}

void printDot(void){
//выводит мигающую точку в конце второй строки
	static u8 dot = 0;
	hd44780_goto_xy(1, 15);
	if(dot==3){
		hd44780_puts(".");
		dot=-2;
	}
	if(dot==0){hd44780_puts(" ");}
	dot++;
}

void printSolderInfoLCD(void){
	static u16 oldSolderT = 0;
	u16 solderT = 0;

	solderT = get_solder_temp();

	hd44780_puts("Sld: t ");
	if(oldSolderT < solderT){
		hd44780_write_data(SYMB_UP_ARROW);
	}else{
		hd44780_write_data(SYMB_DN_ARROW);
	}
	lcd_write_dec_auto(solderT);
	hd44780_write_data(SYMB_DEGREE);
	hd44780_puts("   ");

	oldSolderT = solderT;
}

void printFenInfoLCD(void){
	static u16 oldAirT = 0;
	u16 airT = 0;

	airT = get_airfen_temp();

	hd44780_puts("Fen: ");
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
	hd44780_puts("   ");

	oldAirT = airT;
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


//обработчик прерывания от таймера 2 - срабатывает 1 раз в секунду
void TIM2_IRQHandler(void)
{
	TIM_ClearFlag(TIM2, TIM_SR_UIF);//Сбрасываем флаг прерывания

	PIN_REVERSE(USER_LED_green);
}


void init_All(void){

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

	init_adc();

	init_tim();

	hd44780_backlight_set(SET);
	draw_logo();
	beep(3);
	hd44780_clear();

}

void draw_logo(void){
	hd44780_goto_xy(0, 1);
	hd44780_puts("Solder station");
	hd44780_goto_xy(1, 1);
	hd44780_puts("Drovosekov.net");
	delay_ms(AFTER_LOGO_DELAY_ms);
}


