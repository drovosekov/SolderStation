#include "main.h"

#define Fen_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(FEN_GERKON_PIN)
#define Sld_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(SLD_GERKON_PIN)

SLD_INFO sld;
SLD_INFO fen;

u8 count_do_beep;
u16 power_off_count;
s8 cursor_cnt_state;
u16 encoder_value;
EncBtnStates encBtn = SEL_OFF;

void Fen_Gerkon_IRQHandler(void)
{
	NVIC_DisableIRQ(PIN_TO_EXTI_IRQn(FEN_GERKON_PIN));

	EXTI->PR = 1<<FEN_GERKON_PIN;
	//обработка события

	power_off_count = 0;//сброс автоотключения

	if(PIN_STATE(GERKON_AIR)){
		if(fen.state != isOff) {count_do_beep = 1;}
		fen.state = (fen.state == notReady || fen.state == isOff) ? isOff : isSleepMode;
	}else if(fen.state == isPreOn || fen.state == isSleepMode){
		fen.state = isOn;
	}

	NVIC_EnableIRQ(PIN_TO_EXTI_IRQn(FEN_GERKON_PIN));
}

void Sld_Gerkon_IRQHandler(void)
{
	NVIC_DisableIRQ(PIN_TO_EXTI_IRQn(SLD_GERKON_PIN));

	EXTI->PR = 1<<SLD_GERKON_PIN;
	//обработка события

	power_off_count = 0;//сброс автоотключения

	if(PIN_STATE(GERKON_SOLDER)){
		if(sld.state != isOff) {count_do_beep = 1;}
		sld.state = (sld.state == notReady || sld.state == isOff) ? isOff : isSleepMode;
	}else if(sld.state == isPreOn || sld.state == isSleepMode){
		sld.state = isOn;
	}

	NVIC_EnableIRQ(PIN_TO_EXTI_IRQn(SLD_GERKON_PIN));
}

//состояние кнопок подключеных к АЦП
button_state get_ctrl_button_state(void){
	u16 ctrl_adc = get_ctrl_buttons_value();
	if(ctrl_adc < 500){
		return BTN_ENCODER;
	}else if(ctrl_adc > 600 && ctrl_adc < 900){
		return BTN_FEN;
	}else if(ctrl_adc > 1800 && ctrl_adc < 2300){
		return BTN_SOLDER;
	}else{
		return BTN_NONE;
	}
}


//обработка нажатия кнопок подключенных к АЦП
void check_control_panel_buttons(){
	static u8 btnPressed = 0;

	switch (get_ctrl_button_state()){
	case BTN_SOLDER:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) break;
		if(sld.state == isOn || sld.state == isPreOn || sld.state == isFireProtect){
			sld.state = isOff;
		}else if(PIN_STATE(GERKON_SOLDER)){
			sld.state = isPreOn;
		}else{
			if(fen.state = isOn) {fen.state = isOff;}
			sld.state = isOn;
		}
		btnPressed = 1;
		break;

	case BTN_FEN:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) break;
		if(fen.state == isOn || fen.state == isPreOn || fen.state == isFireProtect){
			fen.state = isOff;
		}else if(PIN_STATE(GERKON_AIR)){
			fen.state = isPreOn;
		}else{
			if(sld.state = isOn) {sld.state = isOff;}
			fen.state = isOn;
		}
		btnPressed = 1;
		break;

	case BTN_ENCODER:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) {break;}

		if((fen.state == isOff || fen.state == notReady || fen.state == isFireProtect) &&
		   (sld.state == isOff || sld.state == notReady || sld.state == isFireProtect)) {break;}

		count_do_beep=1;

		switch(encBtn){
		case SEL_OFF:
			if(sld.state == isOff || sld.state == notReady){
				encBtn = FEN_TEMP;
			}else{
				encBtn = SLD_TEMP;
			}
			break;

		case SLD_TEMP:
			if(fen.state == isOff || fen.state == notReady){
				encBtn = SEL_OFF;
			}else{
				encBtn = FEN_TEMP;
			}
			break;

		case FEN_AIRFLOW:
			encBtn = SEL_OFF;
			break;

		case FEN_TEMP:
			encBtn = FEN_AIRFLOW;
			break;
		}

		switch(encBtn){
		case SLD_TEMP:  //настройка температуры паяльника
			encoder_value = sld.temp;
			break;

		case FEN_AIRFLOW://настройка силы воздушного потока фена
			encoder_value = fen.air_flow;
			break;

		case FEN_TEMP: //настрйока температуры воздушного потока фена
			encoder_value = fen.temp;
			break;

		case SEL_OFF:
			cursor_cnt_state = 0;
			btnPressed = 1;
			return;
			break;
		}

		cursor_cnt_state = CURSOR_OFF_TIMEOUT; //включаем имитацию курсора на заданое время в секундах

		btnPressed = 1;
		break;

	case BTN_NONE:
		btnPressed = 0;
		break;

	}
}
