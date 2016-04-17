#include "main.h"

#define Fen_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(FEN_GERKON_PIN)
#define Sld_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(SLD_GERKON_PIN)

SLD_INFO sld;
SLD_INFO fen;

u8 count_do_beep;
u16 power_off_count;


u8 get_ctrl_button_state(void){
	u16 ctrl_adc = get_ctrl_buttons_value();
	if(ctrl_adc < 1400){
		return BTN_ENCODER;
	}else if(ctrl_adc > 1500 && ctrl_adc < 1700){
		return BTN_SOLDER;
	}else if(ctrl_adc > 1900 && ctrl_adc < 2000){
		return BTN_FEN;
	}else{
		return BTN_NONE;
	}
}


void Fen_Gerkon_IRQHandler(void)
{
	NVIC_DisableIRQ(PIN_TO_EXTI_IRQn(FEN_GERKON_PIN));

	EXTI->PR = 1<<FEN_GERKON_PIN;
	//обработка события

	power_off_count = 0;
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

	power_off_count = 0;
	if(PIN_STATE(GERKON_SOLDER)){
		if(sld.state != isOff) {count_do_beep = 1;}
		sld.state = (sld.state == notReady || sld.state == isOff) ? isOff : isSleepMode;
	}else if(sld.state == isPreOn || sld.state == isSleepMode){
		sld.state = isOn;
	}

	NVIC_EnableIRQ(PIN_TO_EXTI_IRQn(SLD_GERKON_PIN));
}

void check_control_panel_buttons(){
	EncBtnStates encBtn = SLD_TEMP;
	static u8 btnPressed = 0;

	switch (get_ctrl_button_state()){
	case BTN_SOLDER:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) break;
		if(sld.state == isOn || sld.state == isPreOn){
			sld.state = isOff;
		}else if(PIN_STATE(GERKON_SOLDER)){
			sld.state = isPreOn;
		}else{
			sld.state = isOn;
		}
		btnPressed = 1;
		break;

	case BTN_FEN:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) break;
		if(fen.state == isOn || fen.state == isPreOn){
			fen.state = isOff;
		}else if(PIN_STATE(GERKON_AIR)){
			fen.state = isPreOn;
		}else{
			fen.state = isOn;
		}
		btnPressed = 1;
		break;

	case BTN_ENCODER:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) break;
		switch(encBtn){
		case SLD_TEMP:
			encBtn = FEN_AIRFLOW;
			TIM3->CNT = fen.air_flow;
			break;
		case FEN_AIRFLOW:
			encBtn = FEN_TEMP;
			TIM3->CNT = fen.temp;
			break;
		case FEN_TEMP:
			encBtn = SLD_TEMP;
			TIM3->CNT = sld.temp;
			break;
		}
		btnPressed = 1;
		break;

	case BTN_NONE:
		btnPressed = 0;
		break;

	}
}
