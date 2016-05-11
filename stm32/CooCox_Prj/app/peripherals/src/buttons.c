#include "main.h"

#define Fen_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(FEN_GERKON_PIN)
#define Sld_Gerkon_IRQHandler             PIN_TO_EXTI_HANDLER(SLD_GERKON_PIN)

SLD_INFO sld;
SLD_INFO fen;

u8 count_do_beep;
u16 power_off_count;
s8 cursor_cnt_state;
EncBtnStates encBtn = FEN_TEMP;

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


//обработка нажатия кнопок подключенных к АЦП
void check_control_panel_buttons(){
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

		if(btnPressed) {break;}

		if((fen.state == isOff || fen.state == notReady) &&
		   (sld.state == isOff || sld.state == notReady)) {break;}

		count_do_beep=1;

		switch(encBtn){
		case SLD_TEMP: //настройка температуры паяльника
			if(!(fen.state == isOff || fen.state == notReady)){
				encBtn = FEN_AIRFLOW;
			}
			break;

		case FEN_AIRFLOW: //настройка силы воздушного потока фена
			encBtn = FEN_TEMP;
			break;

		case FEN_TEMP: //настрйока температуры воздушного потока фна
			if(sld.state == isOff || sld.state == notReady){
				encBtn = FEN_AIRFLOW;
			}else{
				encBtn = SLD_TEMP;
			}
			break;
		}

		switch(encBtn){ //задание нач.значения таймера обработчика энкодера
		case SLD_TEMP:
			TIM3->CNT = sld.temp;
			break;

		case FEN_AIRFLOW:
			TIM3->CNT = fen.air_flow;
			break;

		case FEN_TEMP:
			TIM3->CNT = fen.temp;
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
