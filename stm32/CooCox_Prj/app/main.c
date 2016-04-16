//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------

#include "main.h"


SolderingStates state_sld = notReady;
SolderingStates state_air = notReady;
u8 count_do_beep = 0;
u16 power_off_count = 0;
u16 auto_sld_off = 0;
u16 auto_fen_off = 0;

#define DEBUG	1

int main()
{
	//static EncoderModes modeSelected = selSolderTemperature;
	init_All();

	while (1)
	{
		check_control_panel_buttons(&state_sld, &state_air);

		if(count_do_beep){//см. в обработчике прерываний от герконов Fen_Gerkon_IRQHandler и Sld_Gerkon_IRQHandler
			beep(count_do_beep);
			count_do_beep = 0;
		}

		//=========Solder=========
		hd44780_goto_xy(0, 0);

		switch(state_sld){
		case notReady:
			hd44780_puts("Sld is out stand");
			break;
		case isPreOn:
		case isSleepMode:
		case isOn:
			printSolderInfoLCD();
			break;
		case isOff:
			hd44780_puts("Sld: =off=       ");
			break;
		}
		//========================

		//========Fen Solder======
		hd44780_goto_xy(1, 0);

		switch(state_air){
		case notReady:
			hd44780_puts("Fen is out stand");
			break;
		case isPreOn:
		case isSleepMode:
		case isOn:
			printFenInfoLCD();
			break;
		case isOff:
			hd44780_puts("Fen: =off=       ");
			break;
		}
		//========================
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


//обработчик прерывания от таймера 2 - срабатывает 1 раз в секунду
void TIM2_IRQHandler(void)
{
	TIM_ClearFlag(TIM2, TIM_SR_UIF);//Сбрасываем флаг прерывания

	PIN_REVERSE(USER_LED_green);	//просто мигаем раз в секунду светодиодом на плате контроллера

	//==обработчик автоотключения станции==
	if((state_air == isOff || state_air == notReady) &&
	   (state_sld == isOff || state_sld == notReady)){
		power_off_count++;
		if(power_off_count > AUTO_POWER_OFF){
			PIN_OFF(RELAY_POWER);
		}
	}else{
		power_off_count = 0;
	}
	//=====================================

	if(state_air == isPreOn || state_air == isSleepMode){
		auto_sld_off++;
		if(auto_sld_off > AUTO_SLD_OFF){
			state_air = isOff;
		}
	}else{
		auto_sld_off=0;
	}
}


void init_All(void){

	SystemInit();

	delay_init();		//обработка задержек

	mcu_gpio_init();	//инициальзация портов

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

	PIN_ON(RELAY_POWER);	//блокируем цепь питания станции

	state_sld = (PIN_STATE(GERKON_SOLDER)) ? isOff : notReady;
	state_air = (PIN_STATE(GERKON_AIR))    ? isOff : notReady;

	init_tim();			//инициализация таймеров
}


