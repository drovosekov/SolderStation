
typedef enum {
	BTN_NONE = 0,
	BTN_ENCODER,
	BTN_SOLDER,
	BTN_FEN
} button_state;

typedef enum{
	SLD_TEMP = 0,
	FEN_AIRFLOW,
	FEN_TEMP
} EncBtnStates;

//��������� ������ ����������� � ���
button_state get_ctrl_button_state(void);
//��������� ������� ������ ������������ � ���
void check_control_panel_buttons();
