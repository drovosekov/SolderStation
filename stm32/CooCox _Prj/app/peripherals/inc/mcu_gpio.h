/*
 *	File:	GPIO.h
 *	Date:	07.01.2011
 */

#ifndef MCU_GPIO_H_
#define MCU_GPIO_H_

void mcu_gpio_deinit();

/////should be defined by user /////

/* ADC defines */
#define SOLDER_TEMP_ADC			A, 1, LOW, ANALOG, SPEED_2MHZ //уст. температуры паяльника
#define AIRFEN_TEMP_ADC			A, 2, LOW, ANALOG, SPEED_2MHZ //уст. температуры фена
/* end ADC defines */

/* buttons defines */
#define ENCODER_A				A, 6, HIGH, INPUT_PULL_UP, SPEED_10MHZ //энкодер вход 1
#define ENCODER_B				A, 7, HIGH, INPUT_PULL_UP, SPEED_10MHZ //энкодер вход 2

#define SELECT_BTN				C, 15, HIGH, INPUT_PULL_UP, SPEED_2MHZ //кнопка на энкодере
#define GERKON_AIR				A, 15, HIGH, INPUT_PULL_UP, SPEED_2MHZ //геркон установки фена на подставку
#define GERKON_SOLDER			B, 4, HIGH,  INPUT_PULL_UP, SPEED_2MHZ  //геркон установки паяльника на подставку

#ifdef STM32F100_DISCOVERY_BOARD
	#define USER_BTN			A, 0, HIGH, INPUT_PULL_UP, SPEED_2MHZ 				//user button on STM32F100R8T6B dev board
	#define USER_LED_green		C, 9, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ   //user LED green on STM32F100R8T6B dev board
	#define USER_LED_blue		C, 8, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ   //user LED blue on STM32F100R8T6B dev board
#else
	#define USER_LED_green		C, 13, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ  //user LED green on STM32F103C8T6 dev board
#endif
/* end buttons defines */

/* out pins defines */
#define SOLDER_HEATER			B, 6, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ  //вкл./выкл. нагревателя паяльника
#define SOLDER_GREEN_LED		B, 5, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ  //зеленый светодиод паяльника
#define AIR_HEATER				B, 8, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ  //вкл./выкл. нагревателя фена
#define AIR_GREEN_LED			B, 9, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ  //зеленый светодиод фена
#define AIR_FLOW_PWM			B, 13, HIGH, GENERAL_OUTPUT_PUSH_PULL, SPEED_10MHZ //выход TIM1 CH1N - ШИМ для фена
/* end out pins defines */


///////////////////////////////////


//-----------------------------------------------------------------------------
#define GPIO_PIN_SPEED_2MHZ()	(2UL)
#define GPIO_PIN_SPEED_10MHZ()	(1UL)
#define GPIO_PIN_SPEED_50MHZ()	(3UL)

#define BIT(NUMBER)		(1UL << (NUMBER))

//-----------------------------------------------------------------------------
#define GPIO_PIN0_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 0); \
			  GPIO##PORT->CRL |= ((MODE) << 0); }
#define GPIO_PIN1_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 4); \
			  GPIO##PORT->CRL |= ((MODE) << 4); }
#define GPIO_PIN2_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 8); \
			  GPIO##PORT->CRL |= ((MODE) << 8); }
#define GPIO_PIN3_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 12); \
			  GPIO##PORT->CRL |= ((MODE) << 12); }
#define GPIO_PIN4_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 16); \
			  GPIO##PORT->CRL |= ((MODE) << 16); }
#define GPIO_PIN5_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 20); \
			  GPIO##PORT->CRL |= ((MODE) << 20); }
#define GPIO_PIN6_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 24); \
			  GPIO##PORT->CRL |= ((MODE) << 24); }
#define GPIO_PIN7_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRL &= ~(15UL << 28); \
			  GPIO##PORT->CRL |= ((MODE) << 28); }
#define GPIO_PIN8_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 0); \
			  GPIO##PORT->CRH |= ((MODE) << 0); }
#define GPIO_PIN9_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 4); \
			  GPIO##PORT->CRH |= ((MODE) << 4); }
#define GPIO_PIN10_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 8); \
			  GPIO##PORT->CRH |= ((MODE) << 8); }
#define GPIO_PIN11_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 12); \
			  GPIO##PORT->CRH |= ((MODE) << 12); }
#define GPIO_PIN12_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 16); \
			  GPIO##PORT->CRH |= ((MODE) << 16); }
#define GPIO_PIN13_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 20); \
			  GPIO##PORT->CRH |= ((MODE) << 20); }
#define GPIO_PIN14_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 24); \
			  GPIO##PORT->CRH |= ((MODE) << 24); }
#define GPIO_PIN15_CONFIGURATION(PORT, MODE) \
			{ GPIO##PORT->CRH &= ~(15UL << 28); \
			  GPIO##PORT->CRH |= ((MODE) << 28); }

//-----------------------------------------------------------------------------
#define GPIO_PIN_CONFIGURATION_ANALOG(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 0UL) }

#define GPIO_PIN_CONFIGURATION_INPUT_FLOATING(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 4UL) }

#define GPIO_PIN_CONFIGURATION_INPUT_PULL_DOWN(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 8UL); \
			  GPIO##PORT->BSRR |= (1UL << PIN) << 16; }

#define GPIO_PIN_CONFIGURATION_INPUT_PULL_UP(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 8UL);  \
	          GPIO##PORT->BSRR |= (1UL << PIN); }

#define GPIO_PIN_CONFIGURATION_GENERAL_OUTPUT_PUSH_PULL(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 0UL | GPIO_PIN_##SPEED()) }

#define GPIO_PIN_CONFIGURATION_GENERAL_OUTPUT_OPEN_DRAIN(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 4UL | GPIO_PIN_##SPEED()) }

#define GPIO_PIN_CONFIGURATION_ALTERNATE_OUTPUT_PUSH_PULL(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 8UL | GPIO_PIN_##SPEED()) }

#define GPIO_PIN_CONFIGURATION_ALTERNATE_OUTPUT_OPEN_DRAIN(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN##PIN##_CONFIGURATION(PORT, 12UL | GPIO_PIN_##SPEED()) }

//-----------------------------------------------------------------------------
#define GPIO_PIN_CONFIGURATION(PORT, PIN, LEVEL, MODE, SPEED) \
			{ \
			  if((RCC->APB2ENR & RCC_APB2ENR_IOP##PORT##EN) != RCC_APB2ENR_IOP##PORT##EN) \
				{ \
			  	  RCC->APB2ENR |= RCC_APB2ENR_IOP##PORT##EN; \
				} \
			  GPIO_PIN_CONFIGURATION_##MODE(PORT, PIN, LEVEL, MODE, SPEED); \
			}

//-----------------------------------------------------------------------------
#define GPIO_PIN_ON_HIGH(PORT, PIN) \
			{ GPIO##PORT->BSRR |= (1UL << PIN); }

#define GPIO_PIN_ON_LOW(PORT, PIN) \
			{ GPIO##PORT->BSRR |= (1UL << PIN) << 16; }

#define GPIO_PIN_OFF_HIGH(PORT, PIN) \
			{ GPIO##PORT->BSRR |= (1UL << PIN) << 16; }

#define GPIO_PIN_OFF_LOW(PORT, PIN) \
			{ GPIO##PORT->BSRR |= (1UL << PIN); }

//-----------------------------------------------------------------------------
#define GPIO_PIN_ON(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN_ON_##LEVEL(PORT, PIN) }

#define GPIO_PIN_OFF(PORT, PIN, LEVEL, MODE, SPEED) \
			{ GPIO_PIN_OFF_##LEVEL(PORT, PIN) }

#define GPIO_PIN_REVERSE(PORT, PIN, LEVEL, MODE, SPEED) \
			{ if(GPIO_PIN_SIGNAL_##LEVEL(PORT, PIN)) { GPIO_PIN_OFF_##LEVEL(PORT, PIN) } \
			  else { GPIO_PIN_ON_##LEVEL(PORT, PIN) } }
//-----------------------------------------------------------------------------
#define GPIO_PIN_SIGNAL_HIGH(PORT, PIN) \
			( (GPIO##PORT->IDR & (1UL << PIN)) == (1UL << PIN) )

#define GPIO_PIN_SIGNAL_LOW(PORT, PIN) \
		( (GPIO##PORT->IDR & (1UL << PIN)) != (1UL << PIN) )

#define GPIO_PIN_SIGNAL(PORT, PIN, LEVEL, MODE, SPEED) \
			( GPIO_PIN_SIGNAL_##LEVEL(PORT, PIN) )

//-----------------------------------------------------------------------------
#define PIN_CONFIGURATION(PIN_DESCRIPTION) GPIO_PIN_CONFIGURATION(PIN_DESCRIPTION)
#define PIN_ON(PIN_DESCRIPTION) GPIO_PIN_ON(PIN_DESCRIPTION)
#define PIN_OFF(PIN_DESCRIPTION) GPIO_PIN_OFF(PIN_DESCRIPTION)
#define PIN_REVERSE(PIN_DESCRIPTION) GPIO_PIN_REVERSE(PIN_DESCRIPTION)
#define PIN_STATE(PIN_DESCRIPTION) GPIO_PIN_SIGNAL(PIN_DESCRIPTION)
#define PIN_N_STATE(PIN_DESCRIPTION) !GPIO_PIN_SIGNAL(PIN_DESCRIPTION)

#endif /* MCU_GPIO_H_ */
