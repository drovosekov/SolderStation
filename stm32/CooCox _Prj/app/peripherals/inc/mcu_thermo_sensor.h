#include "main.h"

//*****************************************************************************
//! \addtogroup DS18B20_Bit_Config DS18B20 Thermometer Resolution Configuration
//*****************************************************************************
//
//! DS18B20 9 Bits MAX CONVERSION TIME is 93.75ms (tCONV/8)
#define DS18B20_9BIT            0x1F
//
//! DS18B20 10 Bits MAX CONVERSION TIME is 187.5ms (tCONV/4)
#define DS18B20_10BIT           0x3F
//
//! DS18B20 11 Bits MAX CONVERSION TIME is 375ms (tCONV/2)
#define DS18B20_11BIT           0x5F
//
//! DS18B20 12 Bits MAX CONVERSION TIME is 750ms (tCONV)
#define DS18B20_12BIT           0x7F


//u8 find_sensor(void);
#ifdef SENS_2WIRE
	void get_temperature_2wire(int *hb, int *lb);
#endif

#ifdef SENS_3WIRE
	s8 get_temperature_3wire();
	void get_temperature_value(u8 *buf, int hb, int lb);
#endif
