#include "main.h"

u8 get_ctrl_button_state(void){
	u16 ctrl_adc = get_ctrl_buttons_value();
	if(ctrl_adc<1400){
		return BTN_ENCODER;
	}else if(ctrl_adc>1500 && ctrl_adc<1700){
		return BTN_SOLDER;
	}else if(ctrl_adc>1900 && ctrl_adc<2000){
		return BTN_AIRSOLDER;
	}else{
		return BTN_NONE;
	}
}
