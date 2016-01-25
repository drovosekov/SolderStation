#include "main.h"


 static __IO uint16_t ADCConvertedValue[2];  //Array that is used to store the calculated DMA values for ADC1

	 /*
	  * Sets up Pin.C4 (Channel 14) and Pin.C5 (Channel 15) to be used as analog inputs.  The first channel
	  * of the DMA is also setup to be used with ADC1.  ADC1 is setup to continuously output data to the
	  * array "ADCConvertedValue"
	  */
void init_adc(void){

	    DMA_InitTypeDef DMA_InitStructure;  //Variable used to setup the DMA
	    ADC_InitTypeDef ADC_InitStructure;  //Variable used to setup the ADC

	    //==Configure the systems clocks for the ADC and DMA==
	    //ADCCLK = PCLK2 / 4
	    RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //Defines the ADC clock divider.  This clock is derived from the APB2 clock (PCLK2).  The
	    //ADCs are clocked by the clock of the high speed domian (APB2) dibivied by 2/4/6/8.
	    //The frequency can never be bigger than 14MHz!!!!

	    //--Enable DMA1 clock--
	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	    //--Enable ADC1 --
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	    //==Configure DMA1 - Channel1==
	    DMA_DeInit(DMA1_Channel1);  //Set DMA registers to default values
	    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;  //Address of peripheral the DMA must map to
	    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) & ADCConvertedValue;  //Variable to which ADC values will be stored
	    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	    DMA_InitStructure.DMA_BufferSize = 2;  //Buffer size (2 because we using two channels)
	    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //Initialise the DMA
	    DMA_Cmd(DMA1_Channel1, ENABLE);  //Enable the DMA1 - Channel1


	    //==Configure ADC1 - Channel 1 and Channel 2==
	    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	    ADC_InitStructure.ADC_NbrOfChannel = 2;  //We using two channels

	    ADC_Init(ADC1, &ADC_InitStructure);  //Initialise ADC1

	    //Setup order in which the Channels are sampled....
	    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 0, ADC_SampleTime_55Cycles5);
	    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);
	    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5);

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

float get_solder_settemp_value(){
	return ADCConvertedValue[0];
}

float get_airfen_settemp_value(){
	return ADCConvertedValue[1];
}

float get_airfen_airflow_value(){
	return ADCConvertedValue[2];
}

u8 get_solder_settemp_(){
	return get_solder_settemp_value();
}

u8 get_airfen_settemp(){
	return get_airfen_settemp_value();
}

u8 get_airfen_airflow_perc_value(){
	const u16 vMAX=	100;	//верхний уровень
	const u16 vMIN=	0;		//нижний уровень
	return 100-100*(vMAX-get_airfen_airflow_value())/(vMAX-vMIN);
}
