
#define ADC1_DR_Address ((uint32_t)0x4001244C)

void init_adc(void);
u16 get_solder_settemp();
u16 get_airfen_settemp();
u16 get_airfen_airflow_perc_value();
u16 get_value_w_precision(u8 idxArray);
