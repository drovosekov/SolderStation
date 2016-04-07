
typedef enum {
	BTN_NONE = 0,
	BTN_ENCODER,
	BTN_SOLDER,
	BTN_AIRSOLDER
} button_state;



u8 get_ctrl_button_state(void);
