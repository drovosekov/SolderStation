
#define ADC1_DR_Address ((uint32_t)0x4001244C)

void init_adc(void);
float get_solder_settemp_value();	//уст. температура паяльника
u8 get_solder_settemp_();
float get_airfen_settemp_value();	//уст. температура фена
u8 get_airfen_settemp();
float get_airfen_airflow_value();	//уст. мощность возд.потока фена
u8 get_airfen_airflow_perc_value();	//уст. мощность возд.потока фена в %
