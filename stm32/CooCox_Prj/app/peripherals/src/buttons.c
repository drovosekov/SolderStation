#include "main.h"

#define Fen_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(FEN_GERKON_PIN)
#define Sld_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(SLD_GERKON_PIN)

SolderingStates state_sld;
SolderingStates state_air;
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
		if(state_air != isOff) {count_do_beep = 1;}
		state_air = (state_air == notReady || state_air == isOff) ? isOff : isSleepMode;
	}else if(state_air == isPreOn || state_air == isSleepMode){
		state_air = isOn;
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
		if(state_sld != isOff) {count_do_beep = 1;}
		state_sld = (state_sld == notReady || state_sld == isOff) ? isOff : isSleepMode;
	}else if(state_sld == isPreOn || state_sld == isSleepMode){
		state_sld = isOn;
	}

	NVIC_EnableIRQ(PIN_TO_EXTI_IRQn(SLD_GERKON_PIN));
}

void check_control_panel_buttons(SolderingStates *sld, SolderingStates *fen){
	EncBtnStates	encBtn = SLD_TEMP;
	static u8 btnPressed = 0;

	switch (get_ctrl_button_state()){
	case BTN_SOLDER:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) break;
		if(*sld == isOn || *sld == isPreOn){
			*sld = isOff;
		}else if(PIN_STATE(GERKON_SOLDER)){
			*sld = isPreOn;
		}else{
			*sld = isOn;
		}
		btnPressed = 1;
		break;

	case BTN_FEN:
		power_off_count = 0;//сброс автоотключения

		if(btnPressed) break;
		if(*fen == isOn || *fen == isPreOn){
			*fen = isOff;
		}else if(PIN_STATE(GERKON_AIR)){
			*fen = isPreOn;
		}else{
			*fen = isOn;
		}
		btnPressed = 1;
		break;

	case BTN_ENCODER:

		power_off_count = 0;//сброс автоотключения

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
