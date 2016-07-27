#include "main.h"

#define ADC_SAMPLE_TIME		ADC_SampleTime_239Cycles5	//количество тактов процессора для усреднения значения АЦП (здесь для всех используемых каналов одинаковая)
#define ADC_COUNT_CHANELS	3  							//количество каналов
#define ADC_PRECISION 		20 							//задается точность +/- от считанного значения не повлияет на изменение результата измерений (см. ф-ию get_value_w_precision)

static __IO u16 ADCConvertedValue[ADC_COUNT_CHANELS];	//массив значений с АЦП полученных через DMA

	 /*
	  * The first channel of the DMA is setup to be used with ADC1.
	  * ADC1 is setup to continuously output data to the
	  * array "ADCConvertedValue"
	  */
void init_adc(void){
	u8 adc_cnt;

	DMA_InitTypeDef DMA_InitStructure;  //Variable used to setup the DMA
	ADC_InitTypeDef ADC_InitStructure;  //Variable used to setup the ADC

	//==Configure the systems clocks for the ADC and DMA==
	//ADCCLK = PCLK2 / 4
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);  //Defines the ADC clock divider.  This clock is derived from the APB2 clock (PCLK2).  The
	//ADCs are clocked by the clock of the high speed domian (APB2) dibivied by 2/4/6/8.
	//The frequency can never be bigger than 14MHz!!!!

	//--Enable DMA1 clock--
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//разрешаем тактирование АЦП
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//==Configure DMA1 - Channel1==
	DMA_DeInit(DMA1_Channel1);  //Set DMA registers to default values
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;  //Address of peripheral the DMA must map to
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) & ADCConvertedValue;  //Variable to which ADC values will be stored
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_COUNT_CHANELS;  //Buffer size (count using channels)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //Initialise the DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);  //Enable the DMA1 - Channel1


	//==Configure ADC1 - Channels
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = ADC_COUNT_CHANELS;

	ADC_Init(ADC1, &ADC_InitStructure);  //Initialise ADC1

	//Setup order in which the Channels are sampled....
	for (adc_cnt = 0; adc_cnt < ADC_COUNT_CHANELS; adc_cnt++) {
		ADC_RegularChannelConfig(ADC1, adc_cnt, adc_cnt, ADC_SAMPLE_TIME);
	}
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
	//ADC_InjectedChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_13Cycles5);

	ADC_DMACmd(ADC1, ENABLE);  //Enable ADC1 DMA
	ADC_Cmd(ADC1, ENABLE);  //Enable ADC1

	//==Calibrate ADC1==

	//Enable ADC1 reset calibaration register
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));  //Check the end of ADC1 reset calibration register

	//Start ADC1 calibaration
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));  //Check the end of ADC1 calibration

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}//end ADC_Configuration


u16 get_ctrl_buttons_value(){//с делителя напряжения на кнопках управления
	return get_value_w_precision(2);
}

u16 get_solder_temp(){//температура паяльника
	return get_value_w_precision(0);
}

u16 get_airfen_temp(){//температура фена
	return get_value_w_precision(1);
}

/*u16 get_airfen_airflow_perc_value(){//мощность потока воздуха в %
	const u16 vMAX=	4028;	//верхний уровень
	const u16 vMIN=	0;		//нижний уровень

	return (100-100*(vMAX-get_value_w_precision(0))/(vMAX-vMIN));
}*/

u16 get_value_w_precision(u8 idxArray){
	static u16 oldADCvalues[ADC_COUNT_CHANELS];  			//массив запомненых значений для учета погрешности

	if((ADCConvertedValue[idxArray] > oldADCvalues[idxArray] + ADC_PRECISION) || (oldADCvalues[idxArray] - ADC_PRECISION > ADCConvertedValue[idxArray])) {
		oldADCvalues[idxArray] = ADCConvertedValue[idxArray];
	}
	return oldADCvalues[idxArray];
}
