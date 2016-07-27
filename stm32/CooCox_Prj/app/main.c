//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------

#include "main.h"

SLD_INFO sld;
SLD_INFO fen;

u8 count_do_beep = 0;		//кол-во биппов
u8 cursor_cnt_state;	//счетчик автоотключения мигания курсора
EncBtnStates encBtn;
HeaterDynamic sldHeaterDynamic;

u16 encoder_value;

#define DEBUG	1

int main()
{
	//static EncoderModes modeSelected = selSolderTemperature;
	init_All();
	u16 airT = 0;
	u16 solderT = 0;

	while (1)
	{
		check_control_panel_buttons();

		if(count_do_beep){//см. в обработчике прерываний от герконов Fen_Gerkon_IRQHandler и Sld_Gerkon_IRQHandler
			beep(count_do_beep);
			count_do_beep = 0;
		}

		solderT = get_solder_temp()/10;

		//=========Solder=========
		hd44780_goto_xy(0, 0);
		/*if(solderT>4090){
			hd44780_puts("Sld disconnected");
			PIN_OFF(SOLDER_HEATER);
			PIN_OFF(SOLDER_LEDBTN);
			if(sld.state != notReady) beep(3);
			sld.state = notReady;
		}else{*/
			switch(sld.state){
			case notReady:
				hd44780_puts("Sld is out stand");
				PIN_OFF(SOLDER_HEATER);
				PIN_OFF(SOLDER_LEDBTN);
				break;
			case isOn:
				PIN_ON(SOLDER_LEDBTN);
				if(solderT < sld.temp){
					PIN_ON(SOLDER_HEATER);
				}else{
					PIN_OFF(SOLDER_HEATER);
				}
				printSolderInfoLCD(&solderT);
				break;
			case isPreOn:
			case isSleepMode:
				PIN_REVERSE(SOLDER_LEDBTN);
				printSolderInfoLCD(&solderT);
				if(solderT < sld.temp * 0.6){
					PIN_ON(SOLDER_HEATER);
				}else{
					PIN_OFF(SOLDER_HEATER);
				}
				break;
			case isOff:
				PIN_OFF(SOLDER_HEATER);
				PIN_OFF(SOLDER_LEDBTN);
				hd44780_puts("Sld: =off=       ");
				break;
			}
		//}
		//========================

		//========Fen Solder======
		hd44780_goto_xy(1, 0);
		if(get_airfen_temp() > 4090){
			hd44780_puts("Fen disconnected");
			PIN_OFF(AIR_HEATER);
			PIN_OFF(RELAY_FEN);
			PIN_OFF(AIRFEN_LEDBTN);
			if(fen.state != notReady) beep(3);
			fen.state = notReady;
		}else{
			airT = get_airfen_temp()/10;
			switch(fen.state){
			case notReady:
				hd44780_puts("Fen is out stand");
				PIN_OFF(AIR_HEATER);
				PIN_OFF(RELAY_FEN);
				PIN_OFF(AIRFEN_LEDBTN);
				TIM_SetCompare1(TIM1, 0);
				TIM_Cmd(TIM1, DISABLE);
				break;
			case isOn:
				TIM_Cmd(TIM1, ENABLE);
				TIM_SetCompare1(TIM1, fen.air_flow);
				PIN_ON(RELAY_FEN);
				PIN_ON(AIRFEN_LEDBTN);
				if(airT < fen.temp){
					PIN_ON(AIR_HEATER);
				}else{
					PIN_OFF(AIR_HEATER);
				}
				printFenInfoLCD(&airT);
				break;
			case isPreOn:
			case isSleepMode:
				PIN_REVERSE(AIRFEN_LEDBTN);
				PIN_OFF(AIR_HEATER);
				printFenInfoLCD(&airT);
				break;
			case isOff:
				TIM_SetCompare1(TIM1, 0);
				TIM_Cmd(TIM1, DISABLE);//airmotor off
				PIN_OFF(AIR_FLOW_PWM);
				PIN_OFF(AIR_HEATER);
				PIN_OFF(RELAY_FEN);
				PIN_OFF(AIRFEN_LEDBTN);
				hd44780_puts("Fen: =off=       ");
				break;
			}
		}
		//========================

		//hd44780_goto_xy(0, 12);
		//lcd_write_dec_auto(captured_direction);
	}
}

void beep(u8 count){
	while(count--){
		//PIN_ON(BUZZER);
		buzzer(ENABLE);
		delay_ms(BEEP_DELAY_ms);
		//PIN_OFF(BUZZER);
		buzzer(DISABLE);
		if(count) {delay_ms(BEEP_DELAY_ms);}
	}
}

void buzzer(u8 state){
	TIM_Cmd(TIM4, state);
}



void init_All(void){

	SystemInit();

	delay_init();		//обработка задержек

	mcu_gpio_init();	//инициальзация портов

	PIN_ON(RELAY_POWER);	//блокируем цепь питания станции
	//PIN_OFF(AIR_FLOW_PWM);

	hd44780_init();		//инициализация LCD дисплея

	init_user_chars();	//загрузка пользовательских символов

	init_adc();			//инициализация АЦП

	external_init();	//инициализация обработчиков прерываний

	hd44780_backlight(SET);//включаем подсветку экрана

#if !DEBUG
	draw_logo();		//рисуем логотип
	beep(3);			//три раза бипп
#endif

	hd44780_clear();

	sld.state = (PIN_STATE(GERKON_SOLDER)) ? isOff : notReady;
	fen.state = (PIN_STATE(GERKON_AIR))    ? isOff : notReady;

	init_tim();			//инициализация таймеров
}


